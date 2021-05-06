
#include "SpiderIPC.h"
#include <thread>
#include <chrono>   

#include <iostream>

#pragma warning(disable:4996) 

int main() {

	

	spider::function notifier("test", [&](spider::function* function) {

		int argument1 = 0;
		int argument2 = 0;


		function->args()
			.get("argument1", &argument1)
			.get("argument2", &argument2);
			


		int result = argument1 + argument2;
		std::cout << "calculated value = " << result << std::endl;
		function->returns()
			.push("returnValue", result);
			

	});

	notifier
		.delay(10)
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