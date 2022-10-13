#ifndef GOSIAFITTER_GOSIA_HH
#define GOSIAFITTER_GOSIA_HH

#include <string>
#include <iostream>

extern "C" {
  void gosia_(const char filename[], int &fnlen, const char dir[], int &dirlen, const char outfile[], int &oflen);
}

void RunGosia(std::string filename, std::string dir, std::string outfile);

#endif
