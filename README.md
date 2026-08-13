# lock-free-queue

This is a templated fixed-capacity circular buffer built for concurrent use by a producer and consumer thread, with no locks.
This project was mainly for me to learn how to use std::atomic and some other C++ 20 stuff. 

## Usage
To compile:
go to build folder

type ``` cmake .. ```

then:

type ``` make ```

To run / valgrind
(should do nothing lmao)

type ``` make run ```

type ``` make val ```

To test:

type ``` ctest ```