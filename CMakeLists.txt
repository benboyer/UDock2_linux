# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.10)

# # Project's name
# project(miniMolViewer)

# # enable C++11 standard
# set(CMAKE_CXX_STANDARD 11) 
# set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -static-libgcc -static-libstdc++ -g")

#Bring the headers,into the project
# include_directories(include glm-0.9.9.5 bullet imgui)

# LINK_LIBRARIES(
#  BulletDynamics BulletCollision LinearMath
# )

# # #get the source
# set (IMGUI_SOURCE_FILES imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_impl_sdl_gl3.cpp)
# set (SOURCE_FILES  src/main.cpp  src/gl3w.c src/marching_cubes.cpp src/Atom.cpp ${IMGUI_SOURCE_FILES})



# FIND_PACKAGE( Boost 1.40 COMPONENTS  atomic thread system  REQUIRED )
# INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )



# #create an executable
# set (PROJECT_NAME mmv)
# set (WIN_RESSOURCE_FILE ressources/toto.rc)
# add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${WIN_RESOURCE_FILE})


# IF(WIN32 OR APPLE) # Check if we are on Windows

# # SDL
# set(SDL_ROOT            "${miniMolViewer_SOURCE_DIR}/SDL2-2.0.5/i686-w64-mingw32/")
# set(SDL2_INCLUDE_DIRS        "${SDL_ROOT}/include")
# set(SDL2_LIBRARIES            "${SDL_ROOT}/lib/")
# # Importing the targets
# add_library(SDL2        STATIC IMPORTED)
# add_library(SDL2main    STATIC IMPORTED)

# set_property(TARGET SDL2        PROPERTY IMPORTED_LOCATION ${SDL2_LIBRARIES}/libSDL2.a)
# set_property(TARGET SDL2main    PROPERTY IMPORTED_LOCATION ${SDL2_LIBRARIES}/libSDL2main.a)
# # Libs to link against, order is actually important
# set(SDL2_LIBRARIES
#                 mingw32 SDL2main SDL2   #SDL2_image SDL2_ttf
#                 m dinput8 dxguid dxerr8 user32
#                 gdi32 winmm imm32 ole32 oleaut32
#                 shell32 version uuid)
 	
# #target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${SDL2_INCLUDE_DIRS})
# #target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})


# elseif(UNIX)
# # # lib needed
# INCLUDE(FindPkgConfig)
# PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

# else()
#   message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
# ENDIF()

# find_package(OpenGL REQUIRED)

# INCLUDE_DIRECTORIES( ${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS} ${SDL2_LIBRARIES}  ${Boost_LIBRARIES} )



# #cpack stuff
# # General CPACK options
# set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
# set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
# set(CPACK_PACKAGE_VENDOR "Benjamin Boyer - CNAM")
# set(CPACK_PACKAGE_VERSION_MAJOR "1")
# set(CPACK_PACKAGE_VERSION_MINOR "0")

# # set(CPACK_NSIS_MODIFY_PATH ON)
# # set (CPACK_CREATE_DESKTOP_LINKS  mmv)

# set(CPACK_NSIS_EXECUTABLES_DIRECTORY .)
# # set (CPACK_NSIS_MUI_FINISHPAGE_RUN ON)
# set (CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL)
# set (CPACK_PACKAGE_EXECUTABLES "mmv" "MMV")
# # set (CPACK_NSIS_MENU_LINKS bin/mmv.exe "mmv")

# # # # To put a shortcut to a file , in the subdirectory "Your directory" 
# # SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS " CreateShortCut \\\"$DESKTOP\\\\mmv.lnk\\\"  \\\"$INSTDIR\\\\bin\\\\mmv.exe\\\" \n"  )
# # # # Don't forget to tell cpack to add the "shortcut.lnk"  in the uninstall routine by doing this : 
# # SET(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "${UNINSTALL_LIST} \n Delete \\\"$DESKTOP\\\\mmv.lnk\\\" ")

# # The file that will be displayed in the LICENSE section of the installer.
#     # set(CPACK_RESOURCE_FILE_LICENSE ${sxtalsoft_SOURCE_DIR}\\\\LICENSE.txt)



# #icon stuff
# # The icon to start the application.
# set(CPACK_NSIS_MUI_ICON ${CMAKE_SOURCE_DIR}/resources/udock.ico)
# # Set the icon for the application in the Add/Remove programs section.
# set(CPACK_NSIS_INSTALLED_ICON_NAME ${CMAKE_SOURCE_DIR}/resources/udock.ico)


# # #windows runtime librarues
#  set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
#  include(InstallRequiredSystemLibraries)
#  install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
#          DESTINATION bin)
# #necessary data
# install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources/
#         DESTINATION resources)
# install(DIRECTORY ${CMAKE_SOURCE_DIR}/shaders/
#         DESTINATION shaders)
# install(DIRECTORY ${CMAKE_SOURCE_DIR}/textures/
#         DESTINATION textures)

# install(TARGETS ${PROJECT_NAME}
#         RUNTIME
#         DESTINATION bin)


