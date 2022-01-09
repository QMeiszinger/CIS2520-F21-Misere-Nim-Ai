Student Name: Quinn Meiszinger
Class ID: CIS2520 (0101) F21 - Data Structures
Instructor: Stefan Kremer

=== SPECIAL NOTE ===
alphanim.c, nim.h, and nimhelp.c were files provided by the instructor.
All credit goes to Stefan Kremer. The nim.c and makefile are were created
by me. 

=== PROJECT DESCRIPTION ===
This was the final project for my CIS2520 Data Structures class. 
I had to create a Misere Nim AI from various data structures I had 
learnt throughout the course. This is the result.

=== COMPLIATION ===
To compile the code a makefile is included for easy compilation. Simply
navigate to the directory of the files and run the make command.
In the event that the makefile does not work for your machine,
run this command:

gcc -Wall -std=c99 -pedantic alphanim.c nimhelp.c nim.c -o alphanim

to run the program:

./alphanim <first_player> <matches>

