# protocol.h 

This is the explanation for the `protocol.h` file - assuming that the reader does not know how to write a C program.

**Here is the line by line explanation :**

```c
// file : protocol.h 
```

It is a good habit to start you file with some kind of comment - that will give an overview, a metadata for the programmer to read the file. When you cat it in a while loop, in case you do something via `grep`, `sed`, replace something, these could be helpful. 

```c
#ifndef PROTOCOL_H
#define PROTOCOL_H

// ... the code goes here.

#endif
```

`protocol.h` is a header file. Meaning, here is where you provide **declarations** for the variables, functions, and define Macros. A google search will help you with Macros. 

`#ifndef PROTOCOL_H` - this line literally means : if not defined some macro PROTOCOL_H, 
`#define PROTOCOL_H` - follows it : then define the PROTOCOL_H
`// .. the code below` - Literally, this is the entire macro called PROTOCOL_H. 

If the macro is not defined, then we define it. That's it. Here the macro is the set of codes that are between the two lines as given above.
`#endif` - Finally we end the if statement we started earlier when we have to define the macro.

### But why so serious? 

This is because - in C, it acts as an **header guard** - This prevents the header from being processed more than just once per translation units. Our `protocol.h` does not have any definitions. (google search about C definitions and declarations if you don't know about them.)

But...

In case you have defined a `struct` that has to be used by various C source files, you include it in the header file, and that header file is used by may source files, which all ultimately compile to one single binary - **in case you have not included a header guard** : you would end up providing multiple definitions of the same `struct` or `function` or `variable or constant`. 

>[!IMPORTANT]
> In C - you can have multiple **identical declarations**, but you need to have exactly **one definition** for that declaration.

**Below given code is an example of multiple non-conflicting declarations :**


```c
// name : multi-declaration.c 
// date : 2026-06-28 
// std  : C 23 standard 


//---------------------------------- include header files -------------------------------
#include <stdio.h> 
#include <stdlib.h> 

// multi-declaration
void function(void);
void function(void);

//--------------------------------------- main program ----------------------------------
int main(void) {
    puts("Multiple declarations.");
    function();
    return EXIT_SUCCESS;
}

// single definition
void function(void) {
    puts("success");
}

```

output : 

```txt
Multiple declarations.
success

```

**This below given code is example for multi definitions of the same struct**

```c 
// name : multi-definition.c 
// date : 2026-06-28 
// std  : C 23 standard 


//---------------------------------- include header files -------------------------------
#include <stdio.h> 
#include <stdlib.h> 

// se what happens if multiple defintion of the same struct is provided.

typedef struct {
    int a;
    char i[4];
    double b;
} mystruct_t;

// comes the second definition

typedef struct {
    int a;
    char i[4];
    double b;
} mystruct_t;

// note that we account for the null terminator too. 
mystruct_t MyStruct = {4, "why", 234.23};

//--------------------------------------- main program ----------------------------------
int main(void) {
    

    puts("Multi defintion");
    printf("String : %s\n", MyStruct.i);
    return EXIT_SUCCESS;
}

```

output : 

```txt 
multi-definition.c:24:3: error: conflicting types for ‘mystruct_t’; have ‘struct <anonymous>’
   24 | } mystruct_t;
      |   ^~~~~~~~~~
multi-definition.c:16:3: note: previous declaration of ‘mystruct_t’ with type ‘mystruct_t’
   16 | } mystruct_t;
      |   ^~~~~~~~~~
make: *** [/home/cobra/.Makefile-default:8: run] Error 1
```

**And this is why the `#ifndef, #define, #endif`** model exists. But you can simplify the three lines to one line. But this requires modern C. (see below.)

>[!IMPORTANT]
> In new C compilers, which support `c23` standard and have `c2y` demo, like `clangd`, `gcc` in arch and bleeding edge releases, you have `#pragma once` preprocessor command, which literally means - include this header file only once. We can use it like :

```c
#pragma once

// ... the code goes here 
```

### `extern`


