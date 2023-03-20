# @file CMakeLists.txt
# 
# CMake files for the srcFacts project

# Can be run by any version of CMake >= this
cmake_minimum_required(VERSION 3.25)

# project info
project(srcFacts)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use Release build type if not specified
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# srcfacts application
add_executable(srcfacts)

# srcfacts sources
target_sources(srcfacts PRIVATE srcFacts.cpp refillContent.cpp XMLParser.cpp xml_parser.cpp)

# cmake . -DTRACE=ON|OFF
if(DEFINED TRACE)
    message(STATUS "TRACE is ${TRACE}")
    if(TRACE)
        target_compile_definitions(srcfacts PUBLIC TRACE)
    endif()
endif()

# Setup optional bigdata
set(BIGDATA_FILENAME "linux-6.0.xml")
set(DATA_DIR "${CMAKE_CURRENT_BINARY_DIR}/data")
include(FetchContent)
FetchContent_Declare(bigdata
    URL "http://131.123.42.38/build/${BIGDATA_FILENAME}.zip"
    DOWNLOAD_DIR ${DATA_DIR}
    SOURCE_DIR ${DATA_DIR}
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    URL_HASH MD5=447a16ebd4b54e27e157576a214e0ba4
)
if(DEFINED DOWNLOAD_BIGDATA)
    if(DOWNLOAD_BIGDATA)
        message(STATUS "DOWNLOAD_BIGDATA is ${DOWNLOAD_BIGDATA}")
        if(NOT DEFINED DOWNLOAD_BIGDATA_CONFIRM)
            message(WARNING "The BIGDATA file (${BIGDATA_FILENAME}) is over 4 GB when extracted. "
                "Make sure you have enough disk space before continuing.\n"
                "To perform the download, set the flag DOWNLOAD_BIGDATA_CONFIRM:\n"
                "    cmake . -DDOWNLOAD_BIGDATA_CONFIRM=ON\n"
                "To remove this warning, either set DOWNLOAD_BIGDATA to OFF:\n"
                "    cmake . -DDOWNLOAD_BIGDATA=OFF\n"
                "or set the confirmation to OFF:\n"
                "    cmake . -DDOWNLOAD_BIGDATA_CONFIRM=OFF"
                )
        elseif(DOWNLOAD_BIGDATA_CONFIRM)
            message(STATUS "DOWNLOAD_BIGDATA_CONFIRM is ${DOWNLOAD_BIGDATA_CONFIRM}")
            FetchContent_GetProperties(bigdata)
            if (NOT bigdata_POPULATED)
                message(STATUS "Downloading ${BIGDATA_FILENAME}.zip")
                if(NOT DEFINED DOWNLOAD_BIGDATA_QUIET OR NOT DOWNLOAD_BIGDATA_QUIET)
                    set(FETCHCONTENT_QUIET OFF)
                endif()
                FetchContent_Populate(bigdata)
                FetchContent_GetProperties(bigdata)
                set(FETCHCONTENT_QUIET ON)
                add_custom_target(run_bigdata
                    COMMAND $<TARGET_FILE:srcfacts> < ${bigdata_SOURCE_DIR}/${BIGDATA_FILENAME}
                    USES_TERMINAL
                )
                add_custom_target(clean_bigdata
                    COMMAND ${CMAKE_COMMAND} -E rm -f ${bigdata_SOURCE_DIR}/${BIGDATA_FILENAME}
                    COMMAND ${CMAKE_COMMAND} -E echo "Set DOWNLOAD_BIGDATA to OFF or cmake may download and extract it again"
                    USES_TERMINAL
                )
            endif()
        else()
            message(STATUS "DOWNLOAD_BIGDATA_CONFIRM is ${DOWNLOAD_BIGDATA_CONFIRM}")
        endif()
    endif()
endif()

# Turn on warnings
target_compile_options(srcfacts PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Wall>
     $<$<CXX_COMPILER_ID:MSVC>: /W4>
)

# Extract the demo input srcML file into the data directory
file(ARCHIVE_EXTRACT INPUT ${CMAKE_SOURCE_DIR}/demo.xml.zip DESTINATION ${DATA_DIR})

# Demo run command
add_custom_target(run
        COMMENT "Run demo"
        COMMAND $<TARGET_FILE:srcfacts> < ${DATA_DIR}/demo.xml
        DEPENDS srcfacts
        USES_TERMINAL
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# xmlstats application
add_executable(xmlstats)

# xmlstats sources
target_sources(xmlstats PRIVATE xmlstats.cpp XMLParser.cpp refillContent.cpp xml_parser.cpp)

# Turn on warnings
target_compile_options(xmlstats PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Wall>
     $<$<CXX_COMPILER_ID:MSVC>: /W4>
)

# xmlstats run command
add_custom_target(run_xmlstats
        COMMENT "Run xmlstats"
        COMMAND $<TARGET_FILE:xmlstats> < ${DATA_DIR}/demo.xml
        DEPENDS xmlstats
        USES_TERMINAL
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# identity application
add_executable(identity)

# identity sources
target_sources(identity PRIVATE identity.cpp XMLParser.cpp refillContent.cpp xml_parser.cpp)

# Turn on warnings
target_compile_options(identity PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Wall>
     $<$<CXX_COMPILER_ID:MSVC>: /W4>
)

# identity run output file
set(IDENTITY_OUTPUT_FILE ${CMAKE_BINARY_DIR}/democopy.xml)

# identity run command
add_custom_target(run_identity
        COMMENT "Run identity"
        COMMAND $<TARGET_FILE:identity> < ${DATA_DIR}/demo.xml > ${IDENTITY_OUTPUT_FILE}
        DEPENDS identity
        USES_TERMINAL
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Add the generated identity output file to the clean target
set_property(
        TARGET identity
        APPEND
        PROPERTY ADDITIONAL_CLEAN_FILES ${IDENTITY_OUTPUT_FILE}
)

# identity run command output
add_custom_target(run_identity_check
        COMMENT "Run identity and compare input and output"
        COMMAND "${CMAKE_COMMAND}" -E compare_files --ignore-eol "${DATA_DIR}/demo.xml" "${IDENTITY_OUTPUT_FILE}" && echo "Files are identical" || echo "Files are not identical"
        DEPENDS run_identity
        USES_TERMINAL
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# identity run command with difference output
# Note: May have a large amount of output
add_custom_target(run_identity_diff
        COMMENT "Run identity and compare input and output"
        COMMAND diff ${DATA_DIR}/demo.xml ${IDENTITY_OUTPUT_FILE}
        DEPENDS run_identity
        USES_TERMINAL
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
