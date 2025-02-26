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

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()