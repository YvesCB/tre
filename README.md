# tre
A *T*erminal *R*ender *E*ngine

## The idea
Experimenting with building a render engine but relying fully on just drawing things in the terminal.

Eventually this library should be able to do full 3d graphics with simple ascii characters.

## Progress
### Initial tests and terminal setup
So far we can set the terminal to raw mode, clear it and read user input from the keyboard.
We currently do a sort of none blocking immediate mode rendering at 30fps, or rather one render every 33ms.

### Actually drawing something
I will try to implement snake 🐍 to get started.
