#ifndef BOTO_CORE_COMMAND_HPP
#define BOTO_CORE_COMMAND_HPP

namespace boto {

/// Comands (comonly keys, but also shortcuts and others)
enum class Command
{
  NONE,
  ACTION,
  ENTER,
  SPACE,
  ESCAPE,
};
} // namespace boto

#endif // BOTO_CORE_COMMAND_HPP
