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
                       src/SensorsUpdater.c
                       src/Effectors.c
                       src/EffectorsMonitor.c
                       src/EffectorsUpdater.c
                       src/WaitFreeReadData.c
                       src/SensorsMonitor.c
                       src/SensorReaders.c
                       src/PressureSim.c
                       src/Simulator.c
                       src/TemperatureController.c
                       src/WaterFlowController.c
                       src/Controller.c)

target_link_libraries(project vxworks)
