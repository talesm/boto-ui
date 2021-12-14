#ifndef BOTOUI_UTIL_COOKIEBASE_HPP
#define BOTOUI_UTIL_COOKIEBASE_HPP

#include <memory>

namespace boto {

/**
 * @brief Generic RAII guard style object
 *
 * @tparam Component The component guarding
 * @tparam CookieReleaser how to unguard
 */
template<class Component, class CookieReleaser>
class CookieBase
{
private:
  std::unique_ptr<Component, CookieReleaser> component;

  friend Component;

  CookieBase(Component* component)
    : component(component)
  {}

public:
  constexpr CookieBase() = default;

  /// True if the cookie still valid
  operator bool() const { return bool(component); }

  /// Invalidate the cookie
  void end() { component.reset(); }
};

} // namespace boto

#endif // BOTOUI_UTIL_COOKIEBASE_HPP
