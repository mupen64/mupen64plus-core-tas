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

typedef struct {
  void* context;
  void (*filter_inputs)(void* context, int port, BUTTONS* input);
  bool (*poll_present)(void* context, int port);
} m64ptas_input_handler;

typedef struct {
  void* context;
  void (*set_audio_rate)(void* context, unsigned int new_rate);
  void (*push_audio_samples)(void* context, const void* samples, size_t length);
} m64ptas_audio_handler;

typedef struct {
  void* context;
  uint32_t signature;
  
  uint32_t (*get_xd_size)(void* context);
  bool (*save_xd)(void* context, unsigned char* data, uint32_t length);
  bool (*load_xd)(void* context, const unsigned char* data, uint32_t length);
} m64ptas_save_handler;

/* CoreTAS_SetInputHandler()
 *
 * This function sets the input handler, which allows the frontend to read controller input
 * and potentially override it.
 */
typedef m64p_error (*ptr_CoreTAS_SetInputHandler)(const m64ptas_input_handler* input_handler);
#ifdef M64P_CORE_PROTOTYPES
EXPORT m64p_error CALL CoreTAS_SetInputHandler(const m64ptas_input_handler* input_handler);
#endif

/* CoreTAS_SetAudioHandler()
 * 
 * This function sets the audio handler, which allow the frontend to extract audio output from
 * the core without going through an audio plugin. They can also be unset by passing NULL for 
 * both callbacks. Callbacks execute asynchronously; care must be taken to ensure thread safety.
 *
 * By default, calling this method will disable the audio tap until explicitly enabled. See
 * CoreTAS_SetAudioTapEnabled() for more details.
 *
 * This function will error with M64ERR_INPUT_ASSERT if one callback is non-NULL and the other is.
 */
typedef m64p_error (*ptr_CoreTAS_SetAudioHandler)(const m64ptas_audio_handler* audio_handler);
#ifdef M64P_CORE_PROTOTYPES
EXPORT m64p_error CALL CoreTAS_SetAudioHandler(const m64ptas_audio_handler* audio_handler);
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