#pragma once

/**
 * @defgroup elements Elements
 *
 * Elements are widgets that can be added to a target.
 *
 * Example:
 * ```
 * boto::window(frame, "Window Name", {100, 200});
 * ```
 *
 * This adds an window to `frame` at given position.
 *
 * All elements must be placed on target, which is always the first parameter. A
 * target is a Frame or a Group. @ref groups themselves are also elements, so
 * there will always be a frame as root of this tree.
 *
 * With exception of static elements such as element(), box() and label(), all
 * elements must have unique ids in the context of its current. For example:
 *
 * ```
 * boto::button(f, "id1"); // Ok
 * boto::button(f, "id2"); // Ok
 * boto::button(f, "id1"); // BAD
 *
 * auto p = boto::panel(f, "panel1");
 * boto::button(p, "id1"); // Ok, as it is in a different target.
 * ```
 */
