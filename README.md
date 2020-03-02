# GameVoid
GameBoy emulator for learning.

Tested on Linux and Windows.

# Building

Clone and initialize the repository:
```
git clone https://github.com/PZerua/GameVoid.git
cd GameVoid
git submodule update --init
```
Generate project:
```
mkdir build
cd build
cmake ..
```
On Windows, open .sln and compile solution.

On Linux:
```
make -j`nproc`
```
Windows usage:
```
GameVoid.exe <path to rom>
```
Linux usage:
```
GameVoid <path to rom>
```

# Controls
A -> A in keyboard\
B -> S in keyboard\
Start -> Return key\
Select -> Space key\
Movement -> Arrow keys
