include(src/platform/${PLATFORM_FOLDER}/CMakeLists.txt)
set(OT_PLATFORM_HEADER
	src/platform/window.h
	src/platform/input.h
)
set(OT_PLATFORM_SRC
)
list(APPEND OT_PLATFORM_HEADER ${OT_PLATFORM_WINDOWS_HEADER})
list(APPEND OT_PLATFORM_SRC ${OT_PLATFORM_WINDOWS_SRC})