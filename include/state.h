#ifndef STATE_H
#define STATE_H 

// define the state enum such that we can represent whether 
// it is idle : STATE_IDLE : not started, just normal. 
// currently focus is being run : STATE_FOCUS_FUNNING : the focus is running. 
// is paused : STATE_FOCUS_PAUSED : the running focus was paused. 
// bread is running : STATE_BREAK_RUNNING : the pomo has finished and the break is running instead. 
typedef enum {
    STATE_IDLE,
    STATE_FOCUS_RUNNING,
    STATE_FOCUS_PAUSED,
    STATE_BREAK_RUNNING,
} PomState;
// we call it the PomState 

// define a struct such that we can store the data. 
// state of the Pomodoro type PomState. 
// the focus hr, min and sec if being focused. 
// the break min and hr, (break should be in minutes, not in hours)
// the remaining time 

typedef struct {
    PomState state;

    int focus_hr;
    int focus_min;
    int focus_sec;

    int break_min;
    int break_sec;

    int remaining_hr;
    int remaining_min;
    int remaining_sec;

} PomData;

// Let us call this the PomData


// then we need some functions to deal with the data. 
// state_init to initialise the Data 
// initialy we use this function to set the fields to 0. 
void state_init(PomData *data);

// function to return whether the focus is running or break is running. 
const char *state_phase_str(PomData *data);

// function to return the current state of the pomodoro
const char *state_str(PomData *data);

#endif 
