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

void StackMachine::push(long long val){
  stackptr_ = (stackptr_ + 1) % stack_size_;
  stack_[stackptr_] = val;
}

long long StackMachine::peek(){
  return stack_[stackptr_];
}

long long StackMachine::pop(){
  long long val = stack_[stackptr_--];
  if(stackptr_ == -1) stackptr_ = stack_size_ - 1;
  return val;
}

void StackMachine::Execute(){
  AddSuffix();
  memset(stack_, default_stack_value_, stack_size_ * sizeof(long long));
  int iptr = 0;
  int used_jumps = 0;
  bool cflag = false;

  stackptr_ = 0;
  output_length_ = 0;
  long long temp;

  while(true){
    int op = current_program_[iptr++];
    switch(op){
    case 0: //Add
      push(pop() + pop());
      break;
    case 1: //Mul
      push(pop() * pop());
      break;
    case 2: //XOR
      push(pop() ^ pop());
      break;
    case 3: //Less
      temp1 = pop(); //C++ does not guarantee order
      cflag = temp1() < pop();
      break;
    case 4: //Jump
      if(used_jumps++ < max_jumps) iptr = pop() % current_nr_ops_;
      break;
    case 5: //If
      if(not cflag) iptr++;
      break;
    case 6: //Out
      output_[output_length_++] = peek();
      break;
    case 7: //Dup
      push(peek());
      break;
    case 8: //Mod
      temp1 = pop();
      temp2 = pop();
      if(temp2 == 0) push(0);
      else push(temp1 % temp2);
      break;
    case 9: //Over
      temp1 = pop();
      temp2 = pop();
      push(temp1);
      push(temp2);
      push(temp1);
      break;
    case 10: //Swap
      temp1 = pop();
      temp2 = pop();
      push(temp2);
      push(temp1);
      break;
    case 11: //Div
      temp1 = pop();
      temp2 = pop();
      if(temp2 == 0) push(temp1);
      else push(temp1 / temp2);
      break;
    case 12: //OR
      push(pop() | pop());
      break;
    case 13: //AND
      push(pop() & pop());
      break;
    case 14: //Equal
      cflag = (pop() == pop());
      break;
    case 15: //Neg
      push(-pop());
      break;
    default: //Push const
      push(op - 16);
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

  next_ok = AddOp("Mod", 8, next_ok);
  next_ok = AddOp("Over", 8, next_ok);
  next_ok = AddOp("Swap", 8, next_ok);
  next_ok = AddOp("Div", 8, next_ok);
  next_ok = AddOp("OR", 8, next_ok);
  next_ok = AddOp("AND", 8, next_ok);
  next_ok = AddOp("Equal", 8, next_ok);
  AddOp("Neg", 8, next_ok);
}
