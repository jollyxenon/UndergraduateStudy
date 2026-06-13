cmake_minimum_required(VERSION 3.16)

add_library(
  GameObject
  STATIC
  GameObject.cpp
  PlantObjects.cpp
  ProjectileObjects.cpp
  SunObjects.cpp
  UIObjects.cpp
  ZombieObjects.cpp
)

target_link_libraries(
  GameObject
  Framework
  GameGeometry
)
