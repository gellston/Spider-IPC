#include "SpiderIPC.h"

#include <Windows.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <functional>
#include <unordered_map>

namespace spider {
	class spider_pimpl {
	public:
		// variable pimpl
		HANDLE	shmem;
		HANDLE	event_write_handle;
		
		// function
		HANDLE  function_start_handle;
		HANDLE  function_end_handle;
		HANDLE  function_blocker;
		std::unordered_map<std::string, std::shared_ptr<Ivariable>> arguments;
		std::unordered_map<std::string, std::shared_ptr<Ivariable>> return_list;


		spider_pimpl() {
			this->shmem = INVALID_HANDLE_VALUE;
			this->event_write_handle = INVALID_HANDLE_VALUE;
			this->function_start_handle = INVALID_HANDLE_VALUE;
			this->function_end_handle = INVALID_HANDLE_VALUE;
			this->function_blocker = INVALID_HANDLE_VALUE;
		}
	};


	class spider_raii {
	private:
		std::function<void()> __instance;
	public:
		spider_raii(std::function<void()> lambda) {
			__instance = lambda;
		}
		~spider_raii() {
			this->__instance();
		}
	};
};



//Double variable
#undef native_type
#define native_type double
spider::variable<native_type>::variable(std::string name, spider_mode mode, spider_access access) : instance(nullptr),
																								    pimpl(new spider_pimpl()),
																						            type_name(typeid(native_type).name()){


	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	this->_name = name;

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->access = access;


	this->pimpl->event_write_handle = CreateEvent(
								     NULL,               // default security attributes
								     TRUE,               // manual-reset event
								     TRUE,              // initial state is nonsignaled
									 unicode_write_event_name.c_str()  // object name
								     );

	switch (mode)
	{
	case spider::create:
		this->pimpl->shmem = ::CreateFileMapping(
								INVALID_HANDLE_VALUE,
								NULL,
								PAGE_READWRITE,
								0,
								sizeof(native_type),
								unicode_name.c_str());
		break;
	case spider::open:
		this->pimpl->shmem = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, unicode_name.c_str());
		break;
	default:
		throw std::exception("Invalid mode");
		break;
	}

	if (this->pimpl->shmem == nullptr)
		throw std::exception("Invalid shared memory handle");

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));

	if (this->instance == nullptr) {
		CloseHandle(this->pimpl->shmem);
		throw std::exception("Invalid memory pointer");
	}
		

	this->delay_time = 100;
	this->element_count = 1;
	this->is_block = true;
}

spider::variable<native_type>::variable(std::string name, unsigned int element_count, unsigned int delay, spider_mode mode, spider_access access) : instance(nullptr),
																																					pimpl(new spider_pimpl()),
																																	                type_name(typeid(native_type).name()) {
	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	this->_name = name;

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->access = access;


	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	switch (mode)
	{
	case spider::create:
		this->pimpl->shmem = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(native_type) * element_count,
			unicode_name.c_str());
		break;
	case spider::open:
		this->pimpl->shmem = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, unicode_name.c_str());
		break;
	default:
		throw std::exception("Invalid mode");
		break;
	}

	if (this->pimpl->shmem == nullptr)
		throw std::exception("Invalid shared memory handle");

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type) * element_count);

	if (this->instance == nullptr) {
		CloseHandle(this->pimpl->shmem);
		throw std::exception("Invalid memory pointer");
	}

	this->delay_time = delay;
	this->element_count = element_count;
	this->is_block = true;
}

spider::variable<native_type>::variable(std::string name, unsigned int delay, spider_mode mode, spider_access access) : instance(nullptr),
																														pimpl(new spider_pimpl()),
																														type_name(typeid(native_type).name()){

	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	this->_name = name;

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->access = access;


	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	switch (mode)
	{
	case spider::create:
		this->pimpl->shmem = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(native_type),
			unicode_name.c_str());
		break;
	case spider::open:
		this->pimpl->shmem = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, unicode_name.c_str());
		break;
	default:
		throw std::exception("Invalid mode");
		break;
	}

	if (this->pimpl->shmem == nullptr)
		throw std::exception("Invalid shared memory handle");

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));

	if (this->instance == nullptr) {
		CloseHandle(this->pimpl->shmem);
		throw std::exception("Invalid memory pointer");
	}

	this->delay_time = delay;
	this->element_count = 1;
	this->is_block = true;
}

std::string spider::variable<native_type>::type() {
	return this->type_name;
}

std::string spider::variable<native_type>::name() {
	return this->_name;
}

spider::variable<native_type>::~variable() {
	if(this->instance)
		::UnmapViewOfFile(this->instance);
	if (this->pimpl->shmem)
		::CloseHandle(this->pimpl->shmem);
	if (this->pimpl->event_write_handle)
		CloseHandle(this->pimpl->event_write_handle);
}


spider::variable<native_type>&  spider::variable<native_type>::block(bool is_block) {
	this->is_block = is_block;

	return *this;
}

void spider::variable<native_type>::operator=(native_type data) {

	if (this->access == spider_access::read_only)
		throw std::exception("Proteced memory");

	if (this->is_block == true) {
		spider_raii raii([&] {
			SetEvent(this->pimpl->event_write_handle);
		});

		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);
		ResetEvent(this->pimpl->event_write_handle);

		switch (write_handle_ret) {
		case WAIT_OBJECT_0:
			memcpy(this->instance, &data, sizeof(native_type));
			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		memcpy(this->instance, &data, sizeof(native_type));
	}

}

void spider::variable<native_type>::operator>>(native_type& data) {
	if (this->is_block == true) {
		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);
		native_type value = 0;
		switch (write_handle_ret) {
		case WAIT_OBJECT_0:
			memcpy(&value, this->instance, sizeof(native_type));
			data = value;
			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		native_type value = 0;
		memcpy(&value, this->instance, sizeof(native_type));
		data = value;
	}

}

