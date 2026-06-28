# Plan?

## Project Structure 
This is a project that is built on top of a template structure for linux. Previously, I have created a repo called [CTemplate](https://github.com/cobra-r9/CTemplate) whose sole purpose is to be cloned and edited, so I need not create a project structure from scratch. You will be able to find the example project structure in the `readme.md` of that repo.

- All the header files `*.h` goes into the `include/` directory. 
- All the source files `*.c` goes into the `src/` directory. 
- Each header file will have its corresponding source file, apart from the targets that are being built. That is in our case - except `pomoc.c` and `pomoc.d`

## Files 

Starting with the files - what they are and what they have, what they do. 

### protocol - `protocol.h` and `protocol.c`

The header file contains the constants - the command names, the socket paths - just constants that would be used for the communication between the daemon, socket and the client. Header has only the declarations of the constants, and tells the compiler to find the definition from the object file via `extern`. (object files are created by the Makefile, in obj directory)

While the source file contains the definitions of the constants. When this file is compiled to the object file, the `ld` would find the definitions from its object file for constants that are declared in the header file.

### state - `state.h` and `state.c`

The header file consists of the PomState enum type for the state of the pomodoro and the struct definition for the PomData, which is the data of the pomodoro around which the entire process revolves around. And the functions for initialising the data as idle, returning the state of the pomodoro whether the break is active or focus is running, and function to return the state.

The source file consists of its definitions.

### config - `config.h` and `config.c`

The header file has the declarations for config path and a function to the config values to the pomodoro data. 
Wheras the source file has the functions defined there.

### timer - `timer.h` and `timer.c`

These are the files for modifying the time data, updating the remaining time and start the countdown process.

### pomoc.c - the pomodoro client

This is much more simpler than the pomod.c (which comes next). This would connect to the socket which was created by the daemon, then send commands to the daemon, when then updates the pomodoro and its state. This would be as a binary : `pomoc`

### pomod.c - the pomodoro daemon

I made this pomodoro as a `client-daemon` model because - you would be able to interact with a single running timer from 5 different programs, each having their own client. For example, say you use the `pomoc` to poll and output the state of the timer, the live running time in the polybar or any other bar system. This is the first client. What if you need to update the time? pause it? from another program on the basis of a set of logic you baked in that program? you call out the second client and use it to modify the state. This `pomod.c` has the logic of **what reply to provide for each command from the client.**


