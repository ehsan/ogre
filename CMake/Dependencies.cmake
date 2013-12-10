#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

#######################################################################
# Find all necessary and optional OGRE dependencies
#######################################################################

# OGRE_DEPENDENCIES_DIR can be used to specify a single base
# folder where the required dependencies may be found.
set(OGRE_DEPENDENCIES_DIR "" CACHE PATH "Path to prebuilt OGRE dependencies")
include(FindPkgMacros)
getenv_path(OGRE_DEPENDENCIES_DIR)
if(OGRE_BUILD_PLATFORM_APPLE_IOS)
  set(OGRE_DEP_SEARCH_PATH 
    ${OGRE_DEPENDENCIES_DIR}
    ${ENV_OGRE_DEPENDENCIES_DIR}
    "${OGRE_BINARY_DIR}/iPhoneDependencies"
    "${OGRE_SOURCE_DIR}/iPhoneDependencies"
    "${OGRE_BINARY_DIR}/../iPhoneDependencies"
    "${OGRE_SOURCE_DIR}/../iPhoneDependencies"
  )
else()
  set(OGRE_DEP_SEARCH_PATH 
    ${OGRE_DEPENDENCIES_DIR}
    ${ENV_OGRE_DEPENDENCIES_DIR}
    "${OGRE_BINARY_DIR}/Dependencies"
    "${OGRE_SOURCE_DIR}/Dependencies"
    "${OGRE_BINARY_DIR}/../Dependencies"
    "${OGRE_SOURCE_DIR}/../Dependencies"
  )
endif()

message(STATUS "Search path: ${OGRE_DEP_SEARCH_PATH}")

# Set hardcoded path guesses for various platforms
if (UNIX)
  set(OGRE_DEP_SEARCH_PATH ${OGRE_DEP_SEARCH_PATH} /usr/local)
  # Ubuntu 11.10 has an inconvenient path to OpenGL libraries
  set(OGRE_DEP_SEARCH_PATH ${OGRE_DEP_SEARCH_PATH} /usr/lib/${CMAKE_SYSTEM_PROCESSOR}-linux-gnu)
endif ()

# give guesses as hints to the find_package calls
set(CMAKE_PREFIX_PATH ${OGRE_DEP_SEARCH_PATH} ${CMAKE_PREFIX_PATH})
set(CMAKE_FRAMEWORK_PATH ${OGRE_DEP_SEARCH_PATH} ${CMAKE_FRAMEWORK_PATH})

#######################################################################
# Core dependencies
#######################################################################

# Find zlib
find_package(ZLIB)
macro_log_feature(ZLIB_FOUND "zlib" "Simple data compression library" "http://www.zlib.net" FALSE "" "")

if (ZLIB_FOUND)
  # Find zziplib
  find_package(ZZip)
  macro_log_feature(ZZip_FOUND "zziplib" "Extract data from zip archives" "http://zziplib.sourceforge.net" FALSE "" "")
endif ()

# Find FreeImage
find_package(FreeImage)
macro_log_feature(FreeImage_FOUND "freeimage" "Support for commonly used graphics image formats" "http://freeimage.sourceforge.net" FALSE "" "")

# Find FreeType
find_package(Freetype)
macro_log_feature(FREETYPE_FOUND "freetype" "Portable font engine" "http://www.freetype.org" TRUE "" "")

# Find X11
if (UNIX AND NOT OGRE_BUILD_PLATFORM_APPLE_IOS AND NOT OGRE_BUILD_PLATFORM_EMSCRIPTEN)
  find_package(X11)
  macro_log_feature(X11_FOUND "X11" "X Window system" "http://www.x.org" TRUE "" "")
  macro_log_feature(X11_Xt_FOUND "Xt" "X Toolkit" "http://www.x.org" TRUE "" "")
  find_library(XAW_LIBRARY NAMES Xaw Xaw7 PATHS ${OGRE_DEP_SEARCH_PATH} ${DEP_LIB_SEARCH_DIR} ${X11_LIB_SEARCH_PATH})
  macro_log_feature(XAW_LIBRARY "Xaw" "X11 Athena widget set" "http://www.x.org" TRUE "" "")
  mark_as_advanced(XAW_LIBRARY)
