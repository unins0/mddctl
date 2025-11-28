cmake_minimum_required(VERSION 3.22)
project(mddctl)
set(CMAKE_CXX_STANDART 20)
set(CMAKE_CXX_STANDART_REQUIRED True)
add_executable(mddctl main.cpp)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)
include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
  GIT_TAG        v6.1.9
  GIT_PROGRESS   TRUE
  GIT_SHALLOW    TRUE
  EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(ftxui)

include_directories(${LIBUSB_INCLUDE_DIRS})
target_link_libraries(mddctl 
  PRIVATE ${LIBUSB_LIBRARIES}
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
