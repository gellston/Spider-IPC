
#include "SpiderIPC.h"
#include <thread>
#include <chrono>   

#include <iostream>

#pragma warning(disable:4996) 

int main() {

	
	spider::function notifier("test", [&](spider::function * function) {

		std::cout << "notified!!" << std::endl;
	});


	while (true) {
		getchar();
	}



	return 0;
}