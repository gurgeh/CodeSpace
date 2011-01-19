#ifndef STACK_MACHINE_H
#define STACK_MACHINE_H

#include <algorithm>
#include <string>
#include <string.h>
#include <stdio.h>

#include <boost/format.hpp>

#include "machine.h"

const int kWakeup = 14;
const int kNrOps = 16;
const int kMaxProgramLength = 20;
const int kConstPrefixLength = 2;
const long long kConstPrefixMask = (1 << kConstPrefixLength) - 1;

class StackMachine : public Machine{
 public:
  StackMachine(int stack_size, long long default_stack_value, int max_jumps, int max_bits, int max_useful_outputs);

  ~StackMachine();

  void Execute();
  int NrChoices();
 
  int AddCode(int code_idx);
  void DelCode(int nr_bits);

  void Load(long long code, int nr_bits);

  std::string ShowCode();
 protected:
  int ReadBits(long long code);

  long long GetBits(int code_idx);
  int GetNrBits(int code_idx);
  
 private:
  void AddOps();
  int AddOp(std::string name, int bit_length, int next_ok);

  void AddSuffix(int dest);

  void push(long long val);
  long long peek();
  long long pop();

  long long op_bits_[kNrOps];
  int nr_op_bits_[kNrOps];
  std::string op_names_[kNrOps];
  int current_added_ops_;

  long long *stack_;
  long long default_stack_value_;

  int stackptr_;
  int max_jumps_;
  int max_useful_outputs_;
  int stack_size_;
  int current_nr_ops_;
  int current_program_[kMaxProgramLength];
};

#endif
