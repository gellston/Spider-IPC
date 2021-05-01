
#include "SpiderIPC.h"
#include <thread>
#include <chrono>   

#include <iostream>

#pragma warning(disable:4996) 

int main() {

	spider::variable<char> test("test", 500, 100);

	char temp[500];

	while (true) {
		try {
			memset(temp, 0, sizeof(temp));
			test.delay(10).receive(temp, 500);
			std::cout << temp << std::endl;
			
		}
		catch (std::exception e) {
			std::cout << e.what() << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}