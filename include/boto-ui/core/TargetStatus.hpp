#ifndef BOTO_CORE_TARGETSTATUS_HPP
#define BOTO_CORE_TARGETSTATUS_HPP

namespace boto {

/**
 * @brief The current element state
 *
 */
enum class Status : uint16_t
{
  NONE = 0,
  HOVERED = 1,
  GRABBED = 2,
  FOCUSED = 4,
  INPUTING = 8,
};

class StatusFlags
{
  uint16_t flags;

public:
  constexpr StatusFlags(Status flags = Status::NONE)
    : flags(uint16_t(flags))
  {}

  constexpr bool operator==(StatusFlags rhs) const
  {
    return flags == rhs.flags;
  }

  constexpr bool test(StatusFlags rhs) const { return flags & rhs.flags; }

  StatusFlags& set(StatusFlags rhs) { return operator|=(rhs); }
  StatusFlags& reset(StatusFlags rhs) { return operator&=(~rhs); }
  StatusFlags& operator~()
  {
    flags = ~flags;
    return *this;
  }
  StatusFlags& operator|=(StatusFlags rhs)
  {
    flags |= rhs.flags;
    return *this;
  }
  StatusFlags& operator&=(StatusFlags rhs)
  {
    flags &= rhs.flags;
    return *this;
  }

  constexpr operator bool() const { return bool(flags); }
};

inline StatusFlags
operator|(StatusFlags lhs, StatusFlags rhs)
{
  return lhs |= rhs;
}
inline StatusFlags
operator|(StatusFlags lhs, Status rhs)
{
  return lhs |= rhs;
}

inline StatusFlags
operator&(StatusFlags lhs, StatusFlags rhs)
{
  return lhs &= (rhs);
}

inline StatusFlags
operator&(StatusFlags lhs, Status rhs)
{
  return lhs &= rhs;
}

}

#endif // BOTO_CORE_TARGETSTATUS_HPP
