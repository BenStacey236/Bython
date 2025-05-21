# Bython
A recreation of the Python interpreter, written in C. Aiming to implement as many of the features of the language as possible.

## Useage
To build the program, run `make` in terminal, this should build the project an compile everything into a single executable. 

Currently, it compiles into `tokeniser`, so to then run the interpreter, you should run `./tokeniser <filePath>`, where filePath is a path to a .py file that you want to run.

To clean up the build after running, you can run `make clean` if you want to (this should remove the build folder with all compiled .o files in it)

## Features
The current feature list can be found by navigating to [features.md](features.md), or clicking the link text.