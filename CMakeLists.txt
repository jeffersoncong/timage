CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(libtimage)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/sample)
SET(LIBTIMAGE_SRC cv_timage.c gm_timage.c lua_timage.c)

# lua5.1
FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(LUA5.1 REQUIRED lua5.1)

# GraphicsMagick cppflags
# eg: -I/usr/local/include/GraphicsMagick
EXEC_PROGRAM(GraphicsMagick-config
	ARGS "--cppflags" 
	OUTPUT_VARIABLE GM_CPPFLAGS_VARIABLE
	RETURN_VALUE GM_CPPFLAGS_RET)

string(REPLACE "-I" "" GM_CPPFLAGS_VARIABLE ${GM_CPPFLAGS_VARIABLE})

IF(NOT GM_CPPFLAGS_RET)
	MESSAGE(STATUS "GraphicsMagick cppflags: " ${GM_CPPFLAGS_VARIABLE})
ELSE(NOT GM_CPPFLAGS_RET)
	MESSAGE(FATAL_ERROR "GraphicsMagick cppflags fail")
ENDIF(NOT GM_CPPFLAGS_RET)

# GraphicsMagick --ldflags
# eg: -L/usr/local/lib -L/home/dfs/install-env/jpeg/src/ -L/usr/lib -L/usr/lib
EXEC_PROGRAM(GraphicsMagick-config
	ARGS "--ldflags"
	OUTPUT_VARIABLE GM_LDFLAGS_VARIABLE
	RETURN_VALUE GM_LDFLAGS_RET)

string(REPLACE "-L" "" GM_LDFLAGS_VARIABLE ${GM_LDFLAGS_VARIABLE})

IF(NOT GM_LDFLAGS_RET)
	MESSAGE(STATUS "GraphicsMagick ldflags: " ${GM_LDFLAGS_VARIABLE})
ELSE(NOT GM_LDFLAGS_RET)
	MESSAGE(FATAL_ERROR "GraphicsMagick ldflags fail")
ENDIF(NOT GM_LDFLAGS_RET)

# GraphicsMagick --libs
# eg: -lGraphicsMagick -llcms -ltiff -lfreetype -ljasper -ljpeg -lpng -lwmflite -lXext -lSM -lICE -lX11 -lbz2 -lxml2 -lz -lm -lgomp -lpthread
EXEC_PROGRAM(GraphicsMagick-config
	ARGS "--libs"
	OUTPUT_VARIABLE GM_LIBS_VARIABLE
	RETURN_VALUE GM_LIBS_RET)

IF(NOT GM_LIBS_RET)
	MESSAGE(STATUS "GraphicsMagick libs: " ${GM_LIBS_VARIABLE})
ELSE(NOT GM_LIBS_RET)
	MESSAGE(FATAL_ERROR "GraphicsMagick libs fail")
ENDIF(NOT GM_LIBS_RET)

#string(REPLACE "-L" "" GM_LDFLAGS_VARIABLE ${GM_LDFLAGS_VARIABLE})

# Header files
INCLUDE_DIRECTORIES(${LUA5.1_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${GM_CPPFLAGS_VARIABLE})

# Libs
LINK_DIRECTORIES(${LUA5.1_LIBRARY_DIRS})
LINK_DIRECTORIES(${GM_LDFLAGS_VARIABLE})

# Libtimage.so
ADD_LIBRARY(timage STATIC ${LIBTIMAGE_SRC})

# gm_test
ADD_EXECUTABLE(gm_test ./sample/gm_test.c ${LIBTIMAGE_SRC})
TARGET_LINK_LIBRARIES(gm_test -llua5.1)
TARGET_LINK_LIBRARIES(gm_test ${GM_LIBS_VARIABLE})

