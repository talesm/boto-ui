#ifndef BOTO_CORE_TARGETSTATUS_HPP
#define BOTO_CORE_TARGETSTATUS_HPP

#include "util/FlagsBase.hpp"

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

using StatusFlags = FlagsBase<Status>;

inline StatusFlags
operator|(Status lhs, StatusFlags rhs)
{
  return rhs |= lhs;
}

inline StatusFlags
operator&(Status lhs, StatusFlags rhs)
{
  return rhs &= lhs;
}

}

#endif // BOTO_CORE_TARGETSTATUS_HPP
