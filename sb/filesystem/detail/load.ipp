#ifndef sb_filesystem_load_h
# error "Direct inclusion error."
#endif

/*
   Copyright (c) 2022, Scott Bailey
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
       * Redistributions of source code must retain the above copyright
         notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above copyright
         notice, this list of conditions and the following disclaimer in the
         documentation and/or other materials provided with the distribution.
       * Neither the name of the <organization> nor the
         names of its contributors may be used to endorse or promote products
         derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL SCOTT BAILEY BE LIABLE FOR ANY
   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <ios>
#include <fstream>
#include <iostream>

namespace sb {
namespace filesystem {

namespace { // anonymous namespace

template<typename C, std::ios_base::openmode MODE>
inline result<C> load_data(std::filesystem::path& path) {

  static_assert(sizeof(typename C::value_type) == 1, "Size of container's data should be 1.");

  std::error_code sec;

  /*
  // Test to see if the file exists:
  if(!std::filesystem::exists(path,sec)) {
    return result<C>(2, std::string("File not found: ") + m_path.string());
  }

  // Test to see if the file exists by getting it's size:
  const size_t sz = std::filesystem::sz(path,sec);
  if( sz == static_cast<std::uintmax_t>(-1)) {
    return result<C>(-1, std::string("File error: ") + sec.message() + " " + path.string());
  }
  */

  // Open the file:
  std::ifstream infile(path,MODE);
  if(!infile.is_open()) {
    return result<C>(-1, std::string("Error opening file: ") + path.string());
  }

  // Get file size:
  infile.seekg(0, infile.end);
  const int sz_int = infile.tellg();
  if(sz_int < 0) {
    return result<C>(-1, std::string("Error determining file size: ") + path.string());
  }
  infile.seekg (0, infile.beg);
  const size_t sz = static_cast<size_t>(sz_int);


  // Reserve memory, and load the file.
  C buffer;
  buffer.resize(sz);

  char* data = reinterpret_cast<char*>(buffer.data());
  infile.read(data, sz);

  if(!infile) {
    buffer.resize(infile.gcount());
    return result<C>(-1, "Failure to read all data.", buffer);
  }

  return result<C>(buffer);
}


} // anonymous namespace


template<typename C>
inline result<C> load_text(std::filesystem::path& path) {
  return load_data<C,std::ios::in>(path);
}


template<typename C>
inline result<C> load_bin(std::filesystem::path& path) {
  return load_data<C,std::ios::binary>(path);
}


} // namespace filesystem
} // namespace sb
