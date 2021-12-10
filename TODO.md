TODO
====

Simplify! (0.5)
---------------

The goal is review and reorganize the code and make it more consistent with its abstractions

- [ ] Rename project
- [ ] Add unit test configuration
- [ ] Unify DisplayList::Command and Shape into DisplayItem and DisplayCommand
  - [ ] Move to its own file
  - [ ] Instead of SHAPE, we have the options SET_CLIP, COLOR_RECT, TEX_RECT
  - [ ] Resolve PUSH and POP while still receiving commands
- [ ] Refactor DisplayList so it delegates the rendering to a visitor
- [ ] Wrap push and pop clip into a Proper guard type
- [ ] When frame is created it sets a thread_local where the current target resides. 
  - [ ] All new targets are going to be stacked there;
  - [ ] The group targets receive cookies that allow popping them
  - [ ] Remove Target parameter from all elements, use stack instead
  - [ ] This stack has two methods: element() and group(), that are related, as they wrap the interaction with Parent, with events and with the State.
- [ ] Move nextPos, nextSize and etc to the targetStack
- [ ] Move styling utils to targetStack (nextStyle, nextColor, etc)
- [ ] Simplify target to use a functor to decide how to advance
- [ ] Refactor Style hierarchy with 4 fundamental Style concepts (Box, Text, Element and Control) and the respectives render* functions to them
  - [ ] Add basic state awareness on the get/set: HOVER, GRABBING, FOCUSING.
- [ ] Move current main back to a branch
- [ ] Except for frame, remove ability of manual ending(), forcing RAII. No more necessary to allow moving them too.
- [ ] Dynamic Styles (stretch)

Wishlist
--------

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
