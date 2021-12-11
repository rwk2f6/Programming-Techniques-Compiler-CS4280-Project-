Storage=Local

Author: Ryan Kelly

Class: Computer Science 4280

School: University of Missouri - Saint Louis

Assignment: Project 4 - Code Generation

Due Date: 12/10/2021

Language Used: C++

Description: The program is to parse the input, generate a parse tree, perform static semantics, and then generate a target file. Any error should display detailed message, including line number if available (depending on scanner). Since P1-P3 were already tested, this focuses on testing P target code and and storage allocation. Testing will be performed by running the generated targets to assess if the runtime benhavior is correct. The target file should generate in the UMSL ASM Language. This particular project also stores variables in a global and local scope.

The target file name should be:
kb.asm if keyboard input
file.asm if file was the input. The base name matches that of the input file, that is if input basename is t2 then output should be t2.asm.

Build Instructions (Linux Terminal): make

Delete compfs Executable and Output Files: make clean

How to run with a file named 'inputFile': ./compfs inputFile

How to run from Keyboard: ./compfs

To redirect the 'inputFile' to stdin: ./compfs < inputFile

