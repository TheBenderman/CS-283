Nathan Bender
3/17/17
CS 283
Project Final Report

I have included code for all of the empty functions in the original code in the
tsh.c file. I have tested the program thoroughly using the 4 provided c files:
myspin.c, mysplit.c, myint.c and mystop.c. I have tested all of the builtin
commands, such as fg, bg, jobs and quit. I have also tested the program using
unix functions such as ls, ps, and echo. Finally, I have also tested sending
SIGSTOP using ctrl-z and SIGINT using ctrl-c.

The following functions are filled in my project report:
- eval
- builtin_cmd
- do_bgfg
- waitfg
- sigchld_handler
- sigint_handler
- sigtstp_handler

I have included a basic makefile that will compile my program. I have also included
a file called Trace.txt that I have copied output from my program to. This output
shows me testing all of the various functions of the program.
