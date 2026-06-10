cmake_minimum_required(VERSION 3.16)

add_library(
  GameObject
  STATIC
  GameObject.cpp
  UIObjects.cpp
)

target_link_libraries(
  GameObject
  Framework
)
