
file(GLOB ALL_CPP_FILES "*.cpp")
file(GLOB ALL_H_FILES "*.h")


# change this later
# message(STATUS "Adding LIBRARY : " ${lib_name} " from " ${PROJECT_SOURCE_DIR}/src)
add_library(Helpers ${ALL_CPP_FILES} ${ALL_H_FILES}) # OBJECT to create object files not tied to a library to save space

# recursive (Needed if there are sub libraries below this current dir)
set (ALL_OBJECT_FILES ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:Helpers> PARENT_SCOPE)
