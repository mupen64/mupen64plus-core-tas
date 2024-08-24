#ifndef M64P_TAS_H
#define M64P_TAS_H

#include "m64p_types.h"
#include "m64p_plugin.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef void (*ptr_InputFilterCallback)(void* context, int port, BUTTONS* input);
typedef void (*ptr_AudioRateCallback)(void* context, unsigned int new_rate);
typedef void (*ptr_AudioSampleCallback)(void* context, void* samples, size_t length);

typedef struct {
  uint32_t id_code;
  uint32_t version;
  void* context;
  
  m64p_error (*save_extra_data)(void* context, void** data, size_t* length);
  m64p_error (*load_extra_data)(void* context, void* data, size_t length);
} m64ptas_save_handler;

/* CoreTAS_SetInputCallback()
 *
 * This function sets the input callback, which allows the frontend to read controller input
 * and potentially override it.
 */
typedef void (*ptr_CoreTAS_SetInputCallback)(void* context, ptr_InputFilterCallback callback);
#ifdef M64P_CORE_PROTOTYPES
EXPORT void CALL CoreTAS_SetInputFilterCallback(void* context, ptr_InputFilterCallback callback);
#endif

/* CoreTAS_SetAudioCallbacks()
 * 
 * This function sets the audio callbacks, which allow the frontend to tap into audio output from
 * the core without going through an audio plugin. They can also be unset by passing NULL for 
 * both callbacks. Callbacks execute asynchronously; care must be taken to ensure thread safety.
 *
 * By default, calling this method will disable the audio tap until explicitly enabled. See
 * CoreTAS_SetAudioTapEnabled() for more details.
 *
 * This function will error with M64ERR_INPUT_ASSERT if one callback is non-NULL and the other is.
 */
typedef m64p_error (*ptr_CoreTAS_SetAudioCallbacks)(void* context, ptr_AudioRateCallback rate_callback, ptr_AudioSampleCallback sample_callback);
#ifdef M64P_CORE_PROTOTYPES
EXPORT m64p_error CALL CoreTAS_SetAudioCallbacks(void* context, ptr_AudioRateCallback rate_callback, ptr_AudioSampleCallback sample_callback);
#endif

/* CoreTAS_SetAudioTapEnabled()
 * 
 * Enables or disables sending audio to the audio callbacks.
 * 
 * This function will error with M64ERR_INVALID_STATE if the audio callbacks are not set.
 */
typedef m64p_error (*ptr_CoreTAS_SetAudioTapEnabled)(bool value);
#ifdef M64P_CORE_PROTOTYPES
EXPORT m64p_error CALL CoreTAS_SetAudioTapEnabled(bool value);
#endif

/* CoreTAS_SetSavestateHandler()
 *
 * Sets a savestate handler to inject and retrieve extra data from savestates. Callbacks
 * execute asynchronously; care must be taken to ensure thread safety.
 */
typedef m64p_error (*ptr_CoreTAS_SetSavestateHandler)(m64ptas_save_handler const* save_handler);
#ifdef M64P_CORE_PROTOTYPES 
EXPORT m64p_error CALL CoreTAS_SetSavestateHandler(m64ptas_save_handler const* save_handler);
#endif

#ifdef __cplusplus
}
#endif
#endif