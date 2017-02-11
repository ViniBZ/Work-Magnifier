/*



WorkMagnifier is a program created to further help me explore the underlaying functionality
of the OS, the kernel and its dependencies.

The program was originally made to run on MACOS X but if you find a otool -tV equivalent disassembler on 
windows it won't be so hard to make it work because it uses QT framework for the interface which is cross platform.

From now on I will refer to the program as Magnifier.
The disassembler used for this is otool  with -tV option in a Mac computer, so the syntax
the program reads is based on that.
Its first dependency is a file called lib_dir.txt which can be edited and created from 
inside the program itself in case it doesn't already exist. This file is meant to hold the
paths for the directories contaning the dynamic libraries general applications might use in
the system.
After the paths are correctly set the next thing to do is to use "disassemble libraries"
that will sweep the directories and its subdirectories looking for the libraries code files
and disassembling every library it can find to the "disass_lib" folder in the data folder
of Magnifier.
After having a collection of disassembled libraries the next step is to click "generate
index", so the program will sweep every disassembled library and make an index file containing
the name call for every function in the libraries its respective library and position.

Now the Magnifier is ready to do what I created it for.
It is to disassemble an executable and replace its function calls for the function implementation.
Then replace the newly imported function calls in the function implementations and so on.
This process is here called expansion.


Choose an executable or assembly code and set the options carefully:

*WARNING: A small executable disassembled all the way can result in a file tens of gigabytes 
long depending on the options.
*That must be the folders described in the paths on the top part of global_definitions.h for it to work

"nest" indicates how many levels it will dig deep(function call inside function implementation).

"expand local functions" does the replacing process even if the functions are local to the
program.

"Repeat functions" expands a function call even if it was already expanded previously.
By default it is unset.

"Keep intermediate nesting files" That is because for every nesting level the Magnifier 
creates a temporary file containing the expanded code. If you choose 3 levels of nesting for
example, the final file will be the third created.
By default it is unset so you only get the final file.

*/