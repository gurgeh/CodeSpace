#ifndef STACK_MACHINE_H
#define STACK_MACHINE_H

#include <string>
#include <string.h>

/*
  Print 
  Load bitseq
 */

const int kNrOps = 16;
const int kMaxProgramLength = 20;

class StackMachine : Machine{
 public:
  StackMachine(int stack_size, int max_jumps, int max_bits) 
    : Machine(max_bits),
      stack_size_(stack_size),
      max_jumps_(max_jumps),
      current_nr_ops_(0);

  ~StackMachine();

  void Execute();
  int NrChoices();
 
 protected:
  GetBits(int code_idx);
  GetNrBits(int code_idx);
  
 private:
  void AddOps();
  int AddOp(string name, int bit_length, int next_ok);

  void AddSuffix();

  long long op_bits_[kNrOps];
  int nr_op_bits_[kNrOps];
  string op_names_[kNrOps];
  int current_added_ops_;

  long long *stack_;

  int max_jumps_;
  int stack_size_;
  int current_nr_ops_;
  int current_program_[kMaxProgramLength];
};

#endif
