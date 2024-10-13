#include "api/m64p_types.h"
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define M64P_CORE_PROTOTYPES
#include "m64p_tas.h"
#include "tas_callbacks.h"

static m64p_input_filter_callback input_filter_callback = NULL;
static void* input_context = NULL;

static m64p_audio_rate_callbck audio_rate_callback = NULL;
static m64p_audio_sample_callback audio_sample_callback = NULL;
static void* audio_context = NULL;
static bool audio_tap_enabled = false;
static unsigned int cur_audio_rate = 0;

static m64ptas_save_handler savestate_handler = {0};

// external API
// ==================

m64p_error CoreTAS_SetInputFilterCallback(void* context, m64p_input_filter_callback callback) {
#ifdef M64P_TAS
  input_context = context;
  input_filter_callback = callback;
  return M64ERR_SUCCESS;
#else
  return M64ERR_UNSUPPORTED;
#endif
}

m64p_error CoreTAS_SetAudioCallbacks(void* context, m64p_audio_rate_callbck rate_callback, m64p_audio_sample_callback sample_callback) {
#ifdef M64P_TAS
  // rate callback and sample callback must either both be null or both be non-null
  if ((rate_callback != NULL) ^ (sample_callback != NULL)) {
    return M64ERR_INPUT_ASSERT;
  }
  
  audio_tap_enabled = false;
  
  audio_context = context;
  audio_rate_callback = rate_callback;
  audio_sample_callback = sample_callback;

  return M64ERR_SUCCESS;
#else
  return M64ERR_UNSUPPORTED;
#endif

}

m64p_error CoreTAS_SetAudioTapEnabled(bool value) {
#ifdef M64P_TAS
  if (audio_rate_callback == NULL || audio_sample_callback == NULL) {
    return M64ERR_INVALID_STATE;
  }

  audio_tap_enabled = value;

  // notify FE of current rate before sending any samples
  if (audio_tap_enabled)
    audio_rate_callback(audio_context, cur_audio_rate);

  return M64ERR_SUCCESS;
#else
  return M64ERR_UNSUPPORTED;
#endif

}

m64p_error CoreTAS_SetSavestateHandler(m64ptas_save_handler const* save_handler) {
#ifdef M64P_TAS
  memcpy(&savestate_handler, save_handler, sizeof(m64ptas_save_handler));
  return M64ERR_SUCCESS;
#else
  return M64ERR_UNSUPPORTED;
#endif

}

// internal API
// ==============

BUTTONS tas_filter_inputs(int port, BUTTONS input) {
  if (input_filter_callback == NULL)
    return input;

  input_filter_callback(input_context, port, &input);
  return input;
}

void tas_rate_changed(unsigned int new_rate) {
  // save the current audio rate
  cur_audio_rate = new_rate;

  // if FE is listening for audio events, feed to FE
  if (audio_rate_callback == NULL || !audio_tap_enabled)
    return;
  audio_rate_callback(audio_context, cur_audio_rate);
}

void tas_push_samples(const void* data, size_t length) {
  if (audio_sample_callback == NULL || !audio_tap_enabled)
    return;
  audio_sample_callback(audio_context, data, length);
}