#ifndef sb_thread_auto_thread_h
#define sb_thread_auto_thread_h

/*
   Copyright (c) 2013-2017, Scott Bailey
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

#include <boost/thread/thread.hpp>
#include <memory> // std::unique_ptr
#include <string>

namespace sb {
namespace thread {

class auto_thread
{
public:
   /// constructor
   auto_thread();
   /// destructor
   virtual ~auto_thread();

   /// start the thread
   virtual void start();
   /// signal the thread to cancel
   virtual void cancel();

   /// wait for the thread to return
   /// @warning NOT threadsafe, only one thread may safely call join at a time
   virtual void join();
   /// try to join a thread, return immideately regardless
   /// @note The #define SB_THREAD_AUTOTHREAD_TRY_JOIN_MINIMUM_SLEEP_US modifies this functions behavior
   /// @warning NOT threadsafe, only one thread may safely call join at a time
   /// @return true if joined, false if thread is still running
   virtual bool try_join();
   /// try to join a thread, wait no more than a specified time before returning
   /// @warning NOT threadsafe, only one thread may safely call join at a time
   /// @param maximum_wait  Maximum time to wait for a return
   /// @return true if joined, false if thread is still running
   template<typename time_duration_type>
   bool try_join_for(const time_duration_type& maximum_wait);

   /// sets the thread name for display in debuggers and process lists
   /// @param name The name to dispaly
   /// @note This is NOT a threadsafe call
   /// @note does not work for MINGW, only windows and linux
   void set_name(const std::string& name);


protected:
   /// overload for the thread!
   virtual void loop()=0;

public:
   void operator()();     ///< function call operator, must be called only by internal implementation

private:
   std::unique_ptr<boost::thread> m_thread;
   std::string m_name;
};

} // namespace thread
} // namespace sb


#include <sb/thread/detail/auto_thread.ipp>
#endif
