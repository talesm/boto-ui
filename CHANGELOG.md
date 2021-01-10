DUI CHANGELOG
=============

Version 0.3 - scRollers
-----------------------

- Groups now call end() on destruction, if needed;
- Fix Bug on Wrapper move operations;
  - It was accessing invalid memory
- Organize source files into include;
- sliderBox();
- Renamed slider() to sliderField()
- Vertical sliderBox (sliderBoxV());
- offsetGroup() element;
- scrollable() element;
- scrollablePanel() element;
- window() element;
- scrollableWindow() element;
- Group element spacing now is customizable through styles;
- Improvements on documentation

Version 0.2 - Freestyle
-----------------------

- textBox basic UTF-8 awareness;
- A basic element style model;
- Styles for {text|int|double}Fields;
- Custom border sizes;
- Static theming;
- Custom font sizes;
- Custom fonts;
- An alternative darkish theme;
- movable cursor over inputBox;
- int and double box stepable;
- slider;

Version 0.1 - MVP
-----------------

- Base State object holding the display lists, ids to current focused items and
  other general state book keeping;
- DisplayList able to render retangular boxes, both colored and textured;
- Frame object, representing a single rendering frame;
- Leaf elements:
  - label - display single line of text;
  - button - action when clicked;
  - toggleButton - toggle a boolean when clicked;
  - choiceButton - set a variable to a given value when clicked;
  - textBox, textField - edit single line strings;
  - intBox, intField - edit a signed integer number;
  - doubleBox, doubleField - edit a double number;
- Group elements:
  - group - a vanilla group;
  - panel - a group with borders and a background color;
- Layouts:
  - NONE: free positioning;
  - VERTICAL: position each element below the previous one;
  - HORIZONTAL: position each element on the left of the previous one;
- Single header script
