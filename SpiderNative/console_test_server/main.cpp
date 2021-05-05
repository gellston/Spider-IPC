#include "SpiderIPC.h"


#include <thread>
#include <chrono>   

#include <iostream>
#include <stdio.h>

#include <cstdlib>
#include <ctime>

#pragma warning(disable:4996) 

int main() {

	spider::function notifier("test");


	while (true) {
		getchar();
		notifier();
	}
	


	return 0;
}