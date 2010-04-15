CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

PROJECT(OpenFala)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-extras/")
SET(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE(SFML REQUIRED)
FIND_PACKAGE(ImageMagick COMPONENTS Magick++ REQUIRED)
FIND_PACKAGE(Boost COMPONENTS filesystem program_options REQUIRED)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${SFML_INCLUDE_DIRS} ${ImageMagick_INCLUDE_DIRS})

AUX_SOURCE_DIRECTORY(src/ OpenFala_SRCS)
AUX_SOURCE_DIRECTORY(src/block/ OpenFala_block_SRCS)
AUX_SOURCE_DIRECTORY(src/client/ OpenFala_client_SRCS)
AUX_SOURCE_DIRECTORY(src/entity/ OpenFala_entity_SRCS)
AUX_SOURCE_DIRECTORY(src/filesystem OpenFala_filesystem_SRCS)
AUX_SOURCE_DIRECTORY(src/gui OpenFala_gui_SRCS)
AUX_SOURCE_DIRECTORY(src/network OpenFala_network_SRCS)
AUX_SOURCE_DIRECTORY(src/particle OpenFala_particle_SRCS)
AUX_SOURCE_DIRECTORY(src/resources OpenFala_resources_SRCS)
AUX_SOURCE_DIRECTORY(src/server OpenFala_server_SRCS)
AUX_SOURCE_DIRECTORY(src/states OpenFala_states_SRCS)
AUX_SOURCE_DIRECTORY(src/utility OpenFala_utility_SRCS)
AUX_SOURCE_DIRECTORY(src/world OpenFala_world_SRCS)

ADD_DEFINITIONS(-Wall)

SET(CMAKE_BUILD_TYPE Debug)

ADD_EXECUTABLE(OpenFala 
    ${OpenFala_SRCS} 
    ${OpenFala_block_SRCS} 
    ${OpenFala_client_SRCS} 
    ${OpenFala_entity_SRCS} 
    ${OpenFala_filesystem_SRCS} 
    ${OpenFala_gui_SRCS} 
    ${OpenFala_network_SRCS}
    ${OpenFala_particle_SRCS} 
    ${OpenFala_resources_SRCS} 
    ${OpenFala_server_SRCS} 
    ${OpenFala_states_SRCS} 
    ${OpenFala_utility_SRCS} 
    ${OpenFala_world_SRCS} 
)
TARGET_LINK_LIBRARIES(OpenFala ${SFML_LIBRARIES} ${Boost_LIBRARIES} ${SFML_LIBRARY} ${ImageMagick_LIBRARIES})