endif ()


#######################################################################
# RenderSystem dependencies
#######################################################################

# Find OpenGL
find_package(OpenGL)
macro_log_feature(OPENGL_FOUND "OpenGL" "Support for the OpenGL render system" "http://www.opengl.org/" FALSE "" "")

# Find OpenGL ES
find_package(OpenGLES)
macro_log_feature(OPENGLES_FOUND "OpenGL ES 1.x" "Support for the OpenGL ES 1.x render system" "http://www.khronos.org/opengles/" FALSE "" "")

# Find OpenGL ES 2.x
find_package(OpenGLES2)
macro_log_feature(OPENGLES2_FOUND "OpenGL ES 2.x" "Support for the OpenGL ES 2.x render system" "http://www.khronos.org/opengles/" FALSE "" "")

# Find DirectX
if(WIN32)
	find_package(DirectX)
	macro_log_feature(DirectX_FOUND "DirectX" "Support for the DirectX render system" "http://msdn.microsoft.com/en-us/directx/" FALSE "" "")
endif()

#######################################################################
# Additional features
#######################################################################

# Find Cg
if (NOT OGRE_BUILD_PLATFORM_APPLE_IOS)
  find_package(Cg)
  macro_log_feature(Cg_FOUND "cg" "C for graphics shader language" "http://developer.nvidia.com/object/cg_toolkit.html" FALSE "" "")
endif (NOT OGRE_BUILD_PLATFORM_APPLE_IOS)

# Find Boost
# Prefer static linking in all cases
if (NOT OGRE_BUILD_PLATFORM_EMSCRIPTEN)
  if (WIN32 OR APPLE)
          set(Boost_USE_STATIC_LIBS TRUE)
  else ()
          # Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
          set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
  endif ()
  if (APPLE AND OGRE_BUILD_PLATFORM_APPLE_IOS)
          set(Boost_COMPILER "-xgcc42")
  endif()
  set(Boost_ADDITIONAL_VERSIONS "1.48" "1.48.0" "1.47" "1.47.0" "1.46" "1.46.0" "1.45" "1.45.0" "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37" )
  # Components that need linking (NB does not include header-only components like bind)
  set(OGRE_BOOST_COMPONENTS thread date_time)
  find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
  if (NOT Boost_FOUND)
          # Try again with the other type of libs
          if(Boost_USE_STATIC_LIBS)
                  set(Boost_USE_STATIC_LIBS)
          else()
                  set(Boost_USE_STATIC_LIBS ON)
          endif()
          find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
  endif()
  find_package(Boost QUIET)
  # Optional Boost libs (Boost_${COMPONENT}_FOUND
  macro_log_feature(Boost_FOUND "boost" "Boost (general)" "http://boost.org" FALSE "" "")
  macro_log_feature(Boost_THREAD_FOUND "boost-thread" "Used for threading support" "http://boost.org" FALSE "" "")
  macro_log_feature(Boost_DATE_TIME_FOUND "boost-date_time" "Used for threading support" "http://boost.org" FALSE "" "")
endif()

# POCO
find_package(POCO)
macro_log_feature(POCO_FOUND "POCO" "POCO framework" "http://pocoproject.org/" FALSE "" "")

# ThreadingBuildingBlocks
find_package(TBB)
macro_log_feature(TBB_FOUND "tbb" "Threading Building Blocks" "http://www.threadingbuildingblocks.org/" FALSE "" "")

# GLSL-Optimizer
find_package(GLSLOptimizer)
macro_log_feature(GLSL_Optimizer_FOUND "GLSL Optimizer" "GLSL Optimizer" "http://github.com/aras-p/glsl-optimizer/" FALSE "" "")

# HLSL2GLSL
find_package(HLSL2GLSL)
macro_log_feature(HLSL2GLSL_FOUND "HLSL2GLSL" "HLSL2GLSL" "http://hlsl2glslfork.googlecode.com/" FALSE "" "")


