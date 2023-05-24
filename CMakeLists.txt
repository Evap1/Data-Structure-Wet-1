cmake_minimum_required(VERSION 3.24)
project(wet1)

set(CMAKE_CXX_STANDARD 17)

add_executable(wet1 main.cpp AVL.h Movie.h Movie.cpp User.cpp User.h Group.cpp Group.h Webflix.cpp Webflix.h StreamingDBa1.h StreamingDBa1.cpp wet1util.h)
