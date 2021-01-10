#pragma once

/**
 * @defgroup groups Groups
 *
 * A group is an special kind of element that acts as container of other
 * elements. Any function that define a group returns an object so you can use
 * it as target to another element.
 *
 * This object will finalize the group when go out of scope. If you need to end
 * it earlier you call its end() method and the object will be invalid
 * afterwards.
 *
 */
