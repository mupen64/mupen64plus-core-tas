#include "api/m64p_types.h"
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define M64P_CORE_PROTOTYPES
#include "m64p_tas.h"
#include "tas_callbacks.h"



static bool input_handler_enabled = false;
static m64ptas_input_handler input_handler = {};

static bool audio_handler_enabled = false;
static bool audio_tap_enabled = false;
static unsigned int cur_audio_rate = 0;
static m64ptas_audio_handler audio_handler = {};

static bool save_handler_enabled = false;
static m64ptas_save_handler save_handler = {};

// external API
// ==================

m64p_error CoreTAS_SetInputHandler(const m64ptas_input_handler* new_input_handler) {
#ifdef M64P_TAS
  if (new_input_handler == NULL) {
    input_handler_enabled = false;
  }
  else {
    if (
      new_input_handler->filter_inputs == NULL
    ) {
      return M64ERR_INPUT_ASSERT;
    }
    input_handler_enabled = true;
    memcpy(&input_handler, new_input_handler, sizeof(m64ptas_input_handler));
  }
  return M64ERR_SUCCESS;
#else
  return M64ERR_UNSUPPORTED;
#endif
}

m64p_error CoreTAS_SetAudioHandler(const m64ptas_audio_handler* new_audio_handler) {
#ifdef M64P_TAS
  // rate callback and sample callback must either both be null or both be non-null
  if (new_audio_handler == NULL) {
    audio_tap_enabled = false;
    audio_handler_enabled = false;
  }
  else {
    if (
      new_audio_handler->push_audio_samples == NULL ||
      new_audio_handler->set_audio_rate == NULL
    ) {
      return M64ERR_INPUT_ASSERT;
    }
    audio_handler_enabled = true;
    memcpy(&audio_handler, new_audio_handler, sizeof(m64ptas_audio_handler));
  }
  return M64ERR_SUCCESS;
#else
  return M64ERR_UNSUPPORTED;
#endif
}

m64p_error CoreTAS_SetAudioTapEnabled(bool value) {
#ifdef M64P_TAS
  if (!audio_handler_enabled) {
    return M64ERR_INVALID_STATE;
  }

  audio_tap_enabled = value;

  // notify FE of current rate before sending any samples
  if (audio_tap_enabled)
    audio_handler.set_audio_rate(audio_handler.context, cur_audio_rate);

  return M64ERR_SUCCESS;
#else
  return M64ERR_UNSUPPORTED;
#endif

}

m64p_error CoreTAS_SetSavestateHandler(m64ptas_save_handler const* new_save_handler) {
#ifdef M64P_TAS
  if (new_save_handler == NULL) {
    save_handler_enabled = false;
  }
  else {
    if (
      new_save_handler->signature == 0 ||
      new_save_handler->alloc_size == 0 ||
      new_save_handler->save_extra_data == NULL ||
      new_save_handler->get_data_size == NULL ||
      new_save_handler->load_extra_data == NULL
    ) {
      return M64ERR_INPUT_ASSERT;
    }
    save_handler_enabled = true;
    memcpy(&save_handler, new_save_handler, sizeof(m64ptas_save_handler));
  }
  return M64ERR_SUCCESS;
#else
  return M64ERR_UNSUPPORTED;
#endif

}

// internal API
// ==============

BUTTONS tas_filter_inputs(int port, BUTTONS input) {
  if (!input_handler_enabled)
    return input;

  input_handler.filter_inputs(input_handler.context, port, &input);
  return input;
}

bool tas_override_present(int port) {
  if (!input_handler_enabled)
    return false;

  return input_handler.poll_present(input_handler.context, port);
}

void tas_rate_changed(unsigned int new_rate) {
  // save the current audio rate
  cur_audio_rate = new_rate;

  // if FE is listening for audio events, feed to FE
  // if (audio_rate_callback == NULL || !audio_tap_enabled)
  //   return;
  // audio_rate_callback(audio_context, cur_audio_rate);
  if (!audio_handler_enabled || !audio_tap_enabled)
    return;
  audio_handler.set_audio_rate(audio_handler.context, cur_audio_rate);
}

void tas_push_samples(const void* data, size_t length) {
  if (!audio_handler_enabled || !audio_tap_enabled)
    return;
  audio_handler.push_audio_samples(audio_handler.context, data, length);
}

const m64ptas_save_handler* get_save_handler() {
  if (save_handler_enabled)
    return &save_handler;
  else
    return NULL;
}