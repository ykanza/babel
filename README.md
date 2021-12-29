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
## Commits
![commits](https://user-images.githubusercontent.com/75526108/147616571-91e2c8a9-3add-4de6-b91b-971521d39dd2.png)
## Branches
![branches](https://user-images.githubusercontent.com/75526108/147616614-1285b9b7-8a49-4ef6-845d-7a4856365b55.png)
## Contributors
![contributors](https://user-images.githubusercontent.com/75526108/147616617-5664bac8-01ff-41f6-9bab-ac8ef044f53f.png)