spider::variable<native_type> & spider::variable<native_type>::delay(unsigned int delay) {
	this->delay_time = delay;
	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::receive(native_type* data, unsigned int element_count) {

	if (this->is_block == true) {
		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);

		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;

		switch (write_handle_ret) {
		case WAIT_OBJECT_0:


			if (source_count < target_count)
				throw std::exception("Invalid size error");

			memcpy(data, this->instance, sizeof(native_type) * element_count);
			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;
		if (source_count < target_count)
			throw std::exception("Invalid size error");

		memcpy(data, this->instance, sizeof(native_type) * element_count);
	}


	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::send(native_type* data, unsigned int element_count) {

	if (this->access == spider_access::read_only)
		throw std::exception("Proected memory");


	if (this->is_block == true) {
		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);

		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;

		switch (write_handle_ret) {
		case WAIT_OBJECT_0:


			if (source_count < target_count)
				throw std::exception("Invalid size error");

			memcpy(this->instance, data, sizeof(native_type) * element_count);

			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;

		if (source_count < target_count)
			throw std::exception("Invalid size error");

		memcpy(this->instance, data, sizeof(native_type) * element_count);
	}

	
	return (*this);
}






//Int variable
#undef native_type
#define native_type int
spider::variable<native_type>::variable(std::string name, spider_mode mode, spider_access access) : instance(nullptr),
pimpl(new spider_pimpl()),
type_name(typeid(native_type).name()) {


	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	this->_name = name;

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->access = access;


	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	switch (mode)
	{
	case spider::create:
		this->pimpl->shmem = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(native_type),
			unicode_name.c_str());
		break;
	case spider::open:
		this->pimpl->shmem = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, unicode_name.c_str());
		break;
	default:
		throw std::exception("Invalid mode");
		break;
	}

	if (this->pimpl->shmem == nullptr)
		throw std::exception("Invalid shared memory handle");

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));

	if (this->instance == nullptr) {
		CloseHandle(this->pimpl->shmem);
		throw std::exception("Invalid memory pointer");
	}


	this->delay_time = 100;
	this->element_count = 1;
	this->is_block = true;
}

spider::variable<native_type>::variable(std::string name, unsigned int element_count, unsigned int delay, spider_mode mode, spider_access access) : instance(nullptr),
pimpl(new spider_pimpl()),
type_name(typeid(native_type).name()) {
	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	this->_name = name;

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->access = access;


	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	switch (mode)
	{
	case spider::create:
		this->pimpl->shmem = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(native_type) * element_count,
			unicode_name.c_str());
		break;
	case spider::open:
		this->pimpl->shmem = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, unicode_name.c_str());
		break;
	default:
		throw std::exception("Invalid mode");
		break;
	}

	if (this->pimpl->shmem == nullptr)
		throw std::exception("Invalid shared memory handle");

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type) * element_count);

	if (this->instance == nullptr) {
		CloseHandle(this->pimpl->shmem);
		throw std::exception("Invalid memory pointer");
	}

	this->delay_time = delay;
	this->element_count = element_count;
	this->is_block = true;
}

spider::variable<native_type>::variable(std::string name, unsigned int delay, spider_mode mode, spider_access access) : instance(nullptr),
pimpl(new spider_pimpl()),
type_name(typeid(native_type).name()) {

	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	this->_name = name;

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->access = access;


	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	switch (mode)
	{
	case spider::create:
		this->pimpl->shmem = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(native_type),
			unicode_name.c_str());
		break;
	case spider::open:
		this->pimpl->shmem = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, unicode_name.c_str());
		break;
	default:
		throw std::exception("Invalid mode");
		break;
	}

	if (this->pimpl->shmem == nullptr)
		throw std::exception("Invalid shared memory handle");

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));

	if (this->instance == nullptr) {
		CloseHandle(this->pimpl->shmem);
		throw std::exception("Invalid memory pointer");
	}

	this->delay_time = delay;
	this->element_count = 1;
	this->is_block = true;
}

std::string spider::variable<native_type>::type() {
	return this->type_name;
}

std::string spider::variable<native_type>::name() {
	return this->_name;
}

spider::variable<native_type>::~variable() {
	if (this->instance)
		::UnmapViewOfFile(this->instance);
	if (this->pimpl->shmem)
		::CloseHandle(this->pimpl->shmem);
	if (this->pimpl->event_write_handle)
		CloseHandle(this->pimpl->event_write_handle);
}


spider::variable<native_type>& spider::variable<native_type>::block(bool is_block) {
	this->is_block = is_block;

	return *this;
}

void spider::variable<native_type>::operator=(native_type data) {

	if (this->access == spider_access::read_only)
		throw std::exception("Proteced memory");

	if (this->is_block == true) {
		spider_raii raii([&] {
			SetEvent(this->pimpl->event_write_handle);
			});

		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);
		ResetEvent(this->pimpl->event_write_handle);

		switch (write_handle_ret) {
		case WAIT_OBJECT_0:
			memcpy(this->instance, &data, sizeof(native_type));
			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		memcpy(this->instance, &data, sizeof(native_type));
	}

}

void spider::variable<native_type>::operator>>(native_type& data) {
	if (this->is_block == true) {
		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);
		native_type value = 0;
		switch (write_handle_ret) {
		case WAIT_OBJECT_0:
			memcpy(&value, this->instance, sizeof(native_type));
			data = value;
			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		native_type value = 0;
		memcpy(&value, this->instance, sizeof(native_type));
		data = value;
	}

}

