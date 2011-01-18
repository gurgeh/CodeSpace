#include <stdio.h>

#include "stack_machine.h"
/*
Why segfault in new factoring?

check prime programs (also allow one or two errors)
--
analyze shortest programs
measure how many sequences hit OEIS

An optimal encoding gives as many unique sequences and as many OEIS hits
as possible for a certain depth.

--
optimize execution with cutoffs

give command line options to run several processes

--

--
Profiling run

store in prefix tree (truncate?)
  tot with prefix
  shortest program, len, bits

optimize program generation - do not generate swap swap, for example.
  Although it could be useful if a jump goes to the second swap, it is so
  rare that we could disallow them in the language.

find interesting sequences and "best operators"

try giving programs input in stack

maybe give them addressable memory and more instructions

compile programs to x86 (GPU?)
  Use floating point?
(#include <fpu_control.h>

_FPU_SETCW (_FPU_DEFAULT);
Since _FPU_DEFAULT defaults to _FPU_MASK_ZM set (ZM stands for Zero-Divide-Mask).)

Randomize longer programs

Try with GA/GP

 */


int main(void){
  StackMachine sm(10, 1, 20, 0);


  return 0;
}
