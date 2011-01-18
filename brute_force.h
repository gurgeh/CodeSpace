#ifndef BRUTE_FORCE_H
#define BRUTE_FORCE_H

#include <map>
#include <vector>

#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "machine.h"

struct ProgramStore{
  long long program_;
  int program_length_;
  int duplicates_;
};

typedef std::map<std::vector<long long>, ProgramStore> datamap;

class BruteForce{
 public:
  BruteForce(Machine *sm, int max_interest=10)
    : sm_(sm),
      max_interest_(max_interest),
      nr_evals_(0){
  }
  
  void Search();

 private:
  long long nr_evals_;
  int max_interest_;

  datamap dmap_;
  Machine *sm_;

  void RecSearch();
  void Store();
  void WriteMap();
};

#endif