spider::variable<native_type>& spider::variable<native_type>::delay(unsigned int delay) {
	this->delay_time = delay;
	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::receive(native_type* data, unsigned int element_count) {

	if (this->is_block == true) {
		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);

		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;

		switch (write_handle_ret) {
		case WAIT_OBJECT_0:


			if (source_count < target_count)
				throw std::exception("Invalid size error");

			memcpy(data, this->instance, sizeof(native_type) * element_count);
			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;
		if (source_count < target_count)
			throw std::exception("Invalid size error");

		memcpy(data, this->instance, sizeof(native_type) * element_count);
	}


	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::send(native_type* data, unsigned int element_count) {

	if (this->access == spider_access::read_only)
		throw std::exception("Proected memory");


	if (this->is_block == true) {
		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);

		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;

		switch (write_handle_ret) {
		case WAIT_OBJECT_0:


			if (source_count < target_count)
				throw std::exception("Invalid size error");

			memcpy(this->instance, data, sizeof(native_type) * element_count);

			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;

		if (source_count < target_count)
			throw std::exception("Invalid size error");

		memcpy(this->instance, data, sizeof(native_type) * element_count);
	}


	return (*this);
}



//Unsigned int variable
#undef native_type
#define native_type unsigned int
spider::variable<native_type>::variable(std::string name, spider_mode mode, spider_access access) : instance(nullptr),
pimpl(new spider_pimpl()),
type_name(typeid(native_type).name()) {


	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	this->_name = name;

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->access = access;


	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	switch (mode)
	{
	case spider::create:
		this->pimpl->shmem = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(native_type),
			unicode_name.c_str());
		break;
	case spider::open:
		this->pimpl->shmem = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, unicode_name.c_str());
		break;
	default:
		throw std::exception("Invalid mode");
		break;
	}

	if (this->pimpl->shmem == nullptr)
		throw std::exception("Invalid shared memory handle");

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));

	if (this->instance == nullptr) {
		CloseHandle(this->pimpl->shmem);
		throw std::exception("Invalid memory pointer");
	}


	this->delay_time = 100;
	this->element_count = 1;
	this->is_block = true;
}

spider::variable<native_type>::variable(std::string name, unsigned int element_count, unsigned int delay, spider_mode mode, spider_access access) : instance(nullptr),
pimpl(new spider_pimpl()),
type_name(typeid(native_type).name()) {
	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	this->_name = name;

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->access = access;


	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	switch (mode)
	{
	case spider::create:
		this->pimpl->shmem = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(native_type) * element_count,
			unicode_name.c_str());
		break;
	case spider::open:
		this->pimpl->shmem = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, unicode_name.c_str());
		break;
	default:
		throw std::exception("Invalid mode");
		break;
	}

	if (this->pimpl->shmem == nullptr)
		throw std::exception("Invalid shared memory handle");

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type) * element_count);

	if (this->instance == nullptr) {
		CloseHandle(this->pimpl->shmem);
		throw std::exception("Invalid memory pointer");
	}

	this->delay_time = delay;
	this->element_count = element_count;
	this->is_block = true;
}

spider::variable<native_type>::variable(std::string name, unsigned int delay, spider_mode mode, spider_access access) : instance(nullptr),
pimpl(new spider_pimpl()),
type_name(typeid(native_type).name()) {

	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	this->_name = name;

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->access = access;


	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	switch (mode)
	{
	case spider::create:
		this->pimpl->shmem = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(native_type),
			unicode_name.c_str());
		break;
	case spider::open:
		this->pimpl->shmem = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, unicode_name.c_str());
		break;
	default:
		throw std::exception("Invalid mode");
		break;
	}

	if (this->pimpl->shmem == nullptr)
		throw std::exception("Invalid shared memory handle");

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));

	if (this->instance == nullptr) {
		CloseHandle(this->pimpl->shmem);
		throw std::exception("Invalid memory pointer");
	}

	this->delay_time = delay;
	this->element_count = 1;
	this->is_block = true;
}

std::string spider::variable<native_type>::type() {
	return this->type_name;
}

std::string spider::variable<native_type>::name() {
	return this->_name;
}

spider::variable<native_type>::~variable() {
	if (this->instance)
		::UnmapViewOfFile(this->instance);
	if (this->pimpl->shmem)
		::CloseHandle(this->pimpl->shmem);
	if (this->pimpl->event_write_handle)
		CloseHandle(this->pimpl->event_write_handle);
}


spider::variable<native_type>& spider::variable<native_type>::block(bool is_block) {
	this->is_block = is_block;

	return *this;
}

void spider::variable<native_type>::operator=(native_type data) {

	if (this->access == spider_access::read_only)
		throw std::exception("Proteced memory");

	if (this->is_block == true) {
		spider_raii raii([&] {
			SetEvent(this->pimpl->event_write_handle);
			});

		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);
		ResetEvent(this->pimpl->event_write_handle);

		switch (write_handle_ret) {
		case WAIT_OBJECT_0:
			memcpy(this->instance, &data, sizeof(native_type));
			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		memcpy(this->instance, &data, sizeof(native_type));
	}

}

void spider::variable<native_type>::operator>>(native_type& data) {
	if (this->is_block == true) {
		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);
		native_type value = 0;
		switch (write_handle_ret) {
		case WAIT_OBJECT_0:
			memcpy(&value, this->instance, sizeof(native_type));
			data = value;
			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		native_type value = 0;
		memcpy(&value, this->instance, sizeof(native_type));
		data = value;
	}

}

