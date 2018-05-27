# subprocess_ipc

A small library to simplify running and interacting with other programs, such as shells, etc., programmatically in a C program.



## Warning
Executing shell commands that incorporate unsanitized input from an untrusted source makes a program vulnerable to shell injection, a serious security 
flaw which can result in arbitrary command execution. Use of this library is strongly discouraged in cases where the command string is constructed from 
external input.


## Compiling

make subprocess_ipc.so


## Installation

