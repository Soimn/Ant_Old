target_sources(${TARGET}
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/dll_definitions.h"
		"${CMAKE_CURRENT_LIST_DIR}/common.h"
)

include("${CMAKE_CURRENT_LIST_DIR}/utility/${CML}")
include("${CMAKE_CURRENT_LIST_DIR}/memory/${CML}")
