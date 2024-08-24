#include "api/m64p_types.h"
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define M64P_CORE_PROTOTYPES
#include "m64p_tas.h"
#include "tas_callbacks.h"

static ptr_InputFilterCallback input_filter_callback = NULL;
static void* input_context = NULL;

static ptr_AudioRateCallback audio_rate_callback = NULL;
static ptr_AudioSampleCallback audio_sample_callback = NULL;
static void* audio_context = NULL;
static bool audio_tap_enabled = false;

static m64ptas_save_handler savestate_handler = {0};

// external API
// ==================

void CoreTAS_SetInputFilterCallback(void* context, ptr_InputFilterCallback callback) {
  input_context = NULL;
  input_filter_callback = callback;
}

m64p_error CoreTAS_SetAudioCallbacks(void* context, ptr_AudioRateCallback rate_callback, ptr_AudioSampleCallback sample_callback) {
  // rate callback and sample callback must either both be null or both be non-null
  if ((rate_callback != NULL) ^ (sample_callback != NULL)) {
    return M64ERR_INPUT_ASSERT;
  }
  if (rate_callback != NULL) {
    audio_tap_enabled = false;
  }
  
  audio_context = context;
  audio_rate_callback = rate_callback;
  audio_sample_callback = sample_callback;

  return M64ERR_SUCCESS;
}

m64p_error CoreTAS_SetAudioTapEnabled(bool value) {
  if (audio_rate_callback == NULL) {
    return M64ERR_INVALID_STATE;
  }

  audio_tap_enabled = value;
  return M64ERR_SUCCESS;
}

m64p_error CoreTAS_SetSavestateHandler(m64ptas_save_handler const* save_handler) {
  memcpy(&savestate_handler, save_handler, sizeof(m64ptas_save_handler));
  return M64ERR_SUCCESS;
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
  audio_rate_callback(audio_context, new_rate);
}

void tas_push_samples(void* data, size_t length) {
  audio_sample_callback(audio_context, data, length);
}