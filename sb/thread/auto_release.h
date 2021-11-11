#ifndef sb_thread_auto_release_h
#define sb_thread_auto_release_h

/*
   Copyright (c) 2020, Scott Bailey
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

#include <boost/thread.hpp>
#include <sb/thread/auto_thread.h>
#include <boost/version.hpp>

#if BOOST_VERSION >= 107500
# include <boost/container/devector.hpp>
#else
# include <deque>
#endif


namespace sb {
namespace thread {

/// T should generally be a std::unique_ptr<sb::auto_thread>
template<typename T>
class auto_release final
   : public sb::thread::auto_thread
{
public:
   /// salls start() to start this thread
   auto_release();
   /// destructor, blocks until m_list is empty and cancel and join of this thread is complete
   virtual ~auto_release() final;

   /// perform a std::move of t into the release queue and perform t->cancel()
   /// @param t  Pointer to the object to cancel and store
   void add(T& t);

   /// @return size() == 0
   bool empty() const;

   /// @return size of the release queue
   size_t size() const;


protected:
   virtual void loop() override final;

private:
   mutable boost::mutex m_mutex;

#if BOOST_VERSION >= 107500
   using list_t = boost::container::devector<T>;
#else
   using list_t = std::deque<T>;
#endif
   list_t m_list;
};

} // namespace thread
} // namespace sb


#include <sb/thread/detail/auto_release.ipp>

#endif
