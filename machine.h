#ifndef MACHINE_H
#define MACHINE_H

#include <string>
#include<stdio.h>

const int kMaxOutput = 1000;

class Machine{
 public:
  Machine(int max_bits) 
    : max_bits_(max_bits),
      nr_current_bits_(0),
      code_bits_(0){
  }

  void set_max_bits(int max_bits){max_bits_ = max_bits;}
  int get_max_bits() const {return max_bits_;}

  bool Ready() const {return nr_current_bits_ == max_bits_;}

  int get_output_length() const {return output_length_;}
  const long long *get_output() const {return output_;}

  long long get_code() const {return code_bits_;}
  int get_nr_current_bits() const {return nr_current_bits_;}

  virtual int AddCode(int code_idx);
  virtual void DelCode(int nr_bits);

  virtual void Execute() = 0;
  virtual int NrChoices() = 0;
  virtual std::string ShowCode() = 0;

  void Load(long long code, int nr_bits);
 
 protected:
  virtual long long GetBits(int code_idx) = 0;
  virtual int GetNrBits(int code_idx) = 0;
  virtual int ReadBits(long long code) = 0;

  int nr_current_bits_;
  int max_bits_;
  long long code_bits_;
  
  int output_length_;
  long long output_[kMaxOutput];
};

#endif
