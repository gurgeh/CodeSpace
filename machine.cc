#include "machine.h"

int Machine::AddCode(int code_idx){
  long long op_bits = GetBits(code_idx);
  int op_nrbits = GetNrBits(code_idx);

  if(nr_current_bits_ + op_nrbits > max_bits_){
    return 0;
  }

  code_bits_ = (code_bits_ & ((1ULL << nr_current_bits_) - 1)) | (op_bits << nr_current_bits_);

  nr_current_bits_ += op_nrbits;

  return op_nrbits;
}

void Machine::DelCode(int nr_bits){
  nr_current_bits_ -= nr_bits;
}

void Machine::Load(long long code, int nr_bits){
  max_bits_ = nr_bits;
  nr_current_bits_ = 0;
  
  while(nr_bits>0){
    int nr_used_bits = ReadBits(code);
    //printf("Used %d\n", nr_used_bits);
    if(nr_used_bits <= 0){
      printf("Something odd - no bits used\n");
      return;
    }
    nr_bits -= nr_used_bits;
    code >>= nr_used_bits;
  }
}

std::string Machine::ShowOutput(){
  std::string s;
  for(int i = 0; i < output_length_; i++){
    s += (boost::format("%1%") % output_[i]).str();
    if(i + 1 != output_length_) s += ", ";
  }
  return s;
}
