
#include "SpiderIPC.h"
#include <thread>
#include <chrono>   

#include <iostream>

#pragma warning(disable:4996) 

int main() {

	try {
		
		while (true) {
			
			try {
				int test_array[100 * 100];

				spider::variable<int> test("test", 100 * 100, 10, spider::spider_mode::create, spider::spider_access::read_write);
				
				test.receive(test_array, 100 * 100);

		


				std::cout << "received data" << test_array[0] << std::endl;

			}
			catch (std::exception e) {
				std::cout << e.what() << std::endl;
			}
			//std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;

	}
	



	return 0;
}