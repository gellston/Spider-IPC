#ifndef SPIDER_IPC_SHARP
#define SPIDER_IPC_SHARP

#include <SpiderIPC.h>

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;


namespace SPIDER {

	enum SPIDER_MODE {
		CREATE = 0,
		OPEN,
	};

	enum SPIDER_ACCESS {
		READ_ONLY,
		READ_WRITE
	};

	enum SPIDER_CALL_MODE {
		SUBSCRIBER,
		NOTIFIER
	};
	

	class Variable {
		private:


	public:
		Variable() {

		}
	};

}

#endif