#######################################################################
# Samples dependencies
#######################################################################

# Find OIS
find_package(OIS)
macro_log_feature(OIS_FOUND "OIS" "Input library needed for the samples" "http://sourceforge.net/projects/wgois" FALSE "" "")

#######################################################################
# Tools
#######################################################################

find_package(Doxygen)
macro_log_feature(DOXYGEN_FOUND "Doxygen" "Tool for building API documentation" "http://doxygen.org" FALSE "" "")

# Find Softimage SDK
find_package(Softimage)
macro_log_feature(Softimage_FOUND "Softimage" "Softimage SDK needed for building XSIExporter" FALSE "6.0" "")

#######################################################################
# Tests
#######################################################################

find_package(CppUnit)
macro_log_feature(CppUnit_FOUND "CppUnit" "Library for performing unit tests" "http://cppunit.sourceforge.net" FALSE "" "")

#######################################################################
# Apple-specific
#######################################################################
if (APPLE)
  find_package(iPhoneSDK)
  macro_log_feature(iPhoneSDK_FOUND "iOS SDK" "iOS SDK" "http://developer.apple.com/ios" FALSE "" "")
  
  if (NOT OGRE_BUILD_PLATFORM_APPLE_IOS)
    find_package(Carbon)
    macro_log_feature(Carbon_FOUND "Carbon" "Carbon" "http://developer.apple.com/mac" TRUE "" "")

    find_package(Cocoa)
    macro_log_feature(Cocoa_FOUND "Cocoa" "Cocoa" "http://developer.apple.com/mac" TRUE "" "")

    find_package(IOKit)
    macro_log_feature(IOKit_FOUND "IOKit" "IOKit HID framework needed by the samples" "http://developer.apple.com/mac" FALSE "" "")

    find_package(CoreVideo)
    macro_log_feature(CoreVideo_FOUND "CoreVideo" "CoreVideo" "http://developer.apple.com/mac" TRUE "" "")
  endif (NOT OGRE_BUILD_PLATFORM_APPLE_IOS)
endif(APPLE)


# now see if we have a buildable Dependencies package in
# the source tree. If so, include that, and it will take care of
# setting everything up, including overriding any of the above
# findings.
set(OGREDEPS_RUNTIME_OUTPUT ${OGRE_RUNTIME_OUTPUT})
if (EXISTS "${OGRE_SOURCE_DIR}/Dependencies/CMakeLists.txt")
  add_subdirectory(Dependencies)
elseif (EXISTS "${OGRE_SOURCE_DIR}/ogredeps/CMakeLists.txt")
  add_subdirectory(ogredeps)
endif ()


# Display results, terminate if anything required is missing
MACRO_DISPLAY_FEATURE_LOG()

# Add library and include paths from the dependencies
include_directories(
  ${ZLIB_INCLUDE_DIRS}
  ${ZZip_INCLUDE_DIRS}
  ${FreeImage_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIRS}
  ${OPENGLES_INCLUDE_DIRS}
  ${OPENGLES2_INCLUDE_DIRS}
  ${OIS_INCLUDE_DIRS}
  ${Cg_INCLUDE_DIRS}
  ${X11_INCLUDE_DIR}
  ${DirectX_INCLUDE_DIRS}
  ${CppUnit_INCLUDE_DIRS}
  ${Carbon_INCLUDE_DIRS}
  ${Cocoa_INCLUDE_DIRS}
  ${CoreVideo_INCLUDE_DIRS}
  ${GLSL_Optimizer_INCLUDE_DIRS}
  ${HLSL2GLSL_INCLUDE_DIRS}
)

link_directories(
  ${OPENGL_LIBRARY_DIRS}
  ${OPENGLES_LIBRARY_DIRS}
  ${OPENGLES2_LIBRARY_DIRS}
  ${Cg_LIBRARY_DIRS}
  ${X11_LIBRARY_DIRS}
  ${DirectX_LIBRARY_DIRS}
  ${CppUnit_LIBRARY_DIRS}
)

if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif ()

# provide option to install dependencies on Windows
include(InstallDependencies)