spider::variable<native_type>& spider::variable<native_type>::delay(unsigned int delay) {
	this->delay_time = delay;
	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::receive(native_type* data, unsigned int element_count) {

	if (this->is_block == true) {
		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);

		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;

		switch (write_handle_ret) {
		case WAIT_OBJECT_0:


			if (source_count < target_count)
				throw std::exception("Invalid size error");

			memcpy(data, this->instance, sizeof(native_type) * element_count);
			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;
		if (source_count < target_count)
			throw std::exception("Invalid size error");

		memcpy(data, this->instance, sizeof(native_type) * element_count);
	}


	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::send(native_type* data, unsigned int element_count) {

	if (this->access == spider_access::read_only)
		throw std::exception("Proected memory");


	if (this->is_block == true) {
		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);

		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;

		switch (write_handle_ret) {
		case WAIT_OBJECT_0:


			if (source_count < target_count)
				throw std::exception("Invalid size error");

			memcpy(this->instance, data, sizeof(native_type) * element_count);

			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;

		if (source_count < target_count)
			throw std::exception("Invalid size error");

		memcpy(this->instance, data, sizeof(native_type) * element_count);
	}


	return (*this);
}






//unsigned Char variable
#undef native_type
#define native_type unsigned char
spider::variable<native_type>::variable(std::string name, spider_mode mode, spider_access access) : instance(nullptr),
pimpl(new spider_pimpl()),
type_name(typeid(native_type).name()) {


	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	this->_name = name;

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->access = access;


	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	switch (mode)
	{
	case spider::create:
		this->pimpl->shmem = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(native_type),
			unicode_name.c_str());
		break;
	case spider::open:
		this->pimpl->shmem = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, unicode_name.c_str());
		break;
	default:
		throw std::exception("Invalid mode");
		break;
	}

	if (this->pimpl->shmem == nullptr)
		throw std::exception("Invalid shared memory handle");

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));

	if (this->instance == nullptr) {
		CloseHandle(this->pimpl->shmem);
		throw std::exception("Invalid memory pointer");
	}


	this->delay_time = 100;
	this->element_count = 1;
	this->is_block = true;
}

spider::variable<native_type>::variable(std::string name, unsigned int element_count, unsigned int delay, spider_mode mode, spider_access access) : instance(nullptr),
pimpl(new spider_pimpl()),
type_name(typeid(native_type).name()) {
	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	this->_name = name;

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->access = access;


	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	switch (mode)
	{
	case spider::create:
		this->pimpl->shmem = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(native_type) * element_count,
			unicode_name.c_str());
		break;
	case spider::open:
		this->pimpl->shmem = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, unicode_name.c_str());
		break;
	default:
		throw std::exception("Invalid mode");
		break;
	}

	if (this->pimpl->shmem == nullptr)
		throw std::exception("Invalid shared memory handle");

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type) * element_count);

	if (this->instance == nullptr) {
		CloseHandle(this->pimpl->shmem);
		throw std::exception("Invalid memory pointer");
	}

	this->delay_time = delay;
	this->element_count = element_count;
	this->is_block = true;
}

spider::variable<native_type>::variable(std::string name, unsigned int delay, spider_mode mode, spider_access access) : instance(nullptr),
pimpl(new spider_pimpl()),
type_name(typeid(native_type).name()) {

	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	this->_name = name;

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->access = access;


	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	switch (mode)
	{
	case spider::create:
		this->pimpl->shmem = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(native_type),
			unicode_name.c_str());
		break;
	case spider::open:
		this->pimpl->shmem = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, unicode_name.c_str());
		break;
	default:
		throw std::exception("Invalid mode");
		break;
	}

	if (this->pimpl->shmem == nullptr)
		throw std::exception("Invalid shared memory handle");

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));

	if (this->instance == nullptr) {
		CloseHandle(this->pimpl->shmem);
		throw std::exception("Invalid memory pointer");
	}

	this->delay_time = delay;
	this->element_count = 1;
	this->is_block = true;
}

std::string spider::variable<native_type>::type() {
	return this->type_name;
}

std::string spider::variable<native_type>::name() {
	return this->_name;
}

spider::variable<native_type>::~variable() {
	if (this->instance)
		::UnmapViewOfFile(this->instance);
	if (this->pimpl->shmem)
		::CloseHandle(this->pimpl->shmem);
	if (this->pimpl->event_write_handle)
		CloseHandle(this->pimpl->event_write_handle);
}


spider::variable<native_type>& spider::variable<native_type>::block(bool is_block) {
	this->is_block = is_block;

	return *this;
}

void spider::variable<native_type>::operator=(native_type data) {

	if (this->access == spider_access::read_only)
		throw std::exception("Proteced memory");

	if (this->is_block == true) {
		spider_raii raii([&] {
			SetEvent(this->pimpl->event_write_handle);
			});

		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);
		ResetEvent(this->pimpl->event_write_handle);

		switch (write_handle_ret) {
		case WAIT_OBJECT_0:
			memcpy(this->instance, &data, sizeof(native_type));
			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		memcpy(this->instance, &data, sizeof(native_type));
	}

}

void spider::variable<native_type>::operator>>(native_type& data) {
	if (this->is_block == true) {
		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);
		native_type value = 0;
		switch (write_handle_ret) {
		case WAIT_OBJECT_0:
			memcpy(&value, this->instance, sizeof(native_type));
			data = value;
			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		native_type value = 0;
		memcpy(&value, this->instance, sizeof(native_type));
		data = value;
	}

}

spider::variable<native_type>& spider::variable<native_type>::delay(unsigned int delay) {
	this->delay_time = delay;
	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::receive(native_type* data, unsigned int element_count) {

	if (this->is_block == true) {
		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);

		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;

		switch (write_handle_ret) {
		case WAIT_OBJECT_0:


			if (source_count < target_count)
				throw std::exception("Invalid size error");

			memcpy(data, this->instance, sizeof(native_type) * element_count);
			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;
		if (source_count < target_count)
			throw std::exception("Invalid size error");

		memcpy(data, this->instance, sizeof(native_type) * element_count);
	}


	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::send(native_type* data, unsigned int element_count) {

	if (this->access == spider_access::read_only)
		throw std::exception("Proected memory");


	if (this->is_block == true) {
		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);

		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;

		switch (write_handle_ret) {
		case WAIT_OBJECT_0:


			if (source_count < target_count)
				throw std::exception("Invalid size error");

			memcpy(this->instance, data, sizeof(native_type) * element_count);

			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;

		if (source_count < target_count)
			throw std::exception("Invalid size error");

		memcpy(this->instance, data, sizeof(native_type) * element_count);
	}


	return (*this);
}




// Char variable
#undef native_type
#define native_type char
spider::variable<native_type>::variable(std::string name, spider_mode mode, spider_access access) : instance(nullptr),
pimpl(new spider_pimpl()),
type_name(typeid(native_type).name()) {


	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	this->_name = name;

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->access = access;


	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	switch (mode)
	{
	case spider::create:
		this->pimpl->shmem = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(native_type),
			unicode_name.c_str());
		break;
	case spider::open:
		this->pimpl->shmem = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, unicode_name.c_str());
		break;
	default:
		throw std::exception("Invalid mode");
		break;
	}

	if (this->pimpl->shmem == nullptr)
		throw std::exception("Invalid shared memory handle");

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));

	if (this->instance == nullptr) {
		CloseHandle(this->pimpl->shmem);
		throw std::exception("Invalid memory pointer");
	}


	this->delay_time = 100;
	this->element_count = 1;
	this->is_block = true;
}

