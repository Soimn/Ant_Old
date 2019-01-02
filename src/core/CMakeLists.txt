target_sources(${TARGET}
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/dll_definitions.h"
		"${CMAKE_CURRENT_LIST_DIR}/compiler_definitions.h"
		"${CMAKE_CURRENT_LIST_DIR}/common.h"
		"${CMAKE_CURRENT_LIST_DIR}/memory.h"
		"${CMAKE_CURRENT_LIST_DIR}/debug.h"
)

include("${CMAKE_CURRENT_LIST_DIR}/utility/${CML}")
include("${CMAKE_CURRENT_LIST_DIR}/memory/${CML}")
include("${CMAKE_CURRENT_LIST_DIR}/debug/${CML}")
