#pragma once
extern int atomicBlockEnter;
extern int atomicBlockExit;
#define ATOMIC_RESTORESTATE 1
#define ATOMIC_BLOCK(x) for (int once = (++atomicBlockEnter, 0); !once; ++once, ++atomicBlockExit)
