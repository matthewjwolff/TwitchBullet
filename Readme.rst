Twitch Bullet
=============

(tentative title)

A celestial body physics simulator using Twitch.tv's bits system. Donate to your favorite streamers to combine and collide galaxies of bits!

Licensed under GPLv3. Binaries (will be) provided for a fee, obtaining source code and building for yourself is gratis.

Dependencies
------------

- CMake
- Bullet3
- OpenGL >3 drivers
- GLFW
- GLEW
- GLM

Use your system's package manager to install these. Ideally CMake should handle linking. If this list is out of date, consult the root ``CMakeLists.txt``.

Build Instructions
------------------

1. Run CMake in the root directory (``cmake .``) to generate your system's preferred build environment.
2. (**Unix**) ``make`` / (**OSX untested**) Open the generated XCode project and build / (**Windows untested**) Open the generated Visual Studio project and build
3. Run the executable (``./TwitchBullet``)
