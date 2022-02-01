#ifndef sb_cli_h
#define sb_cli_h

#include <vector>
#include <utility>                    // std::pair
#include <sb/detail/cli_iterator.ipp> // Iterators plus types

namespace sb {

class cli {
public:
  cli() = delete;
  cli(const cli& rhs) = delete;
  cli(const cli&& rhs) = delete;

  cli(int argc, const char* const argv[]);

  ~cli();

  /// Report the number of expanded enties.
  /// @return The number of expanded entries in the CLI, excluding argv[0];
  size_t size() const;
  /// @return true if the CLI is empty (other than argv[0]).
  bool empty() const;

  /// @return The invocation, aka argv[0].
  const std::string& invocation() const;

  /// @param index  Index into the arg list, note that 0 is the first NON invocation argument.
  /// @return Const ref to the requested argument.
  const std::string& operator[](size_t index) const;


  // iterator functionality
  using iterator = detail::cli_iterator;             ///< mutable iterator.
  using const_iterator = detail::const_cli_iterator; ///< const iterator.

  // @return The argument following iter's argument.  example: `-ftr filename` if dereferencing iter would return any of "-f",
  // "-t", or "-r" then follows() will return "filename".
  const std::string& follows(const iterator& iter) const;
  const std::string& follows(const const_iterator& iter) const;

  // Iterators do NOT include invocation/argv[0] and iterate over the expanded data.
  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;

  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept;

private:
  std::string m_invocation;
  detail::args_t m_args;    // These are the arguments, excluding argv[0].
};


} // namespace sb


#include <sb/detail/cli.ipp>

#endif
