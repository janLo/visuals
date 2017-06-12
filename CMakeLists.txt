cmake_minimum_required(VERSION 3.0)

# ToDo: fix find_library mess
# ToDo: add proper include dir handling

project(visuals)


file(GLOB FILES *.cpp *.hpp *.c *.h)

if (NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    find_package(Threads)
    find_library(ATOMIC_LIBRARY NAMES atomic atomic.so.1 libatomic.so.1 HINTS /usr/lib/${CMAKE_C_LIBRARY_ARCHITECTURE} PATH_SUFFIXES )
    find_library(SDL2_LIBRARY NAME SDL2 HINTS /usr/lib/${CMAKE_C_LIBRARY_ARCHITECTURE})
    find_library(PORTAUDIO_LIBRARY NAME portaudio HINTS /usr/lib/${CMAKE_C_LIBRARY_ARCHITECTURE})
    find_library(VORBIS_LIBRARY NAME vorbis HINTS /usr/lib/${CMAKE_C_LIBRARY_ARCHITECTURE})
    find_library(VORBIS_FILE_LIBRARY NAME vorbisfile HINTS /usr/lib/${CMAKE_C_LIBRARY_ARCHITECTURE})
endif()

add_executable(visuals ${FILES})

if (NOT WIN32)
    target_link_libraries(visuals
        ${CMAKE_DL_LIBS}
        ${CMAKE_THREAD_LIBS_INIT}
	${ATOMIC_LIBRARY}
        ${PORTAUDIO_LIBRARY}
        ${VORBIS_LIBRARY}
        ${VORBIS_FILE_LIBRARY}
        ${SDL2_LIBRARY})
endif()
