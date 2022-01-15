#ifndef sb_result_h
# error "Direct inclusion error."
#endif

/*
   Copyright (c) 2022 Scott Bailey
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


namespace sb {

//--- constructors/destructor ------------------------------------------------------------------------------------------

template<typename T>
inline result<T>::result()
  : m_err{-1}
  , m_message{"Uninitialized"}
{
}


template<typename T>
inline result<T>::result(const T& t)
  : m_err{0}
  , m_value{t}
{
}


template<typename T>
inline result<T>::result(int err, const std::string& message)
  : m_err{err}
  , m_message{message}
{
}


template<typename T>
inline result<T>::result(int err, const std::string& message, const T& t)
  : m_err{err}
  , m_message{message}
  , m_value{t}
{
}


//--- operators --------------------------------------------------------------------------------------------------------

template<typename T>
inline result<T>::operator unspecified_bool_type() const {
   return (m_err != 0 ? 0 : unspecified_bool_true);
}


template<typename T>
inline bool result<T>::operator!() const {
   return m_err != 0;
}


//--- alphabetic--------------------------------------------------------------------------------------------------------

template<typename T>
inline int result<T>::error() const {
  return m_err;
}


template<typename T>
inline T result<T>::get() const {
  return m_value;
}


template<typename T>
inline bool result<T>::is_good() {
  return !m_err;
}


template<typename T>
inline std::string result<T>::message() const {
  return m_message;
}


template<typename T>
inline void result<T>::unspecified_bool_true() {
  ;
}


}
