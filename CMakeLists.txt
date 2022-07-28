cmake_minimum_required (VERSION 3.16)

# project name
project (
  tslpp 
  LANGUAGES CXX
  VERSION "0.0.1"
  DESCRIPTION "A simple modern-c++ serialization library")

# project files
set (MODULE_FILES mod/error.ixx
                  mod/buffer.ixx
                  mod/converters/integer.ixx
                  mod/converters/converters.ixx
                  mod/io/basic_reader.ixx
                  mod/io/basic_writer.ixx
                  mod/io/io.ixx
                  mod/tslpp.ixx)

# targets
add_library (${PROJECT_NAME} STATIC ${MODULE_FILES})

# target options
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)