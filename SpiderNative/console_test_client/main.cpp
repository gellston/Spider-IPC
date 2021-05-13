
#include "SpiderIPC.h"
#include <thread>
#include <chrono>   

#include <iostream>

#pragma warning(disable:4996) 

int main() {

	std::atomic<int> count{ 500 };

	spider::function notifier("test", [&](spider::function* function) {

		try {
			int argument1 = 0;
			int argument2 = 0;


			function->args()
				.get("argument1", &argument1)
				.get("argument2", &argument2);

			int result = argument1 + argument2;

			function->returns()
				.push("returnValue", result);

		}
		catch (std::exception e) {
			std::cout << e.what() << std::endl;
		}

	});

	notifier
		.args()
		.arg<int>("argument1")
		.arg<int>("argument2")
		.returns()
		.ret<int>("returnValue")
		.complete();


	while (true) {
		getchar();
	}



	return 0;
}