#ifndef BOTO_CORE_TARGETSTATUS_HPP
#define BOTO_CORE_TARGETSTATUS_HPP

namespace boto {

/**
 * @brief The current element state
 *
 */
enum TargetStatus : uint16_t
{
  STATUS_NONE = 0,
  STATUS_HOVERED = 1,
  STATUS_GRABBED = 2,
  STATUS_FOCUSED = 4,
  STATUS_INPUTING = 8,
};
}

#endif // BOTO_CORE_TARGETSTATUS_HPP
