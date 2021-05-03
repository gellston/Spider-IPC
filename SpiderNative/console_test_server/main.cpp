#include "SpiderIPC.h"


#include <thread>
#include <chrono>   

#include <iostream>
#include <stdio.h>

#include <cstdlib>
#include <ctime>

#pragma warning(disable:4996) 

int main() {

	spider::variable<int> test("test", 100 * 100, 10);
	int test_array[100 * 100];
	while (true) {
		try {
			int random_variable = std::rand();
			memset(test_array, 0, 100 * 100 * sizeof(int));
			for (int index = 0; index < 10000; index++)
				test_array[index] = random_variable;
			std::cout << "sent data = " << random_variable << std::endl;
			test.send(test_array, 100 * 100);
		}
		catch (std::exception e) {
			std::cout << e.what() << std::endl;
		}
	}

	return 0;
}