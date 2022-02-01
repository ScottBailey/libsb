#ifndef sb_cli_h
# error "Direct inclusion error."
#endif

namespace sb {

//--- constructors/destructor --------------------------------------------------------------------------------

cli::cli(int argc, const char* const argv[]) {

  if(argc < 1)
    return;

  for(size_t i=1; i < static_cast<size_t>(argc); ++i) {

    std::string s = argv[i];

    // 2 char string, any string NOT starting with '-', or any string starting with "--"
    if((s.size() < 3) || (s[0] != '-') || (s[1] == '-'))  {
      m_args.push_back(s);
      continue;
    }

    // Now we have a situations where we have at minimum "-xy"
    for(size_t j=1; j < s.size(); ++j) {
      // For each single letter argument, push a dash followed by the char.
      std::string temp = "-";
      temp.push_back(s[j]);
      m_args.push_back(temp);
    }
  }

}


cli::~cli() = default;


//--- alphabetic ------------------------------------------------------------------------------------------

cli::iterator cli::begin() noexcept {
  return cli::iterator(0, m_args);
}


cli::const_iterator cli::begin() const noexcept {
  return cli::const_iterator(0, m_args);
}


cli::const_iterator cli::cbegin() const noexcept {
  return cli::const_iterator(0, m_args);
}


cli::const_iterator cli::cend() const noexcept {
    return cli::const_iterator(m_args.size(), m_args);
}


bool cli::empty() const {
  return m_args.empty();
}


cli::iterator cli::end() noexcept {
  return cli::iterator(m_args.size(), m_args);
}


cli::const_iterator cli::end() const noexcept {
  return cli::const_iterator(m_args.size(), m_args);
}


const std::string& cli::follows(const iterator& iter) const {
  return iter.follows();
}


const std::string& cli::follows(const const_iterator& iter) const {
  return iter.follows();
}


const std::string& cli::invocation() const {
  return m_invocation;
}


size_t cli::size() const {
  return m_args.size();
}


} // namespace sb
