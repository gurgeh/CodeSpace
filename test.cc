#include <stdio.h>
#include <map>
#include <vector>

#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "stack_machine.h"

const int kMaxDepth = 36;
const int kMaxInterest = 10;

using namespace std;

struct ProgramStore{
  long long program_;
  int program_length_;
  int duplicates_;
};

typedef map<vector<long long>, ProgramStore> datamap;

datamap dm;

/*
Long run

Profiling run

Python-connection

store in prefix tree (truncate?)
  tot with prefix
  shortest program, len, bits

read bits

incl two start bits

optimize program generation - do not generate swap swap, for example.
  Although it could be useful if a jump goes to the second swap, it is so
  rare that we could disallow them in the language.

find interesting sequences and "best operators"

try giving programs input in stack

maybe give them addressable memory and more instructions

compile programs to x86 (GPU?)
  Use floating point?

Randomize longer programs

Try with GA/GP

 */


void store(StackMachine &sm){
  vector<long long> v(sm.get_output(), sm.get_output() + sm.get_output_length());
  if(v.size() > kMaxInterest) v.resize(kMaxInterest);

  datamap::iterator it = dm.find(v);
  if(it == dm.end()){
    ProgramStore p;
    p.duplicates_ = 1;
    p.program_length_ = sm.get_nr_current_bits();
    p.program_ = sm.get_code();
    dm[v] = p;
  } else {
    it->second.duplicates_ += 1;
  }
}

void printOutput(StackMachine &sm){
  for(int i = 0; i < sm.get_output_length(); i++){
    long long out = sm.get_output()[i];
    printf("%lld ", out);
  }
  printf("\n");
}

int search(StackMachine &sm){
  if(sm.Ready()){
    sm.Execute();
    store(sm);
    return 1;
  }

  int evals = 0;
  for(int i = 0; i < sm.NrChoices(); i++){
    int len = sm.AddCode(i);
    if(len){
      evals += search(sm);
      sm.DelCode(len);
    }
  }

  return evals;
}

void writeMap(){
  string fname = (boost::format("Depth%1%.txt") % kMaxDepth).str();
  
  FILE *f = fopen(fname.c_str(), "wt");
  BOOST_FOREACH(const datamap::value_type& x, dm){
    fprintf(f, "%d ", x.second.duplicates_);
    fprintf(f, "%d ", x.second.program_length_);
    fprintf(f, "%lld ", x.second.program_);

    BOOST_FOREACH(long long i, x.first){
      fprintf(f, "%lld ", i);
    }

    fprintf(f, "\n");
  }
  fclose(f);
}

int main(void){
  StackMachine sm(10, 1, 20, 0);

  using namespace boost::posix_time;
  ptime old = second_clock::local_time();
  int evals = 0;
  for(int maxdepth = 0; maxdepth <= kMaxDepth; maxdepth++){
    ptime now = second_clock::local_time();
    time_duration td = now - old;
    printf("Testing depth %d (%d runs, %lu unique) - %s\n", maxdepth, evals, (unsigned long)dm.size(), to_simple_string(td).c_str());
    old = now;

    sm.set_max_bits(maxdepth);
    evals += search(sm);
  }

  printf("%d runs, %lu unique\n", evals, (unsigned long)dm.size());

  writeMap();

  return 0;
}
