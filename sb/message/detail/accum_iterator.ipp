#ifndef sb_message_detail_accum_iterator_ipp
#define sb_message_detail_accum_iterator_ipp

#include <sb/message/detail/pointer_iterator.ipp>

// See:
//
//    http://en.cppreference.com/w/cpp/iterator/iterator
//    http://en.cppreference.com/w/cpp/concept/ForwardIterator

namespace sb {
namespace message {

// there should probably be a const_iterator, a normal iterator, and a detail iterator (with only protected functions) that const
// and normal call

namespace detail {

template<typename T>
class accum_iterator
   : public std::iterator <std::forward_iterator_tag, typename T::type>           // category and value_type
{
   using parent = std::iterator <std::forward_iterator_tag, typename T::type>;
   using type = typename T::type;
   friend T;

public:
   explicit accum_iterator(T& _t, size_t _i, type* _p) : t{_t}, i{_i}, p{_p} {}
   accum_iterator& operator++() { t.increment(*this); return *this;}
   accum_iterator operator++(int) {accum_iterator retval = *this; ++(*this); return retval;}
   accum_iterator& operator--() { t.decrement(*this); return *this;}
   accum_iterator operator--(int) {accum_iterator retval = *this; ++(*this); return retval;}
   bool operator==(const accum_iterator& other) const {return p == other.p;}
   bool operator!=(const accum_iterator& other) const {return !(*this == other);}
   const type& operator*() const {return *p;}
   type& operator*() {return *p;}

private:
   T& t;                        ///< ref to the accumulator
   size_t i;                    ///< internal index for acumulator's use (it's the index into m_list)
   type* p;                     ///< pointer to the data
};


template<typename T>
class const_accum_iterator
   : public std::iterator <std::forward_iterator_tag, typename T::type>           // category and value_type
{
   using parent = std::iterator <std::forward_iterator_tag, typename T::type>;
   using type = typename T::type;
   friend T;

public:
   explicit const_accum_iterator(const T& _t, size_t _i, const type* _p) : t{_t}, i{_i}, p{_p} {}
   const_accum_iterator& operator++() { t.increment(*this); return *this;}
   const_accum_iterator operator++(int) {const_accum_iterator retval = *this; ++(*this); return retval;}
   const_accum_iterator& operator--() { t.decrement(*this); return *this;}
   const_accum_iterator operator--(int) {const_accum_iterator retval = *this; ++(*this); return retval;}
   bool operator==(const const_accum_iterator& other) const {return p == other.p;}
   bool operator!=(const const_accum_iterator& other) const {return !(*this == other);}
   const type& operator*() const {return *p;}

private:
   const T& t;
   size_t i;
   const type* p;

};

} // namespace detail

} // namespace message
} // namespace sb


#endif
