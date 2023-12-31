cmake_minimum_required(VERSION 3.5)
project(cart_pole_gazebo)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)


# # for gazebo plugins
# find_package(gazebo REQUIRED)
# include_directories(${GAZEBO_INCLUDE_DIRS})
# link_directories(${GAZEBO_LIBRARY_DIRS})
# list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

# # create a subdirectory and add basic_world_plugin (inside the world_plugins folder) into it
# add_subdirectory(world_plugins)
# target_include_directories(basic_world_plugin PUBLIC world_plugins)


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# moves into /share/${PROJECT_NAME}
install(
  DIRECTORY
    description
    launch
    # worlds
  DESTINATION
    share/${PROJECT_NAME}
)

# moves into /install/${PROJECT_NAME}
install(
  PROGRAMS
  launch/cart_pole_sim_launch.py
  launch/worldPlugin.launch.py
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
