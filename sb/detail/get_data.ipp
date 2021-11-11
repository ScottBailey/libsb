#ifndef sb_get_data_h
# error "Direct inclusion error"
#endif


namespace sb {
namespace detail {

// structure with 'value' indicating presence of data() as a member function of class C
template <typename C>
class has_data_method
{
protected:
   template <typename T, typename U>
   static std::true_type test_signature(U (T::*)() const);

   template <typename T>
   static decltype(test_signature(&T::data)) test(std::nullptr_t);

   template <typename T>
   static std::false_type test(...);

public:
   using type = decltype(test<C>(nullptr));
   static const bool value = type::value;
};


// structure with 'value' indicating presence of get() as a member function of class C
template <typename C>
class has_get_method
{
protected:
   template <typename T, typename U>
   static std::true_type test_signature(U (T::*)() const);

   template <typename T>
   static decltype(test_signature(&T::get)) test(std::nullptr_t);

   template <typename T>
   static std::false_type test(...);

public:
   using type = decltype(test<C>(nullptr));
   static const bool value = type::value;
};



// prototype for template specialization
template <bool has_data, bool has_get>
struct get_data;

// specilization: both exist
template <>
struct get_data<true,true>
{
   template<typename T>
   static const void* get(const T& t)
   {
      return t.data();
   }

   template<typename T>
   static void* get(T& t)
   {
      return t.data();
   }
};


// specilization: only data() exists
template <>
struct get_data<true,false>
{
   template<typename T>
   static const void* get(const T& t)
   {
      return t.data();
   }

   template<typename T>
   static void* get(T& t)
   {
      return t.data();
   }
};


// specilization: only get() exists
template <>
struct get_data<false,true>
{
   template<typename T>
   static const void* get(const T& t)
   {
      return t.get();
   }

   template<typename T>
   static void* get(T& t)
   {
      return t.get();
   }
};

} // namespace detail


template<typename T>
auto* get_data(T& t)
{
   constexpr bool has_data_method = detail::has_data_method<T>::value;
   constexpr bool has_get_method = detail::has_get_method<T>::value;
   return detail::get_data<has_data_method,has_get_method>::get(t);
}

template<typename T>
const auto* get_data(const T& t)
{
   constexpr bool has_data_method = detail::has_data_method<T>::value;
   constexpr bool has_get_method = detail::has_get_method<T>::value;
   return detail::get_data<has_data_method,has_get_method>::get(t);
}

} // namespace sb
