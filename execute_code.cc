#include <stdio.h>
#include <stdlib.h>

#include "stack_machine.h"

int main(int argc, char *argv[]){
  int i = 1;
  long long code = atol(argv[i++]);
  int code_len = atoi(argv[i++]);
  int max_jmps = atoi(argv[i++]);
  int stack_size = atoi(argv[i++]);
  int stack_default = 1;
  
  StackMachine sm(stack_size, stack_default, max_jmps, code_len, 1000000000);

  sm.Load(code, code_len);
  printf("--- Code (%d) ---\n%s\n\n", sm.get_nr_current_bits(), sm.ShowCode().c_str());
  
  sm.Execute();
  printf("%s\n", sm.ShowOutput().c_str());

  return 0;
}
