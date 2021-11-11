#ifndef tuple_deserialize_ipp
#define tuple_deserialize_ipp



template<std::size_t I = 0, typename... Ts>
inline typename std::enable_if<I == sizeof...(Ts), bool>::type tuple_deserialize(ELEMENT_TAG_T, const void*, std::tuple<Ts...>&) {
   return false; // Indicate we did not deserialize the data.
}

template<std::size_t I = 0, typename... Ts>
inline typename std::enable_if<I < sizeof...(Ts), bool>::type tuple_deserialize(ELEMENT_TAG_T tag, const void* buffer, std::tuple<Ts...>& t) {

   // Get the element, if it's not a match go to the next index.
   auto& element = std::get<I>(t);
   if(element.get_type() != tag)
      return tuple_deserialize<I+1, Ts...>(tag, buffer, t);

   element.deserialize(buffer);
   return true;
}


#endif
