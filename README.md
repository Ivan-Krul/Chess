# Chess
A game of chess with local multiplayer.
It's a full version, as possible: with castling, en passant, promotion and check, mate, draw and finally stalemate.

# Compiling Chess
It's only for Visual Studio 2022
Load project and install to local folder

If you have a deal with OpenGl version, then install GLFW library for x86 or x64 for your prefer and "include" directory [there](https://www.glfw.org/download.html).
Also download json inline include by nlohmann [there](https://github.com/nlohmann/json/releases).

Next, install all in local folder "Dependecies" and like so:
```
    Solution directory
     |
     O "Chess"
     |
     O "ChessCore"
     |
     O "GLerKa"
     |
     O "Dependecies"
         |
         O "GLFW"
         |   |
         |   O "include" (put include files of GLFW here)
         |   |
         |   O "x86" (put GLFW library for x86 here)
         |   |
         |   O "x64" (put GLFW library for x64 here)
         |
         O "nlohmannjson.hpp" (include file json.hpp by nlohmann)
```

If you set up corectly, compilation have done.
