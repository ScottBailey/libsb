#ifndef tuple_print_ipp
#define tuple_print_ipp



template<std::size_t I = 0, typename... Ts>
inline typename std::enable_if<I == sizeof...(Ts), void>::type tuple_print(std::ostream&, const std::tuple<Ts...>&) {
   // Empty, so do nothing!
}

template<std::size_t I = 0, typename... Ts>
inline typename std::enable_if<I < sizeof...(Ts), void>::type tuple_print(std::ostream& os, const std::tuple<Ts...>& t) {
   const auto& element = std::get<I>(t);
   if(element.is_valid()) {
      os << element.get();
   }
   else if(element.is_dismissed()) {
      os << "dismissed";
   }
   else {
      os << "invalid";
   }
   os << ' ';
   tuple_print<I+1, Ts...>(os,t);
}


#endif
