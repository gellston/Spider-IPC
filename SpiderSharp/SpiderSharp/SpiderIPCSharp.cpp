#include <string>

#include <msclr/marshal_cppstd.h>

#include "SpiderIPCSharp.h"


using namespace SPIDER;
using namespace System;




//////////////////////////////////////////////////

/*
#undef NATIVE_TYPE 
#define NATIVE_TYPE double
SPIDER::Variable<NATIVE_TYPE>::Variable(String^ name, SPIDER_MODE mode , SPIDER_ACCESS access) {
	std::string _name = msclr::interop::marshal_as<std::string>(name);
	spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
	spider::spider_access _access = static_cast<spider::spider_access>(access);


	this->_instance = new spider::variable<NATIVE_TYPE>(_name, _mode, _access);

}
SPIDER::Variable<NATIVE_TYPE>::Variable(String^ name, unsigned int element_count, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access ) {
	std::string _name = msclr::interop::marshal_as<std::string>(name);
	spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
	spider::spider_access _access = static_cast<spider::spider_access>(access);


	this->_instance = new spider::variable<NATIVE_TYPE>(_name, element_count, delay, _mode, _access);
}
SPIDER::Variable<NATIVE_TYPE>::Variable(String^ name, unsigned int delay, SPIDER_MODE mode , SPIDER_ACCESS access ) {
	std::string _name = msclr::interop::marshal_as<std::string>(name);
	spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
	spider::spider_access _access = static_cast<spider::spider_access>(access);

	this->_instance = new spider::variable<NATIVE_TYPE>(_name, delay, _mode, _access);
}

SPIDER::Variable<NATIVE_TYPE>::~Variable() {

}
SPIDER::Variable<NATIVE_TYPE>::!Variable() {

}


void SPIDER::Variable<NATIVE_TYPE>::operator=(NATIVE_TYPE data) {
	auto native_instance = this->_instance.get().get();
	(*native_instance) = data;
}
void SPIDER::Variable<NATIVE_TYPE>::operator>>(NATIVE_TYPE% data) {
	auto native_instance = this->_instance.get().get();

	NATIVE_TYPE temp;

	(*native_instance) >> temp;

	data = temp;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Delay(unsigned int delay) {
	this->_instance->delay(delay);

	return *this;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Receive(NATIVE_TYPE* data, unsigned int element_count) {
	this->_instance->receive(data, element_count);

	return *this;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Send( NATIVE_TYPE* data, unsigned int element_count) {
	this->_instance->send(data, element_count);

	return *this;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Block(bool enable) {
	this->_instance->block(enable);

	return *this;
}
String^ SPIDER::Variable<NATIVE_TYPE>::Type() {
	
	String^ temp = gcnew String(this->_instance->type().c_str());

	return temp;

}
String^ SPIDER::Variable<NATIVE_TYPE>::Name() {
	String^ temp = gcnew String(this->_instance->name().c_str());

	return temp;

}



#undef NATIVE_TYPE 
#define NATIVE_TYPE int
SPIDER::Variable<NATIVE_TYPE>::Variable(String^ name, SPIDER_MODE mode, SPIDER_ACCESS access) {
	std::string _name = msclr::interop::marshal_as<std::string>(name);
	spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
	spider::spider_access _access = static_cast<spider::spider_access>(access);


	this->_instance = new spider::variable<NATIVE_TYPE>(_name, _mode, _access);

}
SPIDER::Variable<NATIVE_TYPE>::Variable(String^ name, unsigned int element_count, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access) {
	std::string _name = msclr::interop::marshal_as<std::string>(name);
	spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
	spider::spider_access _access = static_cast<spider::spider_access>(access);


	this->_instance = new spider::variable<NATIVE_TYPE>(_name, element_count, delay, _mode, _access);
}
SPIDER::Variable<NATIVE_TYPE>::Variable(String^ name, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access) {
	std::string _name = msclr::interop::marshal_as<std::string>(name);
	spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
	spider::spider_access _access = static_cast<spider::spider_access>(access);

	this->_instance = new spider::variable<NATIVE_TYPE>(_name, delay, _mode, _access);
}

SPIDER::Variable<NATIVE_TYPE>::~Variable() {

}
SPIDER::Variable<NATIVE_TYPE>::!Variable() {

}

void SPIDER::Variable<NATIVE_TYPE>::operator=(NATIVE_TYPE data) {
	auto native_instance = this->_instance.get().get();
	(*native_instance) = data;
}
void SPIDER::Variable<NATIVE_TYPE>::operator>>(NATIVE_TYPE% data) {
	auto native_instance = this->_instance.get().get();

	NATIVE_TYPE temp;

	(*native_instance) >> temp;

	data = temp;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Delay(unsigned int delay) {
	this->_instance->delay(delay);

	return *this;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Receive(NATIVE_TYPE* data, unsigned int element_count) {
	this->_instance->receive(data, element_count);

	return *this;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Send(NATIVE_TYPE* data, unsigned int element_count) {
	this->_instance->send(data, element_count);

	return *this;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Block(bool enable) {
	this->_instance->block(enable);

	return *this;
}
String^ SPIDER::Variable<NATIVE_TYPE>::Type() {

	String^ temp = gcnew String(this->_instance->type().c_str());

	return temp;

}
String^ SPIDER::Variable<NATIVE_TYPE>::Name() {
	String^ temp = gcnew String(this->_instance->name().c_str());

	return temp;

}



#undef NATIVE_TYPE 
#define NATIVE_TYPE unsigned int
SPIDER::Variable<NATIVE_TYPE>::Variable(String^ name, SPIDER_MODE mode, SPIDER_ACCESS access) {
	std::string _name = msclr::interop::marshal_as<std::string>(name);
	spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
	spider::spider_access _access = static_cast<spider::spider_access>(access);


	this->_instance = new spider::variable<NATIVE_TYPE>(_name, _mode, _access);

}
SPIDER::Variable<NATIVE_TYPE>::Variable(String^ name, unsigned int element_count, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access) {
	std::string _name = msclr::interop::marshal_as<std::string>(name);
	spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
	spider::spider_access _access = static_cast<spider::spider_access>(access);


	this->_instance = new spider::variable<NATIVE_TYPE>(_name, element_count, delay, _mode, _access);
}
SPIDER::Variable<NATIVE_TYPE>::Variable(String^ name, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access) {
	std::string _name = msclr::interop::marshal_as<std::string>(name);
	spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
	spider::spider_access _access = static_cast<spider::spider_access>(access);

	this->_instance = new spider::variable<NATIVE_TYPE>(_name, delay, _mode, _access);
}

SPIDER::Variable<NATIVE_TYPE>::~Variable() {

}
SPIDER::Variable<NATIVE_TYPE>::!Variable() {

}

void SPIDER::Variable<NATIVE_TYPE>::operator=(NATIVE_TYPE data) {
	auto native_instance = this->_instance.get().get();
	(*native_instance) = data;
}
void SPIDER::Variable<NATIVE_TYPE>::operator>>(NATIVE_TYPE% data) {
	auto native_instance = this->_instance.get().get();

	NATIVE_TYPE temp;

	(*native_instance) >> temp;

	data = temp;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Delay(unsigned int delay) {
	this->_instance->delay(delay);

	return *this;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Receive(NATIVE_TYPE* data, unsigned int element_count) {
	this->_instance->receive(data, element_count);

	return *this;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Send(NATIVE_TYPE* data, unsigned int element_count) {
	this->_instance->send(data, element_count);

	return *this;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Block(bool enable) {
	this->_instance->block(enable);

	return *this;
}
String^ SPIDER::Variable<NATIVE_TYPE>::Type() {

	String^ temp = gcnew String(this->_instance->type().c_str());

	return temp;

}
String^ SPIDER::Variable<NATIVE_TYPE>::Name() {
	String^ temp = gcnew String(this->_instance->name().c_str());

	return temp;

}



#undef NATIVE_TYPE 
#define NATIVE_TYPE unsigned char
SPIDER::Variable<NATIVE_TYPE>::Variable(String^ name, SPIDER_MODE mode, SPIDER_ACCESS access) {
	std::string _name = msclr::interop::marshal_as<std::string>(name);
	spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
	spider::spider_access _access = static_cast<spider::spider_access>(access);


	this->_instance = new spider::variable<NATIVE_TYPE>(_name, _mode, _access);

}
SPIDER::Variable<NATIVE_TYPE>::Variable(String^ name, unsigned int element_count, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access) {
	std::string _name = msclr::interop::marshal_as<std::string>(name);
	spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
	spider::spider_access _access = static_cast<spider::spider_access>(access);


	this->_instance = new spider::variable<NATIVE_TYPE>(_name, element_count, delay, _mode, _access);
}
SPIDER::Variable<NATIVE_TYPE>::Variable(String^ name, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access) {
	std::string _name = msclr::interop::marshal_as<std::string>(name);
	spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
	spider::spider_access _access = static_cast<spider::spider_access>(access);

	this->_instance = new spider::variable<NATIVE_TYPE>(_name, delay, _mode, _access);
}

SPIDER::Variable<NATIVE_TYPE>::~Variable() {

}
SPIDER::Variable<NATIVE_TYPE>::!Variable() {

}

void SPIDER::Variable<NATIVE_TYPE>::operator=(NATIVE_TYPE data) {
	auto native_instance = this->_instance.get().get();
	(*native_instance) = data;
}
void SPIDER::Variable<NATIVE_TYPE>::operator>>(NATIVE_TYPE% data) {
	auto native_instance = this->_instance.get().get();

	NATIVE_TYPE temp;

	(*native_instance) >> temp;

	data = temp;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Delay(unsigned int delay) {
	this->_instance->delay(delay);

	return *this;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Receive(NATIVE_TYPE* data, unsigned int element_count) {
	this->_instance->receive(data, element_count);

	return *this;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Send(NATIVE_TYPE* data, unsigned int element_count) {
	this->_instance->send(data, element_count);

	return *this;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Block(bool enable) {
	this->_instance->block(enable);

	return *this;
}
String^ SPIDER::Variable<NATIVE_TYPE>::Type() {

	String^ temp = gcnew String(this->_instance->type().c_str());

	return temp;

}
String^ SPIDER::Variable<NATIVE_TYPE>::Name() {
	String^ temp = gcnew String(this->_instance->name().c_str());

	return temp;

}



#undef NATIVE_TYPE 
#define NATIVE_TYPE char
SPIDER::Variable<NATIVE_TYPE>::Variable(String^ name, SPIDER_MODE mode, SPIDER_ACCESS access) {
	std::string _name = msclr::interop::marshal_as<std::string>(name);
	spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
	spider::spider_access _access = static_cast<spider::spider_access>(access);


	this->_instance = new spider::variable<NATIVE_TYPE>(_name, _mode, _access);

}
SPIDER::Variable<NATIVE_TYPE>::Variable(String^ name, unsigned int element_count, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access) {
	std::string _name = msclr::interop::marshal_as<std::string>(name);
	spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
	spider::spider_access _access = static_cast<spider::spider_access>(access);


	this->_instance = new spider::variable<NATIVE_TYPE>(_name, element_count, delay, _mode, _access);
}
SPIDER::Variable<NATIVE_TYPE>::Variable(String^ name, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access) {
	std::string _name = msclr::interop::marshal_as<std::string>(name);
	spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
	spider::spider_access _access = static_cast<spider::spider_access>(access);

	this->_instance = new spider::variable<NATIVE_TYPE>(_name, delay, _mode, _access);
}

SPIDER::Variable<NATIVE_TYPE>::~Variable() {

}
SPIDER::Variable<NATIVE_TYPE>::!Variable() {

}

void SPIDER::Variable<NATIVE_TYPE>::operator=(NATIVE_TYPE data) {
	auto native_instance = this->_instance.get().get();
	(*native_instance) = data;
}
void SPIDER::Variable<NATIVE_TYPE>::operator>>(NATIVE_TYPE% data) {
	auto native_instance = this->_instance.get().get();

	NATIVE_TYPE temp;

	(*native_instance) >> temp;

	data = temp;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Delay(unsigned int delay) {
	this->_instance->delay(delay);

	return *this;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Receive(NATIVE_TYPE* data, unsigned int element_count) {
	this->_instance->receive(data, element_count);

	return *this;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Send(NATIVE_TYPE* data, unsigned int element_count) {
	this->_instance->send(data, element_count);

	return *this;
}
Variable<NATIVE_TYPE>% SPIDER::Variable<NATIVE_TYPE>::Block(bool enable) {
	this->_instance->block(enable);

	return *this;
}
String^ SPIDER::Variable<NATIVE_TYPE>::Type() {

	String^ temp = gcnew String(this->_instance->type().c_str());

	return temp;

}
String^ SPIDER::Variable<NATIVE_TYPE>::Name() {
	String^ temp = gcnew String(this->_instance->name().c_str());

	return temp;

}
*/