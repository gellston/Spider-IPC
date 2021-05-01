#include "SpiderIPC.h"

#include <Windows.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <functional>


namespace spider {
	class spider_pimpl {
	public:
		HANDLE	shmem;
		HANDLE	event_read_handle;
		HANDLE	event_write_handle;
		spider_pimpl() {
			this->shmem = INVALID_HANDLE_VALUE;
			this->event_write_handle = INVALID_HANDLE_VALUE;
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
spider::variable<native_type>::variable(std::string name) : instance(nullptr),
													   pimpl(new spider_pimpl()){


	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->pimpl->event_write_handle = CreateEvent(
								     NULL,               // default security attributes
								     TRUE,               // manual-reset event
								     TRUE,              // initial state is nonsignaled
									 unicode_write_event_name.c_str()  // object name
								     );

	this->pimpl->shmem = ::CreateFileMapping(
							INVALID_HANDLE_VALUE,
							NULL,
							PAGE_READWRITE,
							0,
							sizeof(native_type),
		unicode_name.c_str()
							);

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));
	this->delay_time = 100;
	this->element_count = 1;

}

spider::variable<native_type>::variable(std::string name, unsigned int element_count, unsigned int delay) : instance(nullptr),
																							           pimpl(new spider_pimpl()) {

	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());

	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	this->pimpl->shmem = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(native_type) * element_count,
		unicode_name.c_str()
	);

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));
	this->delay_time = delay;
	this->element_count = element_count;
}

spider::variable<native_type>::variable(std::string name, unsigned int delay) : instance(nullptr),
																		   pimpl(new spider_pimpl()) {
	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	this->pimpl->shmem = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(native_type),
		unicode_name.c_str()
	);

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));
	this->delay_time = delay;
	this->element_count = 1;
}

spider::variable<native_type>::~variable() {
	::UnmapViewOfFile(this->instance);
	::CloseHandle(this->pimpl->shmem);
	CloseHandle(this->pimpl->event_write_handle);
	CloseHandle(this->pimpl->event_read_handle);
}

void spider::variable<native_type>::operator=(native_type data) {

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

void spider::variable<native_type>::operator>>(native_type& data) {
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

spider::variable<native_type> & spider::variable<native_type>::delay(unsigned int delay) {
	this->delay_time = delay;
	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::receive(native_type* data, unsigned int element_count) {
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
	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::send(native_type* data, unsigned int element_count) {
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
	return (*this);
}






//Int variable
#undef native_type
#define native_type int
spider::variable<native_type>::variable(std::string name) : instance(nullptr),
pimpl(new spider_pimpl()) {


	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	this->pimpl->shmem = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(native_type),
		unicode_name.c_str()
	);

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));
	this->delay_time = 100;
	this->element_count = 1;

}

spider::variable<native_type>::variable(std::string name, unsigned int element_count, unsigned int delay) : instance(nullptr),
pimpl(new spider_pimpl()) {

	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());

	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	this->pimpl->shmem = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(native_type) * element_count,
		unicode_name.c_str()
	);

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));
	this->delay_time = delay;
	this->element_count = element_count;
}

spider::variable<native_type>::variable(std::string name, unsigned int delay) : instance(nullptr),
pimpl(new spider_pimpl()) {
	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	this->pimpl->shmem = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(native_type),
		unicode_name.c_str()
	);

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));
	this->delay_time = delay;
	this->element_count = 1;
}

spider::variable<native_type>::~variable() {
	::UnmapViewOfFile(this->instance);
	::CloseHandle(this->pimpl->shmem);
	CloseHandle(this->pimpl->event_write_handle);
	CloseHandle(this->pimpl->event_read_handle);
}

void spider::variable<native_type>::operator=(native_type data) {

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

void spider::variable<native_type>::operator>>(native_type& data) {
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

spider::variable<native_type>& spider::variable<native_type>::delay(unsigned int delay) {
	this->delay_time = delay;
	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::receive(native_type* data, unsigned int element_count) {
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
	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::send(native_type* data, unsigned int element_count) {
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
	return (*this);
}




//Unsigned int variable
#undef native_type
#define native_type unsigned int
spider::variable<native_type>::variable(std::string name) : instance(nullptr),
pimpl(new spider_pimpl()) {


	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	this->pimpl->shmem = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(native_type),
		unicode_name.c_str()
	);

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));
	this->delay_time = 100;
	this->element_count = 1;

}

spider::variable<native_type>::variable(std::string name, unsigned int element_count, unsigned int delay) : instance(nullptr),
pimpl(new spider_pimpl()) {

	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());

	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	this->pimpl->shmem = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(native_type) * element_count,
		unicode_name.c_str()
	);

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));
	this->delay_time = delay;
	this->element_count = element_count;
}

