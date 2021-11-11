#ifndef sb_thread_detail_auto_thread_h
#define sb_thread_detail_auto_thread_h

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


#ifndef sb_thread_auto_thread_h
# error "You can't call this directly!"
#endif

#include <chrono>

#if defined( _WIN32 )
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <windows.h>
#else
# include <sys/prctl.h>
#endif


#ifdef _WIN32
# pragma pack(push,8)
// see http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
struct THREADNAME_INFO
{
   DWORD dwType; // Must be 0x1000.
   LPCSTR szName; // Pointer to name (in user addr space).
   DWORD dwThreadID; // Thread ID (-1=caller thread).
   DWORD dwFlags; // Reserved for future use, must be zero.
};
# pragma pack(pop)
#endif



namespace sb {
namespace thread {

//--- constructor(s)/destructor ------------------------------------------------------------------------------------------

inline auto_thread::auto_thread()
   : m_thread(nullptr)
{
}


inline auto_thread::~auto_thread()
{
   if( !m_thread)
      return;

   if( m_thread->joinable() )
   {
      m_thread->interrupt();
      m_thread->join();
   }
   m_thread.reset();
}

//--- operators ----------------------------------------------------------------------------------------------------------

inline void auto_thread::operator()()
{
   // this calls loop until it exits or thread interrupted is thrown
   try
   {
      if( !m_name.empty() )
         set_name(m_name);
      this->loop();
   }
   catch(boost::thread_interrupted&)
   {
   }
}

//--- alphabetic ---------------------------------------------------------------------------------------------------------

inline void auto_thread::cancel()
{
   if( m_thread )
      m_thread->interrupt();
}


inline void auto_thread::join()
{
   if( !m_thread )
      return;

   m_thread->join();
   m_thread.reset();
}


inline void auto_thread::set_name(const std::string& name)
{
   m_name = name;
   // for now, only the executing process can change it's name
   if( !m_thread || (m_thread->get_id() != boost::this_thread::get_id()) )
      return;

#if defined( _MSC_VER )
   // copy the name to a c string
   const size_t MAX(255);
   char threadName[MAX];
   strncpy(threadName, name.c_str(),MAX);
   threadName[MAX-1]=0;  // ensure null termination

   const DWORD MS_VC_EXCEPTION=0x406D1388;

   THREADNAME_INFO info;
   info.dwType = 0x1000;
   info.szName = threadName;
   info.dwThreadID = DWORD(-1); // the executing thread
   info.dwFlags = 0;

   __try
   {
      RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
   }
   __except(EXCEPTION_EXECUTE_HANDLER)
   {
   }
#elif defined( _WIN32 )  //mingw
   // need to find a way to do this...
#else
   prctl( PR_SET_NAME, name.c_str(), 0, 0, 0);
#endif
}


inline void auto_thread::start()
{
   if( !m_thread )
      m_thread = std::make_unique<boost::thread>(std::ref(*this));
}


template<typename time_duration_type>
inline bool auto_thread::try_join_for(const time_duration_type& maximum_wait)
{
   if( !m_thread )
      return true;

   if( !m_thread->try_join_for(maximum_wait) )
      return false;

   m_thread.reset();
   return true;
}


inline bool auto_thread::try_join()
{
   // this might need to be larger...
#if defined SB_THREAD_AUTOTHREAD_TRY_JOIN_MINIMUM_SLEEP_US
   return try_join_for(boost::chrono::microseconds(SB_THREAD_AUTOTHREAD_TRY_JOIN_MINIMUM_SLEEP_US));
#else
   return try_join_for(boost::chrono::microseconds(1));
#endif
}


} // namespace thread
} // namespace sb

#endif
