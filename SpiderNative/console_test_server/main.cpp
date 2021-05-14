#include "SpiderIPC.h"
#include <iostream>

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
	while (true) {
		
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
		}
		catch (std::exception e) {
			std::cout << e.what() << std::endl;
		}
	}
	return 0;
}