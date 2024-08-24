#ifndef API_TAS_CALLBACKS_H_INCLUDED
#define API_TAS_CALLBACKS_H_INCLUDED

#include "m64p_tas.h"

extern BUTTONS tas_filter_inputs(int port, BUTTONS input);

extern void tas_rate_changed(unsigned int new_rate);

extern void tas_push_samples(void* data, size_t length);

// extern void tas_save_savedata();

// extern void tas_load_savedata();

#endif