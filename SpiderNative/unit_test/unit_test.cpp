// unit_test.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define CATCH_CONFIG_MAIN  

#include <iostream>
#include <SpiderIPC.h>

#include "catch2/catch.hpp"


TEST_CASE("spider variable double test") {
	spider::variable<double> test("test", spider::spider_mode::create, spider::spider_access::read_write);

	REQUIRE(test.name() == "test");
	REQUIRE(test.type() == "double");
	
	test = 5;
	
	double send_value = 0;
	double receive_value = 0;

	test >> receive_value;
	REQUIRE(receive_value == 5);

	test.delay(10)
		.block(false);

	test = 7;

	test >> receive_value;
	REQUIRE(receive_value == 7);


	send_value = 15;
	receive_value = 0;
	test.send(&send_value, 1);
	test >> receive_value;
	REQUIRE(receive_value == 15);
	

	send_value = 50;
	receive_value = 0;
	test = send_value;
	test.receive(&receive_value, 1);
	REQUIRE(receive_value == 50);



	spider::variable<double> array_test("test",500,10, spider::spider_mode::create, spider::spider_access::read_write);
	double send_array[500];
	double receive_array[500];
	for (int index = 0; index < 500; index++)
		send_array[index] = 5;
	for (int index = 0; index < 500; index++)
		receive_array[index] = 0;
	array_test.send(send_array, 500);
	array_test.receive(receive_array, 500);
	bool ArrayValueCheck = false;
	for (int index = 0; index < 500; index++) {
		if (receive_array[index] == 0) {
			ArrayValueCheck = true;
			break;
		}
	}
	REQUIRE(ArrayValueCheck == false);

}

TEST_CASE("spider variable int test") {
	spider::variable<int> test("test", spider::spider_mode::create, spider::spider_access::read_write);

	REQUIRE(test.name() == "test");
	REQUIRE(test.type() == "int");

	test = 5;

	int send_value = 0;
	int receive_value = 0;

	test >> receive_value;
	REQUIRE(receive_value == 5);

	test.delay(10)
		.block(false);

	test = 7;

	test >> receive_value;
	REQUIRE(receive_value == 7);


	send_value = 15;
	receive_value = 0;
	test.send(&send_value, 1);
	test >> receive_value;
	REQUIRE(receive_value == 15);


	send_value = 50;
	receive_value = 0;
	test = send_value;
	test.receive(&receive_value, 1);
	REQUIRE(receive_value == 50);

}

TEST_CASE("spider variable unsigend int test") {
	spider::variable<unsigned int> test("test", spider::spider_mode::create, spider::spider_access::read_write);

	REQUIRE(test.name() == "test");
	REQUIRE(test.type() == "unsigned int");

	test = 5;

	unsigned int send_value = 0;
	unsigned int receive_value = 0;

	test >> receive_value;
	REQUIRE(receive_value == 5);

	test.delay(10)
		.block(false);

	test = 7;

	test >> receive_value;
	REQUIRE(receive_value == 7);


	send_value = 15;
	receive_value = 0;
	test.send(&send_value, 1);
	test >> receive_value;
	REQUIRE(receive_value == 15);


	send_value = 50;
	receive_value = 0;
	test = send_value;
	test.receive(&receive_value, 1);
	REQUIRE(receive_value == 50);

}
