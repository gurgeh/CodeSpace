#include "brute_force.h"

using namespace std;

void BruteForce::Store(){
  vector<long long> v(sm_->get_output(), sm_->get_output() + sm_->get_output_length());
  if(v.size() > max_interest_) v.resize(max_interest_);

  datamap::iterator it = dmap_.find(v);
  if(it == dmap_.end()){
    ProgramStore p;
    p.duplicates_ = 1;
    p.program_length_ = sm_->get_nr_current_bits();
    p.program_ = sm_->get_code();
    dmap_[v] = p;
  } else {
    it->second.duplicates_ += 1;
  }
}

void BruteForce::Search(){
  using namespace boost::posix_time;
  ptime old = second_clock::local_time();

  printf("now!\n");
  RecSearch();
  printf("done!\n");

  time_duration td = second_clock::local_time() - old;

  printf("Depth %d, %lld runs, %lu unique - %s\n", sm_->get_max_bits(), nr_evals_, (unsigned long)dmap_.size(), to_simple_string(td).c_str());

  WriteMap();
}

void BruteForce::RecSearch(){
  if(sm_->Ready()){
    sm_->Execute();
    Store();
    nr_evals_++;
    return;
  }

  for(int i = 0; i < sm_->NrChoices(); i++){
    int len = sm_->AddCode(i);
    if(len){
      RecSearch();
      sm_->DelCode(len);
    }
  }
}

void BruteForce::WriteMap(){
  string fname = (boost::format("data/v01.%|02|.txt") % sm_->get_max_bits()).str();
  
  FILE *f = fopen(fname.c_str(), "wt");
  BOOST_FOREACH(const datamap::value_type& x, dmap_){
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

