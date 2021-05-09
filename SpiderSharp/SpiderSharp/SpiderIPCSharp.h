#ifndef SPIDER_IPC_SHARP
#define SPIDER_IPC_SHARP



#include <SpiderIPC.h>

#include "managed_shared_ptr.h"

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;


namespace SPIDER {

	public enum SPIDER_MODE {
		CREATE = 0,
		OPEN,
	};

	public enum SPIDER_ACCESS {
		READ_ONLY,
		READ_WRITE
	};

	public enum SPIDER_CALL_MODE {
		SUBSCRIBER,
		NOTIFIER
	};


	generic <class ItemType>
	public ref class MyGeneric {
		ItemType m_item;

	public:
		MyGeneric(ItemType item) : m_item(item) {}
		void F() {
			Console::WriteLine("F");
		}
	};



	template <typename T> public ref class Variable {

	private:
		mananged_shared_ptr<spider::variable<T>> _instance;


	public:

		Variable(String^ name, SPIDER_MODE mode, SPIDER_ACCESS access);
		Variable(String^ name, unsigned int element_count, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access);
		Variable(String^ name, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access);

		~Variable();
		!Variable();

		void operator=(T data);
		void operator>>(T% data);
		Variable<T>% Delay(unsigned int delay);
		Variable<T>% Receive(T* data, unsigned int element_count);
		Variable<T>% Send(T* data, unsigned int element_count);
		Variable<T>% Block(bool enable);
		String^ Type();
		String^ Name();

	};

	
	#undef NATIVE_TYPE 
	#define NATIVE_TYPE double
	template <> public ref class Variable<NATIVE_TYPE> {

	private:
		mananged_shared_ptr<spider::variable<NATIVE_TYPE>> _instance;

		
	public:
		
		Variable(String^ name, SPIDER_MODE mode, SPIDER_ACCESS access);
		Variable(String^ name, unsigned int element_count, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access);
		Variable(String^ name, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access);

		~Variable();
		!Variable();

		void operator=(NATIVE_TYPE data);
		void operator>>( NATIVE_TYPE% data);
		Variable<NATIVE_TYPE>% Delay(unsigned int delay);
		Variable<NATIVE_TYPE>% Receive(NATIVE_TYPE* data, unsigned int element_count);
		Variable<NATIVE_TYPE>% Send( NATIVE_TYPE* data, unsigned int element_count);
		Variable<NATIVE_TYPE>% Block(bool enable);
		String^ Type();
		String^ Name();

	};


	#undef NATIVE_TYPE 
	#define NATIVE_TYPE char
	template <> public ref class Variable<NATIVE_TYPE> {

	private:
		mananged_shared_ptr<spider::variable<NATIVE_TYPE>> _instance;


	public:

		Variable(String^ name, SPIDER_MODE mode, SPIDER_ACCESS access);
		Variable(String^ name, unsigned int element_count, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access);
		Variable(String^ name, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access);

		~Variable();
		!Variable();

		void operator=(NATIVE_TYPE data);
		void operator>>(NATIVE_TYPE% data);
		Variable<NATIVE_TYPE>% Delay(unsigned int delay);
		Variable<NATIVE_TYPE>% Receive(NATIVE_TYPE* data, unsigned int element_count);
		Variable<NATIVE_TYPE>% Send(NATIVE_TYPE* data, unsigned int element_count);
		Variable<NATIVE_TYPE>% Block(bool enable);
		String^ Type();
		String^ Name();

	};


	#undef NATIVE_TYPE 
	#define NATIVE_TYPE unsigned char
	template <> public ref class Variable<NATIVE_TYPE> {

	private:
		mananged_shared_ptr<spider::variable<NATIVE_TYPE>> _instance;


	public:

		Variable(String^ name, SPIDER_MODE mode, SPIDER_ACCESS access);
		Variable(String^ name, unsigned int element_count, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access);
		Variable(String^ name, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access);

		~Variable();
		!Variable();

		void operator=(NATIVE_TYPE data);
		void operator>>(NATIVE_TYPE% data);
		Variable<NATIVE_TYPE>% Delay(unsigned int delay);
		Variable<NATIVE_TYPE>% Receive(NATIVE_TYPE* data, unsigned int element_count);
		Variable<NATIVE_TYPE>% Send(NATIVE_TYPE* data, unsigned int element_count);
		Variable<NATIVE_TYPE>% Block(bool enable);
		String^ Type();
		String^ Name();

	};


	#undef NATIVE_TYPE 
	#define NATIVE_TYPE int
	template <> public ref class Variable<NATIVE_TYPE> {

	private:
		mananged_shared_ptr<spider::variable<NATIVE_TYPE>> _instance;


	public:

		Variable(String^ name, SPIDER_MODE mode, SPIDER_ACCESS access);
		Variable(String^ name, unsigned int element_count, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access);
		Variable(String^ name, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access);

		~Variable();
		!Variable();

		void operator=(NATIVE_TYPE data);
		void operator>>(NATIVE_TYPE% data);
		Variable<NATIVE_TYPE>% Delay(unsigned int delay);
		Variable<NATIVE_TYPE>% Receive(NATIVE_TYPE* data, unsigned int element_count);
		Variable<NATIVE_TYPE>% Send(NATIVE_TYPE* data, unsigned int element_count);
		Variable<NATIVE_TYPE>% Block(bool enable);
		String^ Type();
		String^ Name();

	};

	#undef NATIVE_TYPE 
	#define NATIVE_TYPE unsigned int
	template <> public ref class Variable<NATIVE_TYPE> {

	private:
		mananged_shared_ptr<spider::variable<NATIVE_TYPE>> _instance;


	public:

		Variable(String^ name, SPIDER_MODE mode, SPIDER_ACCESS access);
		Variable(String^ name, unsigned int element_count, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access);
		Variable(String^ name, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access);

		~Variable();
		!Variable();

		void operator=(NATIVE_TYPE data);
		void operator>>(NATIVE_TYPE% data);
		Variable<NATIVE_TYPE>% Delay(unsigned int delay);
		Variable<NATIVE_TYPE>% Receive(NATIVE_TYPE* data, unsigned int element_count);
		Variable<NATIVE_TYPE>% Send(NATIVE_TYPE* data, unsigned int element_count);
		Variable<NATIVE_TYPE>% Block(bool enable);
		String^ Type();
		String^ Name();

	};


	
}

#endif