# Optimisation Explorer

[![GitHub Actions](https://img.shields.io/endpoint.svg?url=https://actions-badge.atrox.dev/MarkTuddenham/OptimExplorer/badge&label=build&logo=none)](https://actions-badge.atrox.dev/MarkTuddenham/OptimExplorer/goto)
![GitHub](https://img.shields.io/github/license/marktuddenham/OptimExplorer.svg)

![GitHub top language](https://img.shields.io/github/languages/top/marktuddenham/OptimExplorer.svg)
![GitHub last commit](https://img.shields.io/github/last-commit/marktuddenham/OptimExplorer.svg)
![GitHub issues](https://img.shields.io/github/issues/marktuddenham/OptimExplorer.svg)
![GitHub closed issues](https://img.shields.io/github/issues-closed/marktuddenham/OptimExplorer.svg)

Explore the optimisation of a Deep Neural Network.

---

## Build Process

```bash
mkdir -p _build/

cmake -H. -B_build/
# or
cmake -H. -B_build/ -DCMAKE_BUILD_TYPE=Debug

# The above line may not work, if it doesn't then use:
pushd _build/ && cmake .. && popd

cmake --build _build/

# Run the tests
./bin/Tests

# Run the example
./bin/OptimExplorer
```

---

## Torch

C++ Torch can be downloaded from [pytorch.org](https://pytorch.org/).

You can now either install C++ Torch as system library and cmake will find it automatically or pass cmake the directory using `CMAKE_PREFIX_PATH`.

```bash
cmake -H. -B_build/ -DCMAKE_PREFIX_PATH=/path/to/libtorch
```

Note. I don't think GPU is supported in Debug mode.

---

## Resources

---

Copyright © 2020 Mark Tuddenham