spider::variable<native_type>::variable(std::string name, unsigned int delay) : instance(nullptr),
pimpl(new spider_pimpl()) {
	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	this->pimpl->shmem = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(native_type),
		unicode_name.c_str()
	);

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));
	this->delay_time = delay;
	this->element_count = 1;
}

spider::variable<native_type>::~variable() {
	::UnmapViewOfFile(this->instance);
	::CloseHandle(this->pimpl->shmem);
	CloseHandle(this->pimpl->event_write_handle);
	CloseHandle(this->pimpl->event_read_handle);
}

void spider::variable<native_type>::operator=(native_type data) {

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

void spider::variable<native_type>::operator>>(native_type& data) {
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

spider::variable<native_type>& spider::variable<native_type>::delay(unsigned int delay) {
	this->delay_time = delay;
	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::receive(native_type* data, unsigned int element_count) {
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
	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::send(native_type* data, unsigned int element_count) {
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
	return (*this);
}







//unsigned Char variable
#undef native_type
#define native_type unsigned char
spider::variable<native_type>::variable(std::string name) : instance(nullptr),
pimpl(new spider_pimpl()) {


	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	this->pimpl->shmem = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(native_type),
		unicode_name.c_str()
	);

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));
	this->delay_time = 100;
	this->element_count = 1;

}

spider::variable<native_type>::variable(std::string name, unsigned int element_count, unsigned int delay) : instance(nullptr),
pimpl(new spider_pimpl()) {

	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());

	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	this->pimpl->shmem = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(native_type) * element_count,
		unicode_name.c_str()
	);

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));
	this->delay_time = delay;
	this->element_count = element_count;
}

spider::variable<native_type>::variable(std::string name, unsigned int delay) : instance(nullptr),
pimpl(new spider_pimpl()) {
	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	this->pimpl->shmem = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(native_type),
		unicode_name.c_str()
	);

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));
	this->delay_time = delay;
	this->element_count = 1;
}

spider::variable<native_type>::~variable() {
	::UnmapViewOfFile(this->instance);
	::CloseHandle(this->pimpl->shmem);
	CloseHandle(this->pimpl->event_write_handle);
	CloseHandle(this->pimpl->event_read_handle);
}

void spider::variable<native_type>::operator=(native_type data) {

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

void spider::variable<native_type>::operator>>(native_type& data) {
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

spider::variable<native_type>& spider::variable<native_type>::delay(unsigned int delay) {
	this->delay_time = delay;
	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::receive(native_type* data, unsigned int element_count) {
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
	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::send(native_type* data, unsigned int element_count) {
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
	return (*this);
}



// Char variable
#undef native_type
#define native_type char
spider::variable<native_type>::variable(std::string name) : instance(nullptr),
pimpl(new spider_pimpl()) {


	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	this->pimpl->shmem = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(native_type),
		unicode_name.c_str()
	);

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));
	this->delay_time = 100;
	this->element_count = 1;

}

spider::variable<native_type>::variable(std::string name, unsigned int element_count, unsigned int delay) : instance(nullptr),
pimpl(new spider_pimpl()) {

	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());

	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	this->pimpl->shmem = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(native_type) * element_count,
		unicode_name.c_str()
	);

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));
	this->delay_time = delay;
	this->element_count = element_count;
}

spider::variable<native_type>::variable(std::string name, unsigned int delay) : instance(nullptr),
pimpl(new spider_pimpl()) {
	if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		throw std::exception("The name contains special characters.");
	}

	std::wstring unicode_name;
	unicode_name.assign(name.begin(), name.end());


	std::wstring unicode_write_event_name = unicode_name;
	unicode_write_event_name += L"_write.event";

	this->pimpl->event_write_handle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,              // initial state is nonsignaled
		unicode_write_event_name.c_str()  // object name
	);

	this->pimpl->shmem = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(native_type),
		unicode_name.c_str()
	);

	this->instance = (native_type*)::MapViewOfFile(this->pimpl->shmem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(native_type));
	this->delay_time = delay;
	this->element_count = 1;
}

spider::variable<native_type>::~variable() {
	::UnmapViewOfFile(this->instance);
	::CloseHandle(this->pimpl->shmem);
	CloseHandle(this->pimpl->event_write_handle);
	CloseHandle(this->pimpl->event_read_handle);
}

void spider::variable<native_type>::operator=(native_type data) {

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

void spider::variable<native_type>::operator>>(native_type& data) {
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

spider::variable<native_type>& spider::variable<native_type>::delay(unsigned int delay) {
	this->delay_time = delay;
	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::receive(native_type* data, unsigned int element_count) {
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
	return (*this);
}

spider::variable<native_type>& spider::variable<native_type>::send(native_type* data, unsigned int element_count) {
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
	return (*this);
}
