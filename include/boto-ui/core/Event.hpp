#ifndef BOTO_CORE_EVENT_HPP
#define BOTO_CORE_EVENT_HPP

namespace boto {

/**
 * @brief The event the element is receiving
 *
 */
enum class Event : uint16_t
{
  NONE,
  GRAB,
  ACTION,
  CANCEL,
  FOCUS_GAINED,
  FOCUS_LOST,
  END_LINE,
  SPACE,
  BACKSPACE,
  INPUT,
};

/**
 * @brief The events an element will accept
 *
 */
enum class RequestEvent
{
  NONE,
  HOVER,
  GRAB,
  FOCUS,
  INPUT,
};

} // namespace boto

#endif // BOTO_CORE_EVENT_HPP
