#include "stack_machine.h"

StackMachine::StackMachine(int stack_size, long long default_stack_value, int max_jumps, int max_bits, int max_useful_outputs)
  : Machine(max_bits),
    stack_size_(stack_size),
    default_stack_value_(default_stack_value),
    max_jumps_(max_jumps),
    max_useful_outputs_(max_useful_outputs),
    current_nr_ops_(0){
  AddOps();
  stack_ = new long long[stack_size_];
}

StackMachine::~StackMachine(){
  delete[] stack_;
}

long long StackMachine::GetBits(int code_idx){
  if(current_nr_ops_ == 0){
    return code_idx;
  }

  if(code_idx >= kNrOps){
    return kConstPrefixMask | (1 << (code_idx - kNrOps + kConstPrefixLength));
  }
  return op_bits_[code_idx];
}

int StackMachine::ReadBits(long long code){
  if(current_nr_ops_ == 0){
    AddCode(code & 3);
    return 2;
  }

  if((code & kConstPrefixMask) == kConstPrefixMask){
    int i = kConstPrefixLength;
    while(!((code >> i) & 1)){
      i++;
      if(i == 40){
        printf("Something odd!\n");
        return 0;
      }
        
    }
    //printf("Const %d\n", i - kConstPrefixLength);
    AddCode(kNrOps + i - kConstPrefixLength);
    return i + 1;
  }

  /*NOTE: Can be made faster with lookup table*/
  for(int code_idx = 0; code_idx < kNrOps; code_idx++){
    int mask = (1 << nr_op_bits_[code_idx]) - 1;
    if((code & mask) == op_bits_[code_idx]){
      AddCode(code_idx);
      return nr_op_bits_[code_idx];
    }
  }
  return 0;
}

int StackMachine::GetNrBits(int code_idx){
  if(current_nr_ops_ == 0){
    return 2;
  }

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

void StackMachine::DelCode(int nr_bits){
  current_nr_ops_--;
  Machine::DelCode(nr_bits);
}

void StackMachine::Load(long long code, int nr_bits){
  current_nr_ops_ = 0;
  Machine::Load(code, nr_bits);
}

int StackMachine::AddOp(std::string name, int bit_length, int next_ok){
  op_names_[current_added_ops_] = name;
  nr_op_bits_[current_added_ops_] = bit_length;

  //Reverse 8 bit integer
  int rbits = (next_ok * 0x0202020202ULL & 0x010884422010ULL) % 1023;

  op_bits_[current_added_ops_++] = rbits;

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
  long long val = stack_[stackptr_];
  stack_[stackptr_] = default_stack_value_;
  stackptr_--;
  if(stackptr_ == -1) stackptr_ = stack_size_ - 1;
  return val;
}

void StackMachine::Execute(){
  std::fill_n(stack_, stack_size_, default_stack_value_);

  int iptr = 0;
  int used_jumps = 0;
  bool cflag = false;

  int totinstr = 0;
  stackptr_ = 0;
  output_length_ = 0;
  long long temp1, temp2;

  AddSuffix(current_program_[0]);

  while(true){
    int op = current_program_[1 + (iptr++)];

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
      temp1 = pop(); //C++ does not guarantee evaluation order
      cflag = temp1 < pop();
      break;
    case 4: //If
      if(not cflag) iptr++;
      break;
    case 5: //Jmp
      if(used_jumps++ < max_jumps_) iptr = pop() % (current_nr_ops_ - 1);
      break;
    case 6: //Out
      output_[output_length_++] = peek();
      if(output_length_ == max_useful_outputs_) iptr = current_nr_ops_;
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

    if(output_length_ == 0 && ++totinstr > kWakeup) break;
    

    if(iptr < 0) iptr = 0;
    else if(iptr >= current_nr_ops_) break;
  }
  current_nr_ops_ -= 3; //Remove suffix
}

int StackMachine::NrChoices(){
  if(nr_current_bits_ == 0){
    return 4;
  }

  int nrconsts = max_bits_ - (nr_current_bits_ + 2);
  if(nrconsts < (4 - 2)) return 0; //(smallest op - 2)
  return 16 + nrconsts;
}

void StackMachine::AddSuffix(int dest){
  current_program_[current_nr_ops_++] = 6;
  current_program_[current_nr_ops_++] = 16 + dest;
  current_program_[current_nr_ops_++] = 5;
}

void StackMachine::AddOps(){
  int next_ok = 0;
  current_added_ops_ = 0;
  next_ok = AddOp("Add", 4, next_ok);
  next_ok = AddOp("Mul", 4, next_ok);
  next_ok = AddOp("XOR", 4, next_ok);
  next_ok = AddOp("Less", 4, next_ok);
  next_ok = AddOp("If", 4, next_ok);
  next_ok = AddOp("Jmp", 4, next_ok);
  next_ok = AddOp("Out", 4, next_ok);
  next_ok = AddOp("Dup", 4, next_ok);

  next_ok = AddOp("Mod", 5, next_ok);
  next_ok = AddOp("Over", 5, next_ok);
  next_ok = AddOp("Swap", 5, next_ok);
  next_ok = AddOp("Div", 5, next_ok);
  next_ok = AddOp("OR", 5, next_ok);
  next_ok = AddOp("AND", 5, next_ok);
  next_ok = AddOp("Equal", 5, next_ok);
  AddOp("Neg", 5, next_ok);
}

std::string StackMachine::ShowCode(){
  std::string s = (boost::format("Setjmp %1%\n") % current_program_[0]).str();

  for(int i = 1; i < current_nr_ops_; i++){
    int op = current_program_[i];
    if(op >= kNrOps){
      s += (boost::format("Push %1%") % (op - kNrOps)).str();
    } else {
      s += op_names_[op];
    }
    s += "\n";
  }

  return s;
}

