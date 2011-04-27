#include <stdio.h>

#include "stack_machine.h"
/*


An optimal encoding gives as many unique sequences and as many OEIS hits
as possible for a certain depth.
  Measure current score for those two
  Make 16 new operators and let all have 5 bits - measure score again
    sqrt, not, pow, 
  Calculate huffman coding
  Measure score after recoding of operators (1-gram, 2-gram, 3-gram, more?)

--

give command line option to check only part of a depth, for parallellization

--
Profiling run


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
