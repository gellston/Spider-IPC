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
		.delay(10)
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



	while (true) {

		int argument1 = dis(gen);
		int argument2 = dis(gen);


		std::cout << "argument1 = " << argument1 << std::endl;
		std::cout << "argument2 = " << argument2 << std::endl;

		notifier
			.args()
			.push<int>("argument1", argument1)
			.push<int>("argument2", argument2);
		

		notifier();


		int returnValue = 0;
		notifier
			.returns()
			.get("returnValue", &returnValue);


		std::cout << "return Value = " << returnValue << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		//_sleep(100);
	}
	


	return 0;
}