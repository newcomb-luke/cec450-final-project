cmake_minimum_required(VERSION 3.15)

project(project)

include_directories(include ../vxworks/include)

add_executable(project src/main.c 
                       src/vector.c 
                       src/array.c 
                       src/Visualizer.c 
                       src/DisplayManager.c 
                       src/VisualizerMessage.c
                       src/LogMessage.c
                       src/Sensors.c
                       src/SensorsUpdater.c)

target_link_libraries(project vxworks)
