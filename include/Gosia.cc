#include "Gosia.hh"

void RunGosia(std::string filename, std::string dir, std::string outfile) {  
  gosia_(filename.c_str(), filename.size(), dir.c_str(), dir.size(), outfile.c_str(), outfile.size());
}