spider::variable<native_type>::variable(std::string name, unsigned int element_count, unsigned int delay, spider_mode mode, spider_access access) : instance(nullptr),
pimpl(new spider_pimpl()),
type_name(typeid(native_type).name()) {
	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	this->_name = name;

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->access = access;


	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	switch (mode)
	{
	case spider::create:
		this->pimpl->shmem = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(native_type) * element_count,
			unicode_name.c_str());
		break;
	case spider::open:
		this->pimpl->shmem = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, unicode_name.c_str());
		break;
	default:
		throw std::exception("Invalid mode");
		break;
	}

	if (this->pimpl->shmem == nullptr)
		throw std::exception("Invalid shared memory handle");

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type) * element_count);

	if (this->instance == nullptr) {
		CloseHandle(this->pimpl->shmem);
		throw std::exception("Invalid memory pointer");
	}

	this->delay_time = delay;
	this->element_count = element_count;
	this->is_block = true;
}

spider::variable<native_type>::variable(std::string name, unsigned int delay, spider_mode mode, spider_access access) : instance(nullptr),
pimpl(new spider_pimpl()),
type_name(typeid(native_type).name()) {

	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	this->_name = name;

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->access = access;


	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	switch (mode)
	{
	case spider::create:
		this->pimpl->shmem = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(native_type),
			unicode_name.c_str());
		break;
	case spider::open:
		this->pimpl->shmem = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, unicode_name.c_str());
		break;
	default:
		throw std::exception("Invalid mode");
		break;
	}

	if (this->pimpl->shmem == nullptr)
		throw std::exception("Invalid shared memory handle");

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));

	if (this->instance == nullptr) {
		CloseHandle(this->pimpl->shmem);
		throw std::exception("Invalid memory pointer");
	}

	this->delay_time = delay;
	this->element_count = 1;
	this->is_block = true;
}

std::string spider::variable<native_type>::type() {
	return this->type_name;
}

std::string spider::variable<native_type>::name() {
	return this->_name;
}

spider::variable<native_type>::~variable() {
	if (this->instance)
		::UnmapViewOfFile(this->instance);
	if (this->pimpl->shmem)
		::CloseHandle(this->pimpl->shmem);
	if (this->pimpl->event_write_handle)
		CloseHandle(this->pimpl->event_write_handle);
}


spider::variable<native_type>& spider::variable<native_type>::block(bool is_block) {
	this->is_block = is_block;

	return *this;
}

void spider::variable<native_type>::operator=(native_type data) {

	if (this->access == spider_access::read_only)
		throw std::exception("Proteced memory");

	if (this->is_block == true) {
		spider_raii raii([&] {
			SetEvent(this->pimpl->event_write_handle);
			});

		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);
		ResetEvent(this->pimpl->event_write_handle);

		switch (write_handle_ret) {
		case WAIT_OBJECT_0:
			memcpy(this->instance, &data, sizeof(native_type));
			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		memcpy(this->instance, &data, sizeof(native_type));
	}

}

void spider::variable<native_type>::operator>>(native_type& data) {
	if (this->is_block == true) {
		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);
		native_type value = 0;
		switch (write_handle_ret) {
		case WAIT_OBJECT_0:
			memcpy(&value, this->instance, sizeof(native_type));
			data = value;
			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		native_type value = 0;
		memcpy(&value, this->instance, sizeof(native_type));
		data = value;
	}

}

