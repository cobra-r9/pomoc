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
