TODO
====

Simplify! (0.5)
---------------

The goal is review and reorganize the code and make it more consistent with its abstractions

- [x] Rename project
- [x] Add unit test configuration
- [x] Unify DisplayList::Command and Shape into DisplayItem and DisplayAction
- [x] Refactor DisplayList so it delegates the rendering to a visitor
- [x] Wrap push and pop clip into a Proper guard type;
- [x] Move event related stuff to new core class EventDispatcher;
- [ ] ~~When frame is created it sets a thread_local where the current target resides~~
- [ ] Create Theme class, that stores all styles
- [ ] Move nextPos, nextSize and etc to the targetStack
- [ ] Move styling utils to targetStack (nextStyle, nextColor, etc)
- [ ] Simplify target to use a functor to decide how to advance
- [ ] Refactor Style hierarchy with 4 fundamental Style concepts (Box, Text, Element and Control) and the respective render* functions to them
- [x] Move current main back to a branch;
- [x] Group source files into subdirectories;

Wishlist
--------

- [ ] Dynamic Styles
- [ ] Persistent elements (conventional UI)
- [ ] selectable
- [ ] listBox
- [ ] modal
- [ ] dropdown
- [ ] comboBox
- [ ] menus
- [ ] treeNode
- [ ] dialog
- [ ] messageBox
- [ ] fileDialog
- [ ] colorInput
- [ ] colorDialog
- [ ] Allow some sort of cache on State
- [ ] textArea;
- [ ] generic numberField;
- [ ] Sized Buttons;
- [ ] Test for numberFields and boxes
- [ ] Test for sliders
- [ ] Allow using the SDL_gfx font
- [ ] TTF Fonts
- [ ] section
- [ ] checkBox
- [ ] radioBox
- [ ] vector numeric input
- [ ] Keyboard navigation
- [ ] Joystick navigation
- [ ] Explicit activation
- [ ] graphs
- [ ] drag & drop
- [ ] multiple mouse buttons
