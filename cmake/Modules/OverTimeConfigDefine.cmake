 #Please use them everywhere
 #WINDOWS   =   Windows Desktop
 #LINUX      =   Linux

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(WINDOWS TRUE)
    set(PLATFORM_FOLDER windows)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LINUX TRUE)
    set(PLATFORM_FOLDER linux)
else()
    message(FATAL_ERROR "Unsupported platform, CMake will exit")
    return()
endif()

# check c/c++ standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enabling Hot Reload for MSVC compilers, if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


macro(res_mgmt EXECUTABLE_NAME FILE_WITH_PATH)
        get_filename_component(DIRECTORY_PATH "${FILE_WITH_PATH}" DIRECTORY)

        #message("BUIDING FOR ${EXECUTABLE_NAME}: ${FILE_WITH_PATH} FROM ${CMAKE_CURRENT_LIST_DIR}/${DIRECTORY_PATH} TO ${CMAKE_BINARY_DIR}/bin/${DIRECTORY_PATH}")
        add_custom_command(
            TARGET ${EXECUTABLE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin/${DIRECTORY_PATH}"
		    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/${FILE_WITH_PATH}" "${CMAKE_BINARY_DIR}/bin/${DIRECTORY_PATH}"
		    DEPENDS ${FILE_WITH_PATH}
            COMMENT "COPYING FOR ${EXECUTABLE_NAME}: ${FILE_WITH_PATH} FROM ${CMAKE_CURRENT_LIST_DIR}/${DIRECTORY_PATH} TO ${CMAKE_BINARY_DIR}/bin/${DIRECTORY_PATH}"
	    )
endmacro()