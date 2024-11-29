# OS Class - Monitor & Virtual Memory Simulators

## Compile

With CMake (>= 3.5):
```bash
mkdir -p build
cmake -S . -B build
cd build
make
```

Without CMake (G++, etc.):
```bash
mkdir -p build
g++ src/simulator.cpp -o build/simulapc
g++ src/virtual_mem.cpp -o build/mvirtual
```

## Execute

Monitor simulator:
```bash
./build/simulapc -p <int:producers> -c <int:consumers> -s <int:queue_size> -t <int:max_wait_time>
```

Virtual memory simulator:
```bash
./build/mvirtual -m <int:frames> -a <string:algorithm> -f <string:filepath>
```

`algorithm` (case insensitive) must be one of: `OPTIMAL`, `FIFO`, `LRU`, `LRU_CLOCK`.
