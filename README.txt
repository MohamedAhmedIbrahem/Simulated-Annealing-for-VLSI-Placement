How to run the code
--------------------
- compile the main.cpp file using g++ commpiler with command (g++ main.cpp -o main)
- run the compiled file using (./main). 
- it should ask for file name that contains the net list. for example testCase1.txt
- Give it the file name and it would work.

Note: there are functions inside the code that generates random netlists given the number of modules. you can use it.

Test-Cases file structure
-------------------------
- the first line contains the number of modules (n)
- followed by (n) lines each line represents the connections of the current module with another module, 
the first line of them is the connection of the first module with other modules and so on.
- these connections represents a single wire between 2 modules, for example if the first line contains 5 then it means that module 1 has a wire connected to module 5,
and there should be a (1) in line 5. (the connections are written in both lines).
more clearly if you want to make a wire between module 1,5 then line 1 should contains 5, and line 5 should contains 1.
- to connect another wire between the same modules it must be written separatly.

- No prequisites are needed to run the program, we only use the standard STL library.

Team information
----------------
Name                    Sec.        B.N.
Reham Gamal             1           21
Mazen Amr Fawzy         2           8
Mohamed Ahmed Ibrahim   2           9
Mahmoud Ahmed Sebak     2           20