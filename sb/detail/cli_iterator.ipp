#ifndef sb_cli_iterator_ipp
#define sb_cli_iterator_ipp

#ifndef sb_cli_h
# error "Direct inclusion error: use `#include <sb/cli.h>` instead."
#endif


// See:
//    http://en.cppreference.com/w/cpp/iterator/iterator
//    http://en.cppreference.com/w/cpp/concept/ForwardIterator

#include <iterator>
#include <algorithm> // std::find


namespace sb {
namespace detail {

using args_t = std::vector<std::string>;

class cli_iterator
  : public std::iterator <std::forward_iterator_tag, std::string> // category and value_type
{
  using parent = std::iterator <std::forward_iterator_tag, std::string>;
  using self_type = cli_iterator;
public:
  explicit cli_iterator(size_t o, args_t& a) :  m_offset{o}, m_args{a} {}
  self_type& operator++() { increment(); return *this;}
  self_type operator++(int) {self_type retval = *this; ++(*this); return retval;}
  self_type& operator--() { decrement(); return *this;}
  self_type operator--(int) {self_type retval = *this; --(*this); return retval;}
  bool operator==(const self_type& other) const {return (m_offset == other.m_offset) && (&m_args == &other.m_args); }
  bool operator!=(const self_type& other) const {return !(*this == other);}
  const std::string& operator*() const { if(m_offset < m_args.size()) return m_args[m_offset]; return m_empty;}
  std::string& operator*() { if(m_offset < m_args.size()) return m_args[m_offset]; return m_empty;}
  self_type operator+(size_t n) {self_type retval = *this; for(size_t i=0; i < n; ++i) { retval.increment(); } return retval;}
  self_type operator-(size_t n) {self_type retval = *this; for(size_t i=0; i < n; ++i) { retval.decrement(); } return retval;}
  const std::string& follows() const {
    size_t offset = follow_index();
    if(offset < m_args.size())
      return m_args[offset];
    return m_empty;
  }
  void skip_follow() {
    size_t temp = follow_index();
    if(temp < m_args.size()) {
      m_skip.push_back(temp);
    }
  }
private:
  size_t follow_index() const {
    for(size_t i = m_offset+1; i < m_args.size(); ++i) {
      // Don't take an empty string as a follow.
      if(!m_args[i].empty() && (m_args[i].front() != '-') && !is_skipped(i)) {
        return i;
      }
    }
    return m_args.size();
  }

  bool is_skipped(size_t n) const {
    return std::find(m_skip.begin(), m_skip.end(), n) != m_skip.end();
  }

  void increment() {
    ++m_offset;
    while(is_skipped(m_offset))
      ++m_offset;
    if(m_offset > m_args.size())
      m_offset = m_args.size();
  }

  void decrement() {
    --m_offset;
    while(is_skipped(m_offset))
      --m_offset;
    if(m_offset > m_args.size())
      m_offset = m_args.size();
  }

  // data
  size_t m_offset;
  args_t& m_args;
  std::vector<size_t> m_skip;
  std::string m_empty;
};



class const_cli_iterator
  : public std::iterator <std::forward_iterator_tag, std::string> // category and value_type
{
  using parent = std::iterator <std::forward_iterator_tag, std::string>;
  using self_type = const_cli_iterator;
public:
  explicit const_cli_iterator(size_t o, const args_t& a) :  m_offset{o}, m_args{a} {}
  self_type& operator++() { increment(); return *this;}
  self_type operator++(int) {self_type retval = *this; ++(*this); return retval;}
  self_type& operator--() { decrement(); return *this;}
  self_type operator--(int) {self_type retval = *this; --(*this); return retval;}
  bool operator==(const self_type& other) const {return (m_offset == other.m_offset) && (&m_args == &other.m_args); }
  bool operator!=(const self_type& other) const {return !(*this == other);}
  const std::string& operator*() const { if(m_offset < m_args.size()) return m_args[m_offset]; return m_empty;}
  //std::string& operator*() { if(m_offset < m_args.size()) return m_args[m_offset]; return m_empty;}
  self_type operator+(size_t n) {self_type retval = *this; for(size_t i=0; i < n; ++i) { retval.increment(); } return retval;}
  self_type operator-(size_t n) {self_type retval = *this; for(size_t i=0; i < n; ++i) { retval.decrement(); } return retval;}
  const std::string& follows() const {
    size_t offset = follow_index();
    if(offset < m_args.size())
      return m_args[offset];
    return m_empty;
  }
  void skip_follow() {
    size_t temp = follow_index();
    if(temp < m_args.size()) {
      m_skip.push_back(temp);
    }
  }
private:
  size_t follow_index() const {
    for(size_t i = m_offset+1; i < m_args.size(); ++i) {
      // Don't take an empty string as a follow.
      if(!m_args[i].empty() && (m_args[i].front() != '-') && !is_skipped(i)) {
        return i;
      }
    }
    return m_args.size();
  }

  bool is_skipped(size_t s) const {
    return std::find(m_skip.begin(), m_skip.end(), s) != m_skip.end();
  }

  void increment() {
    ++m_offset;
    while(is_skipped(m_offset))
      ++m_offset;
    if(m_offset > m_args.size())
      m_offset = m_args.size();
  }

  void decrement() {
    --m_offset;
    while(is_skipped(m_offset))
      --m_offset;
    if(m_offset > m_args.size())
      m_offset = m_args.size();
  }

  // data
  size_t m_offset;
  const args_t& m_args;
  std::vector<size_t> m_skip;
  std::string m_empty;
};

} // namespace detail
} // namespace sb


#endif
