# Babel
- Client/server architecture voice over IP application, similar to Skype or TeamSpeak
- OS independent, compile and run on Unix system and Windows systems.
- Built using a CMake and dependencies handled using conan.
## Libraries
- Qt (client’s graphical user interface)
- Port Audio (sound-related code)
- Opus (compression codec)
- Asio (custom-made server side)
- Qt Network (server-side networking)
##  Build (Unix systems)
mkdir build && cd build && conan install .. && cmake .. -G “Unix
Makefiles” && cmake –build .