spider::variable<native_type>& spider::variable<native_type>::delay(unsigned int delay) {
	this->delay_time = delay;
	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::receive(native_type* data, unsigned int element_count) {

	if (this->is_block == true) {
		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);

		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;

		switch (write_handle_ret) {
		case WAIT_OBJECT_0:


			if (source_count < target_count)
				throw std::exception("Invalid size error");

			memcpy(data, this->instance, sizeof(native_type) * element_count);
			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;
		if (source_count < target_count)
			throw std::exception("Invalid size error");

		memcpy(data, this->instance, sizeof(native_type) * element_count);
	}


	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::send(native_type* data, unsigned int element_count) {

	if (this->access == spider_access::read_only)
		throw std::exception("Proected memory");


	if (this->is_block == true) {
		DWORD write_handle_ret = WaitForSingleObject(this->pimpl->event_write_handle, this->delay_time);

		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;

		switch (write_handle_ret) {
		case WAIT_OBJECT_0:


			if (source_count < target_count)
				throw std::exception("Invalid size error");

			memcpy(this->instance, data, sizeof(native_type) * element_count);

			break;
		case WAIT_TIMEOUT:
			throw std::exception("Time out");
			break;
		case WAIT_FAILED:
			throw std::exception("Unexpected error");
			break;
		default:
			break;
		}
	}
	else {
		unsigned int source_count = this->element_count;
		unsigned int target_count = element_count;

		if (source_count < target_count)
			throw std::exception("Invalid size error");

		memcpy(this->instance, data, sizeof(native_type) * element_count);
	}


	return (*this);
}






//// function
spider::function::function(std::string name) : pimpl(new spider_pimpl()),
										       mode(spider::spider_call_mode::notifier),
											   delay_value(1000),
											   is_complete(false){
	this->_name = name;
	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_start_event_name = unicode_name;
	std::wstring unicode_end_event_name = unicode_name;
	std::wstring unicode_function_blocker = unicode_name;


	unicode_start_event_name += L"_";
	unicode_start_event_name += L"event_start";

	unicode_end_event_name += L"_";
	unicode_end_event_name += L"event_end";

	unicode_function_blocker += L"_";
	unicode_function_blocker += L"event_blocker";

	this->pimpl->function_start_handle = CreateEvent(
		NULL,               // default security attributes
		FALSE,               // manual-reset event
		FALSE,              // initial state is nonsignaled
		unicode_start_event_name.c_str()  // object name
	);


	this->pimpl->function_end_handle = CreateEvent(
		NULL,               // default security attributes
		FALSE,               // manual-reset event
		FALSE,              // initial state is nonsignaled
		unicode_end_event_name.c_str()  // object name
	);

	this->pimpl->function_blocker = CreateMutex(NULL, FALSE, unicode_function_blocker.c_str());
}

spider::function::function(std::string name, std::function<void(spider::function*)> lambda) : pimpl(new spider_pimpl()),
																						      mode(spider::spider_call_mode::subscriber),
																							  is_working(true),
																							  delay_value(1000),
																							  is_complete(false){
	this->_name = name;
	this->lambda = lambda;

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_start_event_name = unicode_name;
	std::wstring unicode_end_event_name = unicode_name;
	std::wstring unicode_function_blocker = unicode_name;


	unicode_start_event_name += L"_";
	unicode_start_event_name += L"event_start";

	unicode_end_event_name += L"_";
	unicode_end_event_name += L"event_end";

	unicode_function_blocker += L"_";
	unicode_function_blocker  += L"event_blocker";


	this->pimpl->function_start_handle = CreateEvent(
		NULL,               // default security attributes
		FALSE,               // manual-reset event
		FALSE,              // initial state is nonsignaled
		unicode_start_event_name.c_str()  // object name
	);

	this->pimpl->function_end_handle = CreateEvent(
		NULL,               // default security attributes
		FALSE,               // manual-reset event
		FALSE,              // initial state is nonsignaled
		unicode_end_event_name.c_str()  // object name
	);

	this->pimpl->function_blocker = CreateMutex(NULL, FALSE, unicode_function_blocker.c_str());




	worker = std::move(std::thread([&](){
		while (this->is_working) {

			try {


				spider::spider_raii raii1([&]() {
					SetEvent(this->pimpl->function_end_handle);
				});

				DWORD write_handle_ret = WaitForSingleObject(this->pimpl->function_start_handle, INFINITE);
				

				spider::spider_raii raii_bloacker([&]() {
					ReleaseMutex(this->pimpl->function_blocker);
				});
				DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
				switch (blocker_ret) {
				case WAIT_OBJECT_0:
					break;
				case WAIT_TIMEOUT:
					throw std::exception("Time out");
					break;
				case WAIT_FAILED:
					throw std::exception("Unexpected error");
					break;
				default:
					break;
				}

			
				
				switch (write_handle_ret) {
				case WAIT_OBJECT_0:
					if(this->is_complete == true)
						this->lambda(this);
					break;
				case WAIT_TIMEOUT:
					throw std::exception("Time out");
					break;
				case WAIT_FAILED:
					throw std::exception("Unexpected error");
					break;
				default:
					break;
				}
			}
			catch (std::exception e) {
				std::cout << e.what() << std::endl;
			}

			
		}
	}));
	worker.detach();
}

spider::function::~function() {
	this->is_working = false;


	if(this->pimpl->function_start_handle != nullptr)
		CloseHandle(this->pimpl->function_start_handle);

	if (this->pimpl->function_end_handle != nullptr)
		CloseHandle(this->pimpl->function_end_handle);


	if (this->pimpl->function_blocker != nullptr)
		CloseHandle(this->pimpl->function_blocker);
}

void spider::function::operator() () {

	SetEvent(this->pimpl->function_start_handle);

	DWORD write_handle_ret = WaitForSingleObject(this->pimpl->function_end_handle, this->delay_value);
	switch (write_handle_ret) {
	case WAIT_OBJECT_0:
		break;
	case WAIT_TIMEOUT:
		throw std::exception("Time out");
		break;
	case WAIT_FAILED:
		throw std::exception("Unexpected error");
		break;
	default:
		break;
	}
}


spider::function& spider::function::lock() {
	DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
	switch (blocker_ret) {
	case WAIT_OBJECT_0:
		break;
	case WAIT_TIMEOUT:
		throw std::exception("Time out");
		break;
	case WAIT_FAILED:
		throw std::exception("Unexpected error");
		break;
	default:
		break;
	}


	return *this;
}
spider::function& spider::function::unlock() {
	ReleaseMutex(this->pimpl->function_blocker);

	return *this;
}


spider::function& spider::function::complete() {
	this->is_complete = true;
	return *this;
}
spider::function& spider::function::args() {
	this->is_args = true;
	this->is_returns = false;
	
	return *this;
}
spider::function& spider::function::returns() {
	this->is_args = false;
	this->is_returns = true;
	return *this;
}
spider::function& spider::function::delay(unsigned int delay) {
	this->delay_value = delay;
	return *this;
}


#undef native_type
#define native_type unsigned char
template<> spider::function& spider::function::arg<native_type>(std::string name) {
	if (this->is_args == false) throw std::exception("Need args mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;


	if(this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end())
		throw std::exception("Duplicate key");

	try {
		switch (mode)
		{
			case spider::subscriber: {
				std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
				variable->block(false);
				auto temp = std::static_pointer_cast<Ivariable>(variable);
				this->pimpl->arguments[type_name] = temp;
				break;
			}

			case spider::notifier: {
				std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
				variable->block(false);
				auto temp = std::static_pointer_cast<Ivariable>(variable);
				this->pimpl->arguments[type_name] = temp;
				break;
			}

		default:
			break;
		}

	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::arg<native_type>(std::string name, unsigned int element_count) {
	if (this->is_args == false) throw std::exception("Need args mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;

	if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end())
		throw std::exception("Duplicate key");

	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->arguments[type_name] = temp;
			break;
		}
			
		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->arguments[type_name] = temp;
			break;
		}
			
		default:
			break;
		}
	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::ret<native_type>(std::string name) {
	if (this->is_returns == false) throw std::exception("Need return mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;
	if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end())
		throw std::exception("Duplicate key");
	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}
			
		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}
			
		default:
			break;
		}
	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::ret<native_type>(std::string name, unsigned int element_count) {
	if (this->is_returns == false) throw std::exception("Need return mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;
	if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end())
		throw std::exception("Duplicate key");
	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}
			
		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}
			
		default:
			break;
		}
	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::push<native_type>(std::string name, native_type value) {
	if (this->is_args == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;


			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				(*temp.get()) = value;
			}

		}
		catch (std::exception e) {
			throw e;
		}
	}
	else if (this->is_returns == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;

			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				(*temp.get()) = value;
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}


	return *this;
}
template<> spider::function& spider::function::push<native_type>(std::string name, native_type* value, unsigned int element_count) {
	if (this->is_args == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;

			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->send(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}
	else if (this->is_returns == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;

			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->send(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}


	return *this;
}

template<> spider::function& spider::function::get<native_type>(std::string name, native_type* value) {
	if (this->is_args == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, 1);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}
	else if (this->is_returns == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, 1);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}

	return *this;
}
template<> spider::function& spider::function::get<native_type>(std::string name, native_type* value, unsigned int element_count) {
	if (this->is_args == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}

	}
	else if (this->is_returns == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}
	return *this;
}


