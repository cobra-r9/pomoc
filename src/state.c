// file : state.c 
// date : 27-06-2026
// std  : c23

#include <string.h>
#include "state.h"


// define the state_init function. 
void state_init(PomData *data) {
    // we use memset to set the fields to zero bytes. 
    // function signature of the memset is void *memset (void field[n], int value, size_t bytes);
    // we will set the full data struct to 0 bytes. 
    // this is required because, we need initialisation for the strut. 
    memset(data, 0, sizeof(PomData));
    
    // then we need to set the PomState to idle. That is not running anything. 
    data->state = STATE_IDLE;
    return;
}

// define the function the return the phase of pomodoro, 
const char *state_phase_str(PomData *data) {
    switch (data->state) {
        case STATE_BREAK_RUNNING: return "break";
        case STATE_IDLE: return "idle";
        default: return "focus";
    }
    // if the state is equal to STATE_BREAK_RUNNING, then return "break", else "focus"
}

// define the function to return the state of the pomodoro. 
// if the state is idle, the return idle, if running focus, return focus, 
// if it is paused, then return paused, if running break, return break. 
const char *state_str(PomData *data) {
    switch (data->state) {
        case STATE_IDLE: return "idle";
        case STATE_FOCUS_RUNNING: return "running";
        case STATE_FOCUS_PAUSED: return "paused";
        case STATE_BREAK_RUNNING: return "break";
        default: return "unknown";
    }
}

