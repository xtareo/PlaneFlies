# PlaneFlies

#### Introduction
A small game developed using VSCode, C language, and SDL3. The interface UI is currently being developed, and the game logic has not yet been written. We also welcome guidance from experts on the code.

#### Software architecture
Developed using SDL3 third-party libraries.

#### Installation Guide
(Taking VSCode editor as an example, other editors can also be used)
1. First, download MinGW64 and configure the environment variables, then install CMake and configure the environment variables.
2. Open VSCode, install C language related extension plugins and CMake Tools plugin, and restart the editor.
3. Extract the project files, open the project folder with VSCode, and use the CMake Tool to build the project. The built folder is called 'build', and the executable and resource files are in the bin folder.

#### Instructions for Use
(Taking VSCode editor as an example, other editors can also be used)
1. Use the CMake Tool to run the executable file, or manually open the bin folder and find the executable file to run.

#### Update
2025-10-10:Added controls to follow window scaling and fixed the issue of log output errors when reading language files.

2025-10-11:Fixed the layer issue of the button above the font and replaced the material of the control.

2025-10-13:Fixed the issue where multiple controls cannot be enlarged together and added detection for multiple control regions.

2025-10-18:Fixed the button text not being centered and added practical functionality to the exit button.
