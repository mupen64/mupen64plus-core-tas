#ifndef M64P_TAS_H
#define M64P_TAS_H

#include "m64p_types.h"
#include "m64p_plugin.h"
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef void (*ptr_InputFilterCallback)(void* context, int port, BUTTONS* input);
typedef void (*ptr_AudioRateCallback)(void* context, unsigned int new_rate);
typedef void (*ptr_AudioSampleCallback)(void* context, void* samples, size_t length);

/* CoreTAS_SetInputCallback()
 *
 * This function sets the input callback, which allows the frontend to read controller input
 * and potentially override it.
 */
typedef void (*ptr_CoreTAS_SetInputCallback)(void* context, ptr_InputFilterCallback callback);
#ifdef M64P_CORE_PROTOTYPES
void CoreTAS_SetInputFilterCallback(void* context, ptr_InputFilterCallback callback);
#endif

/* CoreTAS_SetAudioCallbacks()
 * 
 * This function sets the audio callbacks, which allow the frontend to tap into audio output from
 * the core without going through an audio plugin.
 */
typedef void (*ptr_CoreTAS_SetAudioCallbacks)(void* context, ptr_AudioRateCallback rate_callback, ptr_AudioSampleCallback sample_callback);
#ifdef M64P_CORE_PROTOTYPES
void CoreTAS_SetAudioCallbacks(void* context, ptr_AudioRateCallback rate_callback, ptr_AudioSampleCallback sample_callback);
#endif

/* CoreTAS_SetAudioTapEnabled()
 * 
 * Enables or disables sending audio to the audio callbacks. This should be done *after*
 * setting the audio callbacks, otherwise an error will be thrown.
 */
typedef m64p_error (*ptr_CoreTAS_SetAudioTapEnabled)(bool value);
#ifdef M64P_CORE_PROTOTYPES
m64p_error CoreTAS_SetAudioTapEnabled(bool value);
#endif

#ifdef __cplusplus
}
#endif
#endif