#undef native_type
#define native_type char
template<> spider::function& spider::function::arg<native_type>(std::string name) {
	if (this->is_args == false) throw std::exception("Need args mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;


	if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end())
		throw std::exception("Duplicate key");

	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->arguments[type_name] = temp;
			break;
		}

		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->arguments[type_name] = temp;
			break;
		}

		default:
			break;
		}

	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::arg<native_type>(std::string name, unsigned int element_count) {
	if (this->is_args == false) throw std::exception("Need args mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;

	if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end())
		throw std::exception("Duplicate key");

	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->arguments[type_name] = temp;
			break;
		}

		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->arguments[type_name] = temp;
			break;
		}

		default:
			break;
		}
	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::ret<native_type>(std::string name) {
	if (this->is_returns == false) throw std::exception("Need return mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;
	if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end())
		throw std::exception("Duplicate key");
	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}

		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}

		default:
			break;
		}
	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::ret<native_type>(std::string name, unsigned int element_count) {
	if (this->is_returns == false) throw std::exception("Need return mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;
	if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end())
		throw std::exception("Duplicate key");
	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}

		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}

		default:
			break;
		}
	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::push<native_type>(std::string name, native_type value) {
	if (this->is_args == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;


			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				(*temp.get()) = value;
			}

		}
		catch (std::exception e) {
			throw e;
		}
	}
	else if (this->is_returns == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;

			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				(*temp.get()) = value;
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}


	return *this;
}
template<> spider::function& spider::function::push<native_type>(std::string name, native_type* value, unsigned int element_count) {
	if (this->is_args == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;

			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->send(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}
	else if (this->is_returns == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;

			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->send(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}


	return *this;
}

template<> spider::function& spider::function::get<native_type>(std::string name, native_type* value) {
	if (this->is_args == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, 1);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}
	else if (this->is_returns == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, 1);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}

	return *this;
}
template<> spider::function& spider::function::get<native_type>(std::string name, native_type* value, unsigned int element_count) {
	if (this->is_args == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}

	}
	else if (this->is_returns == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}
	return *this;
}

#undef native_type
#define native_type int
template<> spider::function& spider::function::arg<native_type>(std::string name) {
	if (this->is_args == false) throw std::exception("Need args mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;


	if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end())
		throw std::exception("Duplicate key");

	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->arguments[type_name] = temp;
			break;
		}

		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->arguments[type_name] = temp;
			break;
		}

		default:
			break;
		}

	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::arg<native_type>(std::string name, unsigned int element_count) {
	if (this->is_args == false) throw std::exception("Need args mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;

	if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end())
		throw std::exception("Duplicate key");

	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->arguments[type_name] = temp;
			break;
		}

		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->arguments[type_name] = temp;
			break;
		}

		default:
			break;
		}
	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::ret<native_type>(std::string name) {
	if (this->is_returns == false) throw std::exception("Need return mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;
	if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end())
		throw std::exception("Duplicate key");
	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}

		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}

		default:
			break;
		}
	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::ret<native_type>(std::string name, unsigned int element_count) {
	if (this->is_returns == false) throw std::exception("Need return mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;
	if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end())
		throw std::exception("Duplicate key");
	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}

		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}

		default:
			break;
		}
	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::push<native_type>(std::string name, native_type value) {
	if (this->is_args == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;


			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				(*temp.get()) = value;
			}

		}
		catch (std::exception e) {
			throw e;
		}
	}
	else if (this->is_returns == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;

			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				(*temp.get()) = value;
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}


	return *this;
}
template<> spider::function& spider::function::push<native_type>(std::string name, native_type* value, unsigned int element_count) {
	if (this->is_args == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;

			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->send(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}
	else if (this->is_returns == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;

			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->send(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}


	return *this;
}

template<> spider::function& spider::function::get<native_type>(std::string name, native_type* value) {
	if (this->is_args == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, 1);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}
	else if (this->is_returns == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, 1);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}

	return *this;
}
template<> spider::function& spider::function::get<native_type>(std::string name, native_type* value, unsigned int element_count) {
	if (this->is_args == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}

	}
	else if (this->is_returns == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}
	return *this;
}

