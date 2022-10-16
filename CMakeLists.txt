cmake_minimum_required(VERSION 3.15)
project(geometry_rviz_plugins VERSION 0.0.0)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_AUTOMOC ON)

# find dependencies
find_package(Qt5 REQUIRED
    COMPONENTS
        Widgets
)

find_package(ament_cmake_auto REQUIRED)

ament_auto_find_build_dependencies()

qt5_wrap_cpp(geometry_rviz_plugins_moc_files
    ${CMAKE_CURRENT_SOURCE_DIR}/include/geometry_rviz_plugins/displays/vector3_stamped.hpp
)

add_library(geometry_rviz_plugins SHARED)

target_sources(geometry_rviz_plugins
    PRIVATE
        ${geometry_rviz_plugins_moc_files}
        src/displays/vector3_stamped.cpp
)
target_include_directories(geometry_rviz_plugins
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${Qt5Widgets_INCLUDE_DIRS}
)
target_link_libraries(geometry_rviz_plugins
    PUBLIC
        Qt5::Widgets
        rviz_ogre_vendor::OgreMain
        rviz_ogre_vendor::OgreOverlay
)
set_target_properties(geometry_rviz_plugins
    PROPERTIES
        VERSION ${PROJECT_VERSION}
)
ament_target_dependencies(geometry_rviz_plugins
    PUBLIC
        rclcpp
        rviz_common
        rviz_rendering
        rviz_ogre_vendor
        geometry_msgs
)

ament_export_include_directories("include/${PROJECT_NAME}")
pluginlib_export_plugin_description_file(rviz_common
    rviz_plugins_descriptions.xml
)
ament_export_targets(geometry_rviz_plugins
    HAS_LIBRARY_TARGET
)
ament_export_dependencies(geometry_rviz_plugins
    rviz_common
    rviz_ogre_vendor
    geometry_msgs
)

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

install(
    TARGETS geometry_rviz_plugins
    EXPORT geometry_rviz_plugins
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(
    DIRECTORY include/
    DESTINATION include/
)

ament_package(
    CONFIG_EXTRAS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/geometry_rviz_plugins-extras.cmake"
)
