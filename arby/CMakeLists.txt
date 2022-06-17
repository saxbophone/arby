include(GNUInstallDirs)

CPMFindPackage(
    NAME Codlili
    GIT_REPOSITORY https://github.com/saxbophone/codlili.git
    GIT_TAG v0.5.0
)

add_library(arby STATIC)
# ALIAS target to export a namespaced target even when building in-tree
add_library(Arby::arby ALIAS arby)
# add source files
add_subdirectory(src)
# library public header files location
target_include_directories(
    arby PUBLIC
    # different include directory path depending on if using the local or installed version of library
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
# pass in version of project as preprocessor definitions
target_compile_definitions(
    arby PRIVATE
    -DARBY_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    -DARBY_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    -DARBY_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    -DARBY_VERSION_STRING=${ARBY_ESCAPED_VERSION_STRING}
)
# set up version and soversion for the main library object
set_target_properties(
    arby PROPERTIES
    VERSION ${ARBY_VERSION_STRING}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    INTERFACE_Arby_MAJOR_VERSION ${PROJECT_VERSION_MAJOR}
    INTERFACE_Arby_MINOR_VERSION ${PROJECT_VERSION_MINOR}
)
set_property(
    TARGET arby
    APPEND PROPERTY COMPATIBLE_INTERFACE_STRING "${Arby_MAJOR_VERSION}.${Arby_MINOR_VERSION}"
)
# inherit common project compiler options
target_link_libraries(
    arby
        PRIVATE
            $<BUILD_INTERFACE:arby-compiler-options>
)
# link to codlili, using public linkage as we'll need to install its headers too
target_link_libraries(arby PUBLIC Codlili::codlili)

# library
install(
    TARGETS arby
    EXPORT ArbyTargets
    # when a static library
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    # when a shared library on UNIX
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    # when a DLL on Windows
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
# public headers
install(
    DIRECTORY "include/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
# CMake export
install(
    EXPORT ArbyTargets
    FILE ArbyTargets.cmake
    NAMESPACE Arby::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Arby"
)
# CMake package
include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/ArbyConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Arby"
)
# generate the version file for the config file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ArbyConfigVersion.cmake"
    VERSION "${version}"
    COMPATIBILITY SameMinorVersion
)
install(
    FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ArbyConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ArbyConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Arby"
)
