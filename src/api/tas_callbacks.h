#ifndef API_TAS_CALLBACKS_H_INCLUDED
#define API_TAS_CALLBACKS_H_INCLUDED

#include "m64p_tas.h"

extern BUTTONS tas_filter_inputs(int port, BUTTONS input);

extern bool tas_override_present(int port);

extern void tas_rate_changed(unsigned int new_rate);

extern void tas_push_samples(const void* data, size_t length);

extern const m64ptas_save_handler* get_save_handler();

#endif