# FindPipeWire.cmake
#
# Finds the PipeWire library using pkg-config
#
# This will define the following variables:
#
#   PIPEWIRE_FOUND        - True if PipeWire has been found
#   PIPEWIRE_INCLUDE_DIRS - PipeWire include directory
#   PIPEWIRE_LIBRARIES    - PipeWire libraries
#   PIPEWIRE_VERSION      - PipeWire version string
#
# and the following imported target:
#
#   PipeWire::PipeWire

find_package(PkgConfig REQUIRED)
pkg_check_modules(PIPEWIRE QUIET libpipewire-0.3)

if(PIPEWIRE_FOUND)
  # Create an imported target
  add_library(PipeWire::PipeWire INTERFACE IMPORTED)
  set_target_properties(PipeWire::PipeWire PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PIPEWIRE_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES "${PIPEWIRE_LIBRARIES}"
  )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PipeWire
  REQUIRED_VARS PIPEWIRE_LIBRARIES PIPEWIRE_INCLUDE_DIRS
  VERSION_VAR PIPEWIRE_VERSION
)

mark_as_advanced(PIPEWIRE_INCLUDE_DIRS PIPEWIRE_LIBRARIES PIPEWIRE_VERSION)
