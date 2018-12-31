#pragma once

#include "core/dll_definitions.h"

namespace Ant {

struct Application {
	public:
		ANT_API void init ();
		ANT_API void run ();
		ANT_API void force_shutdown ();
};

}
