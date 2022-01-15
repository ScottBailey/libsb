#include <iostream>
#include <vector>
#include <sb/filesystem/load.h>
#include <sb/filesystem/executable_path.h>

int main(int argc, char** argv) {

  if(argc != 2) {
    std::cerr << sb::filesystem::executable_path().filename() << " needs the path to a text file to open." << std::endl;
    return -1;
  }

  std::filesystem::path path{argv[1]};

  auto result = sb::filesystem::load_text<std::vector<char>>(path);
  if(!result) {
    std::cout << "Error loading file: " << path << " err: " << result.message() << std::endl;
    return -1;
  }

  auto vec = result.get();

  for(auto a : vec) {
    std::cout << a;
  }

  return 0;
}
