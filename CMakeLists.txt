cmake_minimum_required(VERSION 3.10)
project(SNAKE)

set(CMAKE_CXX_STANDARD 20)

# Ścieżka do rozpakowanego SFML
set(SFML_DIR "C:/Users/kawir/Downloads/SFML-3.0.0-windows-gcc-14.2.0-mingw-64-bit/SFML-3.0.0/lib/cmake/SFML")

# Dodanie katalogu z plikami nagłówkowymi SFML
include_directories("C:/Users/kawir/Downloads/SFML-3.0.0-windows-gcc-14.2.0-mingw-64-bit/SFML-3.0.0/include")

# Dodanie katalogu z bibliotekami SFML
link_directories("C:/Users/kawir/Downloads/SFML-3.0.0-windows-gcc-14.2.0-mingw-64-bit/SFML-3.0.0/lib")

# Tworzenie pliku wykonywalnego z Twojego programu
add_executable(SNAKE main.cpp)

# Łączenie z bibliotekami SFML
target_link_libraries(SNAKE sfml-graphics sfml-window sfml-system)
