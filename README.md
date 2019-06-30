# smolSf
Small SFML 2.5 header-only C++17 wrapper made for personal raymarching/sphere tracing project.


## Overview

This wrapper was developed to help with a personal raymarcher since I found SFML to be somewhat cumbersome.
As such it doesn't have any support for drawing anything but buffer of pixel data and text to screen.
Input from keyboard and mouse has also been somewhat reworked.
Project also uses some features of C++17 like ```[[nodiscard]]``` to help with development.

## Build

```sfml-graphics.lib```,```sfml-window.lib```,```sfml-system-d.lib``` libraries are necessary as well as SFML include directory.
This wrapper has only been tested on SFML 2.5.1.


## Features and examples

### Constructing windows

smol_windows can be constructed using normal coordinates(size and top-left corner) or they can be constructed using system inspired by matlab subplot.

For example:
```
smolSf::smol_window window (3,3,{0,2},0);
smolSf::smol_window window2(3,3,2,{1,2});
smolSf::smol_window window3(3,3,0,2);
```

This will partition screen into 9 cells(3 by 3). Next constructor expects either a unsigned integer or a initilizer list of unsigned integers.
If two integers are provided window will take up a cell at (x,y).
If list or lists such as {a,b} and {c,d} are provided window will take up a rectangle of cells starting from {a,c} (top-left corner) to {b,d} (bottom-right corner).

### Console style output

smol_window can be used with combination stream operator << to create a console style output.

For example:
```
window << "Framerate: " << framerate <<  sf::endl;
window << "Camera position: " << camera.pos << sf::endl;
```
smol_window can accept any type of variable as long as it has an overloaded stream operator for generic ostream.
Display function has to be called to actually show the text on screen.
There is no default font is SFML so one has to be loaded before stream operator is used.

### Revised input

There are three arrays of bools for keyboard as well as mouse. They are updated when respectively ```poll_keyboard()``` and ```poll_mouse()``` are called.
```isKeyPressed``` checks if key is pressed at the moment of polling inputs.
```isKeyDown``` is true if key was not pressed at last poll and is pressed at this poll.
```isKeyUp``` is true if key was pressed at last poll and is not pressed at this poll.

```add_on_key_*(sf::Keyboard::Key, std::function<void()>)``` can be used to add function that will be triggered at every poll. 
Functions will be first called on_key_down, then on_key_up and on_key_pressed.

All functions are static functions in input class.

### smol_helper

smol_helper will poll input and clear all windows when its constructed and display windows when its destroyed.

### tic-toc

chrono class contains static functions tic, toc and toc_as_fseconds, which are used used to measure time. Whenever
tic is called a current time is pushed into a queue. When toc is called current time is compared with first time in the queue
which is then removed from the queue.
Difference is returned as std::chrono::nanoseconds.
If you don't want to deal with std::chrono, toc_as_fseconds returns a float which is a output of toc converted to number of seconds.

## Example

```
smolSf::chrono::tic();
smolSf::smol_window w(2, 2, 0, 0);
smolSf::smol_window w2(2, 2, { 0,2 }, { 1 });
smolSf::smol_window w3(2, 2, 1, 0);
std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(smolSf::chrono::toc()).count() << "ms\n";

smolSf::character_size = 16;
w2.font.loadFromFile("default.ttf");

smolSf::input::add_on_key_up(sf::Keyboard::Escape, [&]() { smolSf::all_close(); });

while (smolSf::any_isOpen()) {
	smolSf::smol_helper help;
	smolSf::chrono::tic();
	w2 << "Framerate: " << (size_t) (1/smolSf::chrono::toc_as_fseconds());
}
```




