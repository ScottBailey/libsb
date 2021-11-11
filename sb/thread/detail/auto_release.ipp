#ifndef sb_thread_auto_release_h
# error "Direct inclusion error"
#endif

namespace sb {
namespace thread {

//--- constructor / destructors ------------------------------------------------------------------------------------------

template<typename T>
inline auto_release<T>::auto_release()
{
   set_name("auto_release");
   start();
}


template<typename T>
auto_release<T>::~auto_release()
{
   cancel();
   join();
}


//--- alphabetic ---------------------------------------------------------------------------------------------------------

template<typename T>
void auto_release<T>::add(T& t)
{
   if(!t)
      return;
   boost::unique_lock<boost::mutex> lk{m_mutex};
   t->cancel();
   m_list.emplace_back(std::move(t));
}


template<typename T>
bool auto_release<T>::empty() const
{
   boost::unique_lock<boost::mutex> lk{m_mutex};
   return m_list.empty();
}


template<typename T>
void auto_release<T>::loop()
{
   try
   {
      boost::chrono::milliseconds sleep_duration{100};
      for(;;)
      {
         // sleep before locking
         boost::this_thread::sleep_for(sleep_duration);

         // lock the mutex and then iterate over the list joining and reseting as we go
         boost::unique_lock<boost::mutex> lk{m_mutex};
         for(typename list_t::iterator i=m_list.begin(); i != m_list.end(); ) // increment inside loop
         {
            // if we can't join it, go on to the next one
            if(!(*i)->try_join())
               ++i;
            else
            {
               // we were able to join, so delete the memory from the smart pointer and remove it from the list
               i->reset();
               i = m_list.erase(i);
            }
         }

         // set sleep time longer if the list is empty
         if(m_list.empty())
            sleep_duration = boost::chrono::milliseconds{300};
         else
            sleep_duration = boost::chrono::milliseconds{100};
      }
   }
   catch(boost::thread_interrupted&)
   {
      // time to exit, go for a hard join and then a clear all
      boost::unique_lock<boost::mutex> lk{m_mutex};
      for(T& t:m_list)
         t->join();
      m_list.clear(); // smartpointers self reset
   }
}


template<typename T>
size_t auto_release<T>::size() const
{
   boost::unique_lock<boost::mutex> lk{m_mutex};
   return m_list.size();
}


} // namespace thread
} // namespace sb
