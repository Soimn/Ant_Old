target_sources(${TARGET}
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/application.h"
		"${CMAKE_CURRENT_LIST_DIR}/application.cpp"
)

include("${CMAKE_CURRENT_LIST_DIR}/core/${CML}")
