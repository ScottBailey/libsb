#ifndef tuple_valid_ipp
#define tuple_valid_ipp



template<size_t MAX, size_t I = 0, typename... Ts>
inline typename std::enable_if<I == MAX, bool>::type tuple_valid(const std::tuple<Ts...>&) {
   static_assert( (MAX <= sizeof...(Ts)), "Template argument MAX exceeds tuple size.");
   return true;
}

template<size_t MAX, size_t I = 0, typename... Ts>
inline typename std::enable_if<I < MAX, bool>::type tuple_valid(const std::tuple<Ts...>& t) {
   static_assert( (MAX <= sizeof...(Ts)), "Template argument MAX exceeds tuple size.");
   const auto& element = std::get<I>(t);
   if(!element.is_valid())
      return false;
   return tuple_valid<MAX,I+1>(t);
}


template<size_t MAX, size_t I = 0, typename... Ts>
inline typename std::enable_if<I == MAX, bool>::type tuple_any_valid(const std::tuple<Ts...>&) {
   static_assert( (MAX <= sizeof...(Ts)), "Template argument MAX exceeds tuple size.");
   return false;
}

template<size_t MAX, size_t I = 0, typename... Ts>
inline typename std::enable_if<I < MAX, bool>::type tuple_any_valid(const std::tuple<Ts...>& t) {
   static_assert( (MAX <= sizeof...(Ts)), "Template argument MAX exceeds tuple size.");
   const auto& element = std::get<I>(t);
   if(element.is_valid())
      return true;
   return tuple_any_valid<MAX,I+1>(t);
}


#endif