#undef native_type
#define native_type double
template<> spider::function& spider::function::arg<native_type>(std::string name) {
	if (this->is_args == false) throw std::exception("Need args mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;


	if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end())
		throw std::exception("Duplicate key");

	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->arguments[type_name] = temp;
			break;
		}

		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->arguments[type_name] = temp;
			break;
		}

		default:
			break;
		}

	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::arg<native_type>(std::string name, unsigned int element_count) {
	if (this->is_args == false) throw std::exception("Need args mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;

	if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end())
		throw std::exception("Duplicate key");

	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->arguments[type_name] = temp;
			break;
		}

		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->arguments[type_name] = temp;
			break;
		}

		default:
			break;
		}
	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::ret<native_type>(std::string name) {
	if (this->is_returns == false) throw std::exception("Need return mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;
	if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end())
		throw std::exception("Duplicate key");
	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}

		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}

		default:
			break;
		}
	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::ret<native_type>(std::string name, unsigned int element_count) {
	if (this->is_returns == false) throw std::exception("Need return mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;
	if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end())
		throw std::exception("Duplicate key");
	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}

		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}

		default:
			break;
		}
	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::push<native_type>(std::string name, native_type value) {
	if (this->is_args == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;


			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				(*temp.get()) = value;
			}

		}
		catch (std::exception e) {
			throw e;
		}
	}
	else if (this->is_returns == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;

			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				(*temp.get()) = value;
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}


	return *this;
}
template<> spider::function& spider::function::push<native_type>(std::string name, native_type* value, unsigned int element_count) {
	if (this->is_args == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;

			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->send(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}
	else if (this->is_returns == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;

			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->send(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}


	return *this;
}

template<> spider::function& spider::function::get<native_type>(std::string name, native_type* value) {
	if (this->is_args == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, 1);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}
	else if (this->is_returns == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, 1);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}

	return *this;
}
template<> spider::function& spider::function::get<native_type>(std::string name, native_type* value, unsigned int element_count) {
	if (this->is_args == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}

	}
	else if (this->is_returns == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}
	return *this;
}



#undef native_type
#define native_type unsigned int
template<> spider::function& spider::function::arg<native_type>(std::string name) {
	if (this->is_args == false) throw std::exception("Need args mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;


	if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end())
		throw std::exception("Duplicate key");

	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->arguments[type_name] = temp;
			break;
		}

		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->arguments[type_name] = temp;
			break;
		}

		default:
			break;
		}

	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::arg<native_type>(std::string name, unsigned int element_count) {
	if (this->is_args == false) throw std::exception("Need args mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;

	if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end())
		throw std::exception("Duplicate key");

	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->arguments[type_name] = temp;
			break;
		}

		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->arguments[type_name] = temp;
			break;
		}

		default:
			break;
		}
	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::ret<native_type>(std::string name) {
	if (this->is_returns == false) throw std::exception("Need return mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;
	if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end())
		throw std::exception("Duplicate key");
	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}

		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(type_name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}

		default:
			break;
		}
	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::ret<native_type>(std::string name, unsigned int element_count) {
	if (this->is_returns == false) throw std::exception("Need return mode on");
	if (this->is_complete == true) throw std::exception("Registration completed");
	std::string type_name = this->_name + "_";
	type_name += name;
	if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end())
		throw std::exception("Duplicate key");
	try {
		switch (mode)
		{
		case spider::subscriber: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}

		case spider::notifier: {
			std::shared_ptr<spider::variable<native_type>> variable(new spider::variable<native_type>(name, spider::spider_mode::create, spider::spider_access::read_write));
			variable->block(false);
			auto temp = std::static_pointer_cast<Ivariable>(variable);
			this->pimpl->return_list[type_name] = temp;
			break;
		}

		default:
			break;
		}
	}
	catch (std::exception e) {
		throw e;
	}

	return *this;
}
template<> spider::function& spider::function::push<native_type>(std::string name, native_type value) {
	if (this->is_args == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;


			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				(*temp.get()) = value;
			}

		}
		catch (std::exception e) {
			throw e;
		}
	}
	else if (this->is_returns == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;

			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				(*temp.get()) = value;
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}


	return *this;
}
template<> spider::function& spider::function::push<native_type>(std::string name, native_type* value, unsigned int element_count) {
	if (this->is_args == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;

			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->send(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}
	else if (this->is_returns == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;

			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->send(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}


	return *this;
}

template<> spider::function& spider::function::get<native_type>(std::string name, native_type* value) {
	if (this->is_args == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, 1);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}
	else if (this->is_returns == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, 1);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}

	return *this;
}
template<> spider::function& spider::function::get<native_type>(std::string name, native_type* value, unsigned int element_count) {
	if (this->is_args == true) {
		try {
			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->arguments.find(type_name) != this->pimpl->arguments.end()) {
				auto variable = this->pimpl->arguments[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}

	}
	else if (this->is_returns == true) {
		try {
			spider::spider_raii raii_bloacker([&]() {
				ReleaseMutex(this->pimpl->function_blocker);
				});
			DWORD blocker_ret = WaitForSingleObject(this->pimpl->function_blocker, INFINITE);
			switch (blocker_ret) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				throw std::exception("Time out");
				break;
			case WAIT_FAILED:
				throw std::exception("Unexpected error");
				break;
			default:
				break;
			}

			std::string type_name = this->_name + "_";
			type_name += name;
			if (this->pimpl->return_list.find(type_name) != this->pimpl->return_list.end()) {
				auto variable = this->pimpl->return_list[type_name];
				auto temp = std::static_pointer_cast<spider::variable<native_type>>(variable);
				temp->receive(value, element_count);
			}
		}
		catch (std::exception e) {
			throw e;
		}
	}
	return *this;
}