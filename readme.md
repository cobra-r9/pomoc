# CTemplate 

**What this repo is about :**
This is just a template structure for how a well organised C project should be (from standard perspective) and this will help when the programs become larger in size. We should have some prerequisites covered here - knowledge about the C compiler, and Makefile and make system. (Which we will be learning here.)

**Standard Structure of the Project**

```txt 
CTemplate/
├── Makefile    # make build instructions for the project 
├── .clangd     # help the clangd lsp-config to resolve include directory
├── .gitignore  # ignore the files in your git 
├── LICENSE     # license for the project 
├── readme.md   # readme for the project 
├── PKGBUILD    # if you were an arch user.
├── clean.sh    # to clean the template's include and src. 
├── writeups/   # folder for giving explanations for files/writeups (optional)
├── include/    # include directory for header files. 
│   └── extmath.h
├── src/        # source directory for c source files. 
│   ├── main.c  # main c file 
│   └── extmath.c
└── build/      # This folder will be created and deleted automatically if you run make clean
    ├── bin/    # where the main program binary lives.
    ├── main.o
    └── extmath.o
```


**Make commands**

I have added several useful make build instructions.

1) Build the project.

```bash
make build 
# or 
make all
```

2) Run the main binary. 

This one builds the binary and then runs it. Here the build directory is not deleted and persists. 

```bash 
make run
```

3) Test the program 

This one will build the binary and then run it, then finally clean it. No build directory persists. 

```bash 
make test 
```

4) Clean the project structure 

This will remove the build directory and clean the object files. 

```bash 
make clean
```

>[!NOTE]
> The `main.c` and `extmath.c` source files and `extmath.c` header files are preexisting. I have also included a shell script called `clean.sh` for cleaning all those files from `src/` and `include/`. I have also added the `make reset` command which will reset the entire project to the baseline, same as clean.sh, but from make. 

**Arch `makepkg`**

For installing the binary system wide, if you are in an arch based repo, you need to use the `makepkg` to build and install the binary to standard paths. Run the command as given below the install it. 

```bash 
makepkg -si 
```

>[!IMPORTANT]
> This project CTemplate is only for linux based systems and the PKGBUILD is optional (have it if you are an arch based user), else delete it. This makefile structure has no compactness with windows or mac based or any other non linux OS. 
