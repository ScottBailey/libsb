#ifndef sb_message_pointer_iterator_ipp
#define sb_message_pointer_iterator_ipp

// See:
//
//    http://en.cppreference.com/w/cpp/iterator/iterator
//    http://en.cppreference.com/w/cpp/concept/ForwardIterator

#include <iterator>

namespace sb {
namespace message {
namespace detail {

template<typename T>
class iterator
   : public std::iterator <std::forward_iterator_tag, T>           // category and value_type
{
   using parent = std::iterator <std::forward_iterator_tag, T>;
   using self_type = iterator<T>;
public:
   explicit iterator(T* _p) : p(_p) {}
   iterator& operator++() { ++p; return *this;}
   iterator operator++(int) {iterator retval = *this; ++(*this); return retval;}
   iterator& operator--() { --p; return *this;}
   iterator operator--(int) {iterator retval = *this; --(*this); return retval;}
   bool operator==(const iterator& other) const {return p == other.p;}
   bool operator!=(const iterator& other) const {return !(*this == other);}
   const T& operator*() const {return *p;}
   T& operator*() {return *p;}
   self_type operator+(size_t n) {self_type retval = *this; retval.p += n; return retval;}
   self_type operator-(size_t n) {self_type retval = *this; retval.p -= n; return retval;}
private:
   T* p;

};


template<typename T>
class const_iterator
   : public std::iterator <std::forward_iterator_tag, T>           // category and value_type
{
   using parent = std::iterator <std::forward_iterator_tag, T>;
   using self_type = const_iterator<T>;
public:
   explicit const_iterator(T* _p) : p(_p) {}
   const_iterator& operator++() { ++p; return *this;}
   const_iterator operator++(int) {const_iterator retval = *this; ++(*this); return retval;}
   const_iterator& operator--() { --p; return *this;}
   const_iterator operator--(int) {const_iterator retval = *this; --(*this); return retval;}
   bool operator==(const const_iterator& other) const {return p == other.p;}
   bool operator!=(const const_iterator& other) const {return !(*this == other);}
   const T& operator*() const {return *p;}
   T& operator*() {return *p;}
   self_type operator+(size_t n) {self_type retval = *this; retval.p += n; return retval;}
   self_type operator-(size_t n) {self_type retval = *this; retval.p -= n; return retval;}
private:
   T* p;

};

} // namespace detail

} // namespace message
} // namespace sb


#endif
