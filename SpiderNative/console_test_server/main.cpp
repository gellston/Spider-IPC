#include "SpiderIPC.h"
#include <iostream>
#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

/// <summary>
/// Server Side
/// </summary>
/// 
int main() {

	spider::function notifier("test");
	notifier
		.args()
		.arg<int>("argument1")
		.arg<int>("argument2")
		.returns()
		.ret<int>("returnValue")
		.complete();

	auto t1 = high_resolution_clock::now();
	int count = 0; 
	while (true) {
		count++;
		try {
			int argument1 = 11;
			int argument2 = 11;
			int returnValue = 0;

			notifier
				.args()
				.push<int>("argument1", argument1)
				.push<int>("argument2", argument2);

			notifier();

			notifier
				.returns()
				.get("returnValue", &returnValue);
			auto t2 = high_resolution_clock::now();
			auto ms_int = duration_cast<milliseconds>(t2 - t1);


			duration<double, std::milli> ms_double = t2 - t1;
			if (ms_double.count() > 1000) {
				
				std::cout << "calculated value from server = " << returnValue << std::endl;
				std::cout << "count per sec = " << count << std::endl;
				count = 0;
				t1 = high_resolution_clock::now();
			}
		}
		catch (std::exception e) {
			std::cout << e.what() << std::endl;
		}

	}


	return 0;
}