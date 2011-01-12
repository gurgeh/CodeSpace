#ifndef MACHINE_H
#define MACHINE_H

/*
  stackMachine
--
  searchMachine
--
  run

  store in prefix tree (truncate?)
    tot with prefix
    shortest program, len, bits

  read bits

  profile

  incl two start bits

 */

const int kMaxOutput = 400;

class Machine{
 public:
  Machine(int max_bits) 
    : max_bits_(max_bits),
      nr_current_bits_(0),
      code_bits_(0){
  }

  virtual int AddCode(int code_idx);
  virtual void DelCode(int nr_bits);

  virtual void Execute() = NULL;
  virtual int NrChoices() = NULL;
 
 protected:
  virtual GetBits(int code_idx) = NULL;
  virtual GetNrBits(int code_idx) = NULL;

  int nr_current_bits_;
  int max_bits_;
  long long code_bits_;
  
  int output_length_;
  long long output_[kMaxOutput];
};

#endif
