 Project Overview : 
Switchback Rails is a deterministic 2D railway simulation game implemented in C++. The simulation controls trains moving on a grid-based track system.

 Features :
 
Grid-based track system with multiple tile types (-, |, /, , +, S, D, =, A-Z)
Terminal grid output at each tick
Comprehensive logging (trace.csv, switches.csv, signals.csv, metrics.txt)
Switch logic with GLOBAL and PER_DIR modes

Advanced Features :

Having colourful train lights like green,red and purple
Having colourful switches like green,red and purple
 Multiple train scheduling with tick-based spawning
 
 Project Structure :
 
 PF-Project-SwitchbackRails/
├── main.cpp                    # Console entry point
├── Makefile                    # Build system
├── README.md                   # This file
├── libraries.sh                # Installation script
├── core/                       # Core simulation logic
│   ├── grid.cpp/.h            # Grid management and validation
│   ├── io.cpp/.h              # File I/O and logging
│   ├── simulation_state.cpp/.h # Global state variables
│   ├── switches.cpp/.h        # Switch logic and signals
│   └── trains.cpp/.h          # Train movement and collision
├── levels/                     # Level files
│   ├── easy_level.lvl
│   ├── medium_level.lvl
│   ├── hard_level.lvl
│   └── complex_network.lvl
├── out/                        # Generated output files
│   ├── trace.csv
│   ├── switches.csv
│   ├── signals.csv
│   └── metrics.tx
   sfml 
    └── main_sfml.cpp,app.cpp,app.h
    
Prerequisites :

Operating System: Linux (Ubuntu recommended)
Compiler: g++ with C++11 support
Libraries: Standard C++ library

Build and Run :

make            # Compile the game
make run        # Run with default level
make clean      # Clean build files

 Run specific level
./switchback_rails data/levels/simple_test.lvl
./switchback_rails data/levels/full_network.lvl
./switchback_rails data/levels/complex_network.lvl

Control Buttons 
SPACE: Pause/Resume simulation
Up-Key: it moves faster to the destination
Down-Key: it moves slower to the destination
ESC: Exit and save metrics

Output files : 
trace.csv - Complete train movement history
switches.csv - Switch state changes per tick
signals.csv - Signal light states (GREEN/YELLOW/RED)
metrics.txt - Final statistics and efficiency metric

LEVELS :
easy_level.lvl - 2 trains, simple railway with switches 
medium_level.lvl - 3 trains, simple railway with switches
hard_level.lvl - 3 trains, but it contains more switches than the rest two levels 
complex_network.lvl - 4 trains, but it contains no barrier switches



 


    
 
 




