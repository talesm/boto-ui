#ifndef BOTO_UTIL_FLAGSBASE_HPP
#define BOTO_UTIL_FLAGSBASE_HPP

#include <type_traits>

namespace boto {

// Aux class for Flags
template<class FLAGS_ENUM>
class FlagsBase
{
  using UnderlineType = std::underlying_type_t<FLAGS_ENUM>;
  UnderlineType flags;

public:
  constexpr FlagsBase(FLAGS_ENUM flags = {})
    : flags(UnderlineType(flags))
  {}

  constexpr bool operator==(FlagsBase rhs) const { return flags == rhs.flags; }

  constexpr bool test(FlagsBase rhs) const { return flags & rhs.flags; }

  FlagsBase& set(FlagsBase rhs) { return operator|=(rhs); }
  FlagsBase& reset(FlagsBase rhs) { return operator&=(~rhs); }
  FlagsBase& operator~()
  {
    flags = ~flags;
    return *this;
  }
  FlagsBase& operator|=(FlagsBase rhs)
  {
    flags |= rhs.flags;
    return *this;
  }
  FlagsBase& operator&=(FlagsBase rhs)
  {
    flags &= rhs.flags;
    return *this;
  }

  constexpr operator bool() const { return bool(flags); }

  inline FlagsBase operator|(FlagsBase rhs) const { return rhs |= *this; }

  inline FlagsBase operator|(FLAGS_ENUM rhs) const
  {
    return *this | FlagsBase{rhs};
  }

  inline FlagsBase operator&(FlagsBase rhs) const { return rhs &= *this; }

  inline FlagsBase operator&(FLAGS_ENUM rhs) const
  {
    return *this & FlagsBase{rhs};
  }
};

} // namespace boto

#endif // BOTO_UTIL_FLAGSBASE_HPP
