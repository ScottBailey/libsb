#include <iostream>
#include <sb/cli.h>

int main(int argc,char** argv) {

  sb::cli cli(argc, argv);

  for(auto i = cli.begin(); i != cli.end(); ++i) {
    if((*i == "-f") || (*i == "--filename")) {
      std::cout << "arg: " << *i << " follows: " << i.follows() << std::endl;
      i.skip_follow();
      continue;
    }
    std::cout << "arg: " << *i << std::endl;
  }

  return 0;
}
