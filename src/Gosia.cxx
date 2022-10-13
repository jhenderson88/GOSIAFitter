#include "Gosia.h"

void RunGosia(std::string filename, std::string dir, std::string outfile) {
  int fnlen = (int)filename.size();
  int dirlen =  (int)dir.size();
  int oflen = (int)outfile.size();
  //system(("cd "+dir+"; ./gosia < "+filename+" > "+outfile).c_str());
  gosia_(filename.c_str(), fnlen, dir.c_str(), dirlen, outfile.c_str(), oflen);
}