# include(CPack)




# UDOCK parts, quick n' dirty test should reorganise the cmakelist files by directory


# Project's name
project(udock)

# enable C++11 standard
set(CMAKE_CXX_STANDARD 11) 

if(APPLE)
# Clang does not support static-libgcc
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -g")
else()
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -static-libgcc -static-libstdc++")
ENDIF()

SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -framework CoreFoundation")

#Bring the headers,into the project
include_directories(include glm-0.9.9.5 bullet/src imgui CPPCartoon)

#get the source
set (IMGUI_SOURCE_FILES imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_impl_sdl_gl3.cpp)
set (SOURCE_FILES  udock/main.cpp  src/gl3w.c src/marching_cubes.cpp src/Atom.cpp ${IMGUI_SOURCE_FILES})

# LINK_LIBRARIES(
#  BulletDynamics BulletCollision LinearMath
# )



FIND_PACKAGE( Boost 1.40 COMPONENTS  atomic thread system  REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

#create an executable
set (PROJECT_NAME udock)
set (WIN_RESSOURCE_FILE ressources/toto.rc)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${WIN_RESOURCE_FILE})


#Bring the headers,into the project
include_directories(include glm-0.9.9.5 bullet imgui)







IF(WIN32) # Check if we are on Windows

# SDL
set(SDL_ROOT            "${miniMolViewer_SOURCE_DIR}/SDL2-2.0.5/i686-w64-mingw32/")
set(SDL2_INCLUDE_DIRS        "${SDL_ROOT}/include")
set(SDL2_LIBRARIES            "${SDL_ROOT}/lib/")
# Importing the targets
add_library(SDL2        STATIC IMPORTED)
add_library(SDL2main    STATIC IMPORTED)

set_property(TARGET SDL2        PROPERTY IMPORTED_LOCATION ${SDL2_LIBRARIES}/libSDL2.a)
set_property(TARGET SDL2main    PROPERTY IMPORTED_LOCATION ${SDL2_LIBRARIES}/libSDL2main.a)
# Libs to link against, order is actually important
set(SDL2_LIBRARIES
                mingw32 SDL2main SDL2   #SDL2_image SDL2_ttf
                m dinput8 dxguid dxerr8 user32
                gdi32 winmm imm32 ole32 oleaut32
                shell32 version uuid)
    
#target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${SDL2_INCLUDE_DIRS})
#target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})


elseif(UNIX)
# # lib needed
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

elseif(APPLE)
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
target_link_libraries(${PROJECT_NAME} -demangle -dynamic -arch x86_64)

else()
  message(SEND_ERROR "You are on an unsupported platform! (Not Win32, Apple or Unix)")
ENDIF()

find_package(OpenGL REQUIRED)
find_package(Bullet REQUIRED)


INCLUDE_DIRECTORIES( ${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${BULLET_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS} ${SDL2_LIBRARIES}  ${Boost_LIBRARIES} ${BULLET_LIBRARIES})



#cpack stuff
# General CPACK options
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "Benjamin Boyer - CNAM")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")

# set(CPACK_NSIS_MODIFY_PATH ON)
# set (CPACK_CREATE_DESKTOP_LINKS  mmv)

set(CPACK_NSIS_EXECUTABLES_DIRECTORY .)
# set (CPACK_NSIS_MUI_FINISHPAGE_RUN ON)
set (CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL)
set (CPACK_PACKAGE_EXECUTABLES "udock" "Udock")
# set (CPACK_NSIS_MENU_LINKS bin/mmv.exe "mmv")

# # # To put a shortcut to a file , in the subdirectory "Your directory" 
# SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS " CreateShortCut \\\"$DESKTOP\\\\mmv.lnk\\\"  \\\"$INSTDIR\\\\bin\\\\mmv.exe\\\" \n"  )
# # # Don't forget to tell cpack to add the "shortcut.lnk"  in the uninstall routine by doing this : 
# SET(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "${UNINSTALL_LIST} \n Delete \\\"$DESKTOP\\\\mmv.lnk\\\" ")

# The file that will be displayed in the LICENSE section of the installer.
    # set(CPACK_RESOURCE_FILE_LICENSE ${sxtalsoft_SOURCE_DIR}\\\\LICENSE.txt)



#icon stuff
# The icon to start the application.
set(CPACK_NSIS_MUI_ICON ${CMAKE_SOURCE_DIR}/resources/udock.ico)
# Set the icon for the application in the Add/Remove programs section.
set(CPACK_NSIS_INSTALLED_ICON_NAME ${CMAKE_SOURCE_DIR}/resources/udock.ico)


# #windows runtime librarues
 set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
 include(InstallRequiredSystemLibraries)
 install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
         DESTINATION bin)
#necessary data
install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources/
        DESTINATION resources)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/shaders/
        DESTINATION shaders)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/textures/
        DESTINATION textures)

install(TARGETS ${PROJECT_NAME}
        RUNTIME
        DESTINATION bin)


include(CPack)
