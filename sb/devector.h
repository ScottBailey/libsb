#ifndef sb_devector_h
#define sb_devector_h

#include <boost/version.hpp>
#if BOOST_VERSION >= 107500
# include <boost/container/devector.hpp>
#else
# include <deque>
#endif


namespace sb {

#if defined(BOOST_CONTAINER_DEVECTOR_HPP)
template<typename T>
using devector = boost::container::devector<T>;
#else
template<typename T>
using devector = std::deque<T>;
#endif

} // namespace sb


#endif
