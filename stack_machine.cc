#include "stack_machine.h"

StackMachine::StackMachine(){
  AddOps();
  stack_ = new long long[stack_size_];
}

StackMachine::~StackMachine(){
  del stack[];
}

long long StackMachine::GetBits(int code_idx){
  if(code_idx >= kNrOps){
    return 3 | (1 << (code_idx - kNrOps + 2));
  }
  return op_bits_[code_idx];
}

int StackMachine::GetNrBits(int code_idx){
  if(code_idx >= kNrOps){
    return kConstPrefixLength + 1 + code_idx - kNrOps;
  }
  return nr_op_bits_[code_idx];
}

int StackMachine::AddCode(int code_idx){
  int n = Machine::AddCode(code_idx);
  if(n){
    current_program_[current_nr_ops_++] = code_idx;
  }

  return n;
}

int StackMachine::AddOp(string name, int bit_length, int next_ok){
  op_names_[current_nr_ops_] = name
  nr_op_bits_[current_nr_ops_] = bit_length;

  //Reverse 8 bit integer
  int rbits = (next_ok * 0x0202020202ULL & 0x010884422010ULL) % 1023;

  op_bits_[current_nr_ops_++] = rbits;

  next_ok += (1 << 7) >> (bit_length - 1);

  return next_ok;
}

void StackMachine::push(){
}

void StackMachine::peek(){
}

void StackMachine::pop(){
}

void StackMachine::Execute(){
  AddSuffix();
  memset(stack_, default_stack_value_, stack_size_ * sizeof(long long));
  iptr = 0;
  stackptr_ = 0;
  output_length_ = 0;
  used_jumps = 0;

  while(true){
    switch(current_program_[iptr++]){
    case 0: //Add
      break;
    case 1: //Mul
      break;
    case 2: //XOR
      break;
    case 3: //Less
      break;
    case 4: //Jump
      if(used_jumps++ < max_jumps) iptr = pop() % current_nr_ops_;
      break;
    case 5: //If
      break;
    case 6: //Out
      break;
    case 7: //Dup
      break;
    case 8: //Rot
      break;
    case 9: //Over
      break;
    case 10: //Swap
      break;
    case 11: //Div
      break;
    case 12: //OR
      break;
    case 13: //AND
      break;
    case 14: //Equal
      break;
    case 15: //Neg
      break;
    default: //Push const
    }

    if(iptr < 0) iptr = 0;
    else if(iptr == current_nr_ops_) break;
    else iptr %= current_nr_ops_;
  }
}

int StackMachine::NrChoices(){
  return 16 + (max_bits_ - (nr_current_bits_ + 2))
}

void StackMachine::AddOps(){
  int next_ok = 0;
  next_ok = AddOp("Add", 4, next_ok);
  next_ok = AddOp("Mul", 4, next_ok);
  next_ok = AddOp("XOR", 4, next_ok);
  next_ok = AddOp("Less", 4, next_ok);
  next_ok = AddOp("If", 4, next_ok);
  next_ok = AddOp("Jmp", 4, next_ok);
  next_ok = AddOp("Out", 4, next_ok);
  next_ok = AddOp("Dup", 4, next_ok);

  next_ok = AddOp("Rot", 8, next_ok);
  next_ok = AddOp("Over", 8, next_ok);
  next_ok = AddOp("Swap", 8, next_ok);
  next_ok = AddOp("Div", 8, next_ok);
  next_ok = AddOp("OR", 8, next_ok);
  next_ok = AddOp("AND", 8, next_ok);
  next_ok = AddOp("Equal", 8, next_ok);
  AddOp("Neg", 8, next_ok);
}
