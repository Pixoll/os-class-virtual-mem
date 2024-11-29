# OS Class - Monitor & Virtual Memory Simulators

## Compile

With CMake (>= 3.5):
```bash
mkdir build
cmake -S . -B build
cd build
make
```

Without CMake (GCC, etc.):
```bash
mkdir build
gcc src/simulator.cpp src/monitor_queue.hpp -o build/simulapc
gcc src/virtual_mem.cpp src/paging/*.hpp -o build/mvirtual
```

## Execute

Monitor simulator:
```bash
./build/simulapc
```

Virtual memory simulator:
```bash
./build/simulapc
```
