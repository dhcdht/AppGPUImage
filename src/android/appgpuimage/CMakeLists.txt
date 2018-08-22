# Sets the minimum version of CMake required to build your native library.
# This ensures that a certain set of CMake features is available to
# your build.

cmake_minimum_required(VERSION 3.4.1)
# 使用 c++14
set(CMAKE_CXX_STANDARD 14)
# 不使用 rtti
add_compile_options(-fno-rtti)

file(GLOB_RECURSE COMMON_SOURCES ../../common/*.cpp)

set( PUBLIC_HEADERS
../../common/SDKTest.h
)

set( PROJECT_HEADERS
../../common/IO/AGIImage.h
../../common/IO/input/AGIPiplineInput.h
../../common/IO/input/AGIPiplineInputFFmpegReader.h
../../common/IO/input/AGIPiplineInputPicture.h
../../common/IO/output/AGIPiplineOutputContextWindow.h
../../common/core/AGIContext.h
../../common/core/AGIDispatchQueue.h
../../common/core/AGIResourceManager.h
../../common/filter/AGIFilter.h
../../common/filter/AGIFilterAlphaBlend.h
../../common/filter/AGIFilterContentMode.h
../../common/filter/AGIFilterGrayscale.h
../../common/filter/AGIFilterSobelEdgeDetection.h
../../common/filter/AGIFilterTransform.h
../../common/filter/AGIFilterTwoPass.h
../../common/filter/core/AGIProgram.h
../../common/filter/core/AGIProgramCacheManager.h
../../common/piplineIO/AGIPiplineIO.hpp
../../common/timeline/AGIClip.h
../../common/timeline/AGITimeline.h
../../common/timeline/AGITrack.h
../../common/util/AGIUtilDefer.h
../../common/util/bgfxUtils.h
../../common_deps/bgfx/3rdparty/glsl-optimizer/src/glsl/glsl_optimizer.h
)

set( PRIVATE_HEADERS
)

set( RESOURCES
../../common_res/shaders
)

# Specifies a library name, specifies whether the library is STATIC or
# SHARED, and provides relative paths to the source code. You can
# define multiple libraries by adding multiple add.library() commands,
# and CMake builds them for you. When you build your app, Gradle
# automatically packages shared libraries with your APK.

add_library( # Specifies the name of the library.
             AppGPUImage

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ${COMMON_SOURCES}
             ${PUBLIC_HEADERS}
             ${PROJECT_HEADERS}
             ${PRIVATE_HEADERS}
             ${RESOURCES}
             )

target_include_directories( AppGPUImage
   PUBLIC
      ../../common
      ../../common_deps/bgfx/include
      ../../common_deps/bx/include
      ../../common_deps/bimg/include
      ../../common_deps/ffmpeg/include
      ../../common_deps/bgfx/3rdparty/glsl-optimizer/src/glsl
      ../../common_deps/bgfx/3rdparty/fcpp
)

target_link_libraries(AppGPUImage
    -lz
    -landroid
    -llog
)
find_library(path_egl EGL)
find_library(path_glesv2 GLESv2)
target_link_libraries(AppGPUImage
    ${path_glesv2}
    ${path_egl}
)

    set(AndroidNDKBundlePath ~/Library/Android/sdk/ndk-bundle)
    add_custom_target(bgfx
                      /usr/bin/make
                      -e "ANDROID_NDK_CLANG='${AndroidNDKBundlePath}/toolchains/llvm/prebuilt/darwin-x86_64'"
                      -e "ANDROID_NDK_ROOT='${AndroidNDKBundlePath}'"
                      -e "ANDROID_NDK_ARM='${AndroidNDKBundlePath}/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64'"
                      -C "${CMAKE_CURRENT_SOURCE_DIR}/../../common_deps/bgfx/.build/projects/gmake-android-arm"
                      -f "./Makefile"
                      )
add_dependencies(AppGPUImage bgfx)
target_link_libraries(AppGPUImage
    ${PROJECT_SOURCE_DIR}/../../common_deps/bgfx/.build/android-arm/bin/libbimgDebug.a
    ${PROJECT_SOURCE_DIR}/../../common_deps/bgfx/.build/android-arm/bin/libbimg_decodeDebug.a
    ${PROJECT_SOURCE_DIR}/../../common_deps/bgfx/.build/android-arm/bin/libbgfxDebug.a
    ${PROJECT_SOURCE_DIR}/../../common_deps/bgfx/.build/android-arm/bin/libbxDebug.a
)

target_link_libraries(AppGPUImage
    ${PROJECT_SOURCE_DIR}/../../common_deps/ffmpeg/android/lib/armeabi-v7a/libavformat.a
    ${PROJECT_SOURCE_DIR}/../../common_deps/ffmpeg/android/lib/armeabi-v7a/libavcodec.a
    ${PROJECT_SOURCE_DIR}/../../common_deps/ffmpeg/android/lib/armeabi-v7a/libavutil.a
    ${PROJECT_SOURCE_DIR}/../../common_deps/ffmpeg/android/lib/armeabi-v7a/libswscale.a
    ${PROJECT_SOURCE_DIR}/../../common_deps/ffmpeg/android/lib/armeabi-v7a/libswresample.a
)

    file(GLOB glcpp-library_sources
            ${PROJECT_SOURCE_DIR}/../../common_deps/bgfx/3rdparty/glsl-optimizer/src/glsl/glcpp/*.c
            ${PROJECT_SOURCE_DIR}/../../common_deps/bgfx/3rdparty/glsl-optimizer/src/util/*.c
            )
    add_library(glcpp-library ${glcpp-library_sources})
    target_include_directories( glcpp-library
               PUBLIC
                  ../../common_deps/bgfx/3rdparty/glsl-optimizer/include
                  ../../common_deps/bgfx/3rdparty/glsl-optimizer/src
                  ../../common_deps/bgfx/3rdparty/glsl-optimizer/src/mesa
            )

    file(GLOB mesa_sources
            ${PROJECT_SOURCE_DIR}/../../common_deps/bgfx/3rdparty/glsl-optimizer/src/mesa/program/*.c
            ${PROJECT_SOURCE_DIR}/../../common_deps/bgfx/3rdparty/glsl-optimizer/src/mesa/main/*.c
            )
    add_library(mesa ${mesa_sources})
    target_include_directories( mesa
           PUBLIC
              ../../common_deps/bgfx/3rdparty/glsl-optimizer/include
              ../../common_deps/bgfx/3rdparty/glsl-optimizer/src
              ../../common_deps/bgfx/3rdparty/glsl-optimizer/src/mesa
        )

    file(GLOB glsl_sources
            ${PROJECT_SOURCE_DIR}/../../common_deps/bgfx/3rdparty/glsl-optimizer/src/glsl/*.cpp
            ${PROJECT_SOURCE_DIR}/../../common_deps/bgfx/3rdparty/glsl-optimizer/src/glsl/*.c
            )
    file(GLOB glsl_sources_remove
            ${PROJECT_SOURCE_DIR}/../../common_deps/bgfx/3rdparty/glsl-optimizer/src/glsl/main.cpp
            ${PROJECT_SOURCE_DIR}/../../common_deps/bgfx/3rdparty/glsl-optimizer/src/glsl/builtin_stubs.cpp
            )
    list(REMOVE_ITEM glsl_sources ${glsl_sources_remove})
    add_library(glsl_optimizer ${glsl_sources})
    target_link_libraries(glsl_optimizer glcpp-library mesa)
    target_include_directories( glsl_optimizer
       PUBLIC
          ../../common_deps/bgfx/3rdparty/glsl-optimizer/include
          ../../common_deps/bgfx/3rdparty/glsl-optimizer/src
          ../../common_deps/bgfx/3rdparty/glsl-optimizer/src/mesa
    )
add_dependencies(AppGPUImage glsl_optimizer)
target_link_libraries(AppGPUImage glsl_optimizer)

    file(GLOB fcpp_sources
            ${PROJECT_SOURCE_DIR}/../../common_deps/bgfx/3rdparty/fcpp/*.cpp
            ${PROJECT_SOURCE_DIR}/../../common_deps/bgfx/3rdparty/fcpp/*.c
            )
    file(GLOB fcpp_sources_remove
            ${PROJECT_SOURCE_DIR}/../../common_deps/bgfx/3rdparty/fcpp/usecpp.c
            )
    list(REMOVE_ITEM fcpp_sources ${fcpp_sources_remove})
    add_library(fcpp ${fcpp_sources})
    target_include_directories( fcpp
           PUBLIC
              ../../common_deps/bgfx/3rdparty/fcpp
        )
add_dependencies(AppGPUImage fcpp)
target_link_libraries(AppGPUImage fcpp)
