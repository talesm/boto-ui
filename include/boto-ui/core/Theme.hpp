#ifndef BOTO_CORE_THEME_HPP_
#define BOTO_CORE_THEME_HPP_

#include <any>
#include <map>
#include <stdexcept>
#include <typeindex>
#include <typeinfo>

namespace boto {

struct SteelBlue;

#ifndef BOTO_THEME
#define BOTO_THEME boto::SteelBlue
#endif

template<class THEME>
class ThemeT;

// Default style
template<class THEME, class T>
struct StyleFor
{
  static int get(const ThemeT<THEME>&)
  {
    throw std::out_of_range(std::string("Style not found: ") +
                            typeid(T*).name());
  }
};

template<class THEME, class T>
struct StyleTypeT
{
  using type = std::decay_t<decltype(
    StyleFor<THEME, T>::get(std::declval<ThemeT<THEME>&>()))>;
};

template<class THEME, class T>
using StyleType = typename StyleTypeT<THEME, T>::type;

template<class THEME>
class ThemeT
{
public:
  template<class T>
  using Tag = T*;

  template<class T>
  const auto& of()
  {
    auto it = styles.find(tag<T>());
    if (it == styles.end()) {
      it = styles.emplace(tag<T>(), StyleFor<THEME, T>::get(*this)).first;
    }
    return std::any_cast<const StyleType<THEME, T>&>(it->second);
  }

  template<class T, class U>
  void set(U&& style)
  {
    static_assert(std::is_convertible_v<U, StyleType<THEME, T>>);
    styles.insert_or_assign(tag<T>(), std::forward<U>(style));
  }

private:
  std::map<std::type_index, std::any> styles;

  template<class T>
  static constexpr std::type_index tag()
  {
    return typeid(T*);
  }
};

template<class T>
struct Hovered;

template<class THEME, class T>
struct StyleFor<THEME, Hovered<T>>
{
  static StyleType<THEME, T> get(ThemeT<THEME>& theme)
  {
    return theme.template of<T>();
  }
};

template<class T>
struct Grabbed;

template<class THEME, class T>
struct StyleFor<THEME, Grabbed<T>>
{
  static StyleType<THEME, T> get(ThemeT<THEME>& theme)
  {
    return theme.template of<T>();
  }
};

template<class THEME, class T>
struct StyleFor<THEME, Grabbed<Hovered<T>>>
{
  static StyleType<THEME, Hovered<Grabbed<T>>> get(ThemeT<THEME>& theme)
  {
    return theme.template of<Hovered<Grabbed<T>>>();
  }
};

template<class T>
struct Focused;

template<class THEME, class T>
struct StyleFor<THEME, Focused<T>>
{
  static StyleType<THEME, T> get(ThemeT<THEME>& theme)
  {
    return theme.template of<T>();
  }
};

template<class THEME, class T>
struct StyleFor<THEME, Focused<Hovered<T>>>
{
  static StyleType<THEME, Hovered<Focused<T>>> get(ThemeT<THEME>& theme)
  {
    return theme.template of<Hovered<Focused<T>>>();
  }
};

template<class THEME, class T>
struct StyleFor<THEME, Focused<Grabbed<T>>>
{
  static StyleType<THEME, Grabbed<Focused<T>>> get(ThemeT<THEME>& theme)
  {
    return theme.template of<Grabbed<Focused<T>>>();
  }
};

using Theme = ThemeT<BOTO_THEME>;

} // namespace boto

#endif // BOTO_CORE_THEME_HPP_
