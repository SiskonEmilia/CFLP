# GA for CFLP

This is an implementation in C++ of Genetic Algorithm for Capacitated Facility Location Problem. The program compiled with g++ has passed all tests on MacOS.

## Usage

### Compile & Run

```cpp
g++ *.cpp *.hpp -std=c++11
./a.out
```

### Looking for better configuration?

We recommend you to use 20% as your crossover rate, 5% as your mutation rate ('%' is not needed when you were asked to input these values) and 20 as your popularity size. This configuration has been tested to have a good balance between result quality and time consumed.

## Result

[This table](result.csv) shows its performance under the recommended configuration. And [this file](detail.txt) record more detail information about each case.