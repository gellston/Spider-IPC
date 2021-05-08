#include "SpiderIPC.h"


#include <thread>
#include <chrono>   

#include <iostream>
#include <stdio.h>

#include <cstdlib>
#include <ctime>
#include <random>

#pragma warning(disable:4996) 

int main() {

	spider::function notifier("test");
	notifier
		.args()
		.arg<int>("argument1")
		.arg<int>("argument2")
		.returns()
		.ret<int>("returnValue")
		.complete();


	// 시드값을 얻기 위한 random_device 생성.
	std::random_device rd;

	// random_device 를 통해 난수 생성 엔진을 초기화 한다.
	std::mt19937 gen(rd());

	// 0 부터 99 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
	std::uniform_int_distribution<int> dis(0, 99);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	int count = 0;
	while (true) {
		try {
			//begin = std::chrono::steady_clock::now();
			

			int argument1 = dis(gen);
			int argument2 = dis(gen);
			int returnValue = 0;

			//std::cout << "argument1 = " << argument1 << std::endl;
			//std::cout << "argument2 = " << argument2 << std::endl;
			notifier
				.args()
				//.lock()
				.push<int>("argument1", argument1)
				.push<int>("argument2", argument2);
				//.unlock();

			notifier();

			
			notifier
				.returns()
				//.lock()
				.get("returnValue", &returnValue);
				//.unlock();

			//std::cout << "return Value = " << returnValue << std::endl;

			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			count++;
			if (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() > 1000) {
				std::cout << "fps = " << count << std::endl;
				begin = std::chrono::steady_clock::now();
				count = 0;

			}
		}
		catch (std::exception e) {
			std::cout << e.what() << std::endl;
		}

	}
	


	return 0;
}