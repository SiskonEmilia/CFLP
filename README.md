# CFLP Solutions

We use climbing hill algorithm and genetic algorithm to solve CFLP problem. This repository contains all related code. All program compiled with g++ has passed all 71 benchmark cases on MacOS.

## Directory Structure

```text
- CFLP/
| - CH/: Code for Climbing Hill Algorithm
| - GA/: Code for Genetic Algorithm
| - Instances/: 71 benchmark cases
```

## Usage

After enter any of the two algorithm folders, use command below to start executing the algorithm and compute result for 71 benchmark cases in `Instances/`.

```cpp
g++ *.cpp *.hpp -std=c++11
./a.out
```