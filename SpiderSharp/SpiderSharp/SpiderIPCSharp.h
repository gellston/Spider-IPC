#ifndef SPIDER_IPC_SHARP
#define SPIDER_IPC_SHARP


#include <iostream>


#include <SpiderIPC.h>

#include "managed_shared_ptr.h"

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

namespace SPIDER {





	public enum class SPIDER_MODE {
		CREATE = 0,
		OPEN,
	};

	public enum class SPIDER_ACCESS {
		READ_ONLY,
		READ_WRITE
	};

	public enum class SPIDER_CALL_MODE {
		SUBSCRIBER,
		NOTIFIER
	};


	generic <typename T> public ref class Variable {

	private:
		mananged_shared_ptr<spider::Ivariable> _instance;
		bool _is_disposed;

	public:

	

		
		Variable(String^ name, SPIDER_MODE mode, SPIDER_ACCESS access) {
			std::string _name = msclr::interop::marshal_as<std::string>(name);
			spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
			spider::spider_access _access = static_cast<spider::spider_access>(access);
			_is_disposed = false;

			try {
				if (T::typeid == System::Int32::typeid) {
					auto variable = new spider::variable<int>(_name, _mode, _access);
					this->_instance = variable;

				}
				else if (T::typeid == System::UInt32::typeid) {
					auto variable = new spider::variable<unsigned int>(_name, _mode, _access);
					this->_instance = variable;

				}
				else if (T::typeid == System::Double::typeid) {
					auto variable = new spider::variable<double>(_name, _mode, _access);
					this->_instance = variable;
				}
				else if (T::typeid == System::Byte::typeid) {
					auto variable = new spider::variable<unsigned char>(_name, _mode, _access);
					this->_instance = variable;
				}
				else if (T::typeid == System::SByte::typeid) {
					auto variable = new spider::variable<char>(_name, _mode, _access);
					this->_instance = variable;
				}
				else if (T::typeid == bool::typeid) {
					auto variable = new spider::variable<bool>(_name, _mode, _access);
					this->_instance = variable;
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew System::String(e.what()));
			}



		}
		Variable(String^ name, unsigned int element_count, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access) {

			std::string _name = msclr::interop::marshal_as<std::string>(name);
			spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
			spider::spider_access _access = static_cast<spider::spider_access>(access);
			_is_disposed = false;

			try {
				if (T::typeid == System::Int32::typeid) {
					auto variable = new spider::variable<int>(_name,element_count, delay, _mode, _access);
					this->_instance = variable;

				}
				else if (T::typeid == System::UInt32::typeid) {
					auto variable = new spider::variable<int>(_name, element_count, delay, _mode, _access);
					this->_instance = variable;

				}
				else if (T::typeid == System::Double::typeid) {
					auto variable = new spider::variable<int>(_name, element_count, delay, _mode, _access);
					this->_instance = variable;
				}
				else if (T::typeid == System::Byte::typeid) {
					auto variable = new spider::variable<unsigned char>(_name, element_count, delay, _mode, _access);
					this->_instance = variable;
				}
				else if (T::typeid == System::SByte::typeid) {
					auto variable = new spider::variable<char>(_name, element_count, delay, _mode, _access);
					this->_instance = variable;
				}
				else if (T::typeid == System::String::typeid) {
					auto variable = new spider::variable<std::string>(_name, element_count, delay, _mode, _access);
					this->_instance = variable;
				}
				else if (T::typeid == bool::typeid) {
					auto variable = new spider::variable<bool>(_name, element_count, delay, _mode, _access);
					this->_instance = variable;
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew System::String(e.what()));
			}



		}
		Variable(String^ name, unsigned int delay, SPIDER_MODE mode, SPIDER_ACCESS access) {

			std::string _name = msclr::interop::marshal_as<std::string>(name);
			spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
			spider::spider_access _access = static_cast<spider::spider_access>(access);
			_is_disposed = false;

			try {
				if (T::typeid == System::Int32::typeid) {
					auto variable = new spider::variable<int>(_name, delay, _mode, _access);
					this->_instance = variable;

				}
				else if (T::typeid == System::UInt32::typeid) {
					auto variable = new spider::variable<int>(_name, delay, _mode, _access);
					this->_instance = variable;

				}
				else if (T::typeid == System::Double::typeid) {
					auto variable = new spider::variable<int>(_name, delay, _mode, _access);
					this->_instance = variable;
				}
				else if (T::typeid == System::Byte::typeid) {
					auto variable = new spider::variable<unsigned char>(_name, delay, _mode, _access);
					this->_instance = variable;
				}
				else if (T::typeid == System::SByte::typeid) {
					auto variable = new spider::variable<char>(_name, delay, _mode, _access);
					this->_instance = variable;
				}
				else if (T::typeid == bool::typeid) {
					auto variable = new spider::variable<bool>(_name, delay, _mode, _access);
					this->_instance = variable;
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew System::String(e.what()));
			}


		}
		
		~Variable() { 
			// Managed 자원 해제 
			// Unmanaged 자원 해제 
		}
		!Variable() { //ummanaged 자원 해제 

		}

		
	    void Set(System::Object^ data) {
			try {
				if (T::typeid == System::Int32::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<int>>(this->_instance.get());
					*temp.get() = safe_cast<int>(data);
				}
				else if (T::typeid == System::UInt32::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<unsigned int>>(this->_instance.get());
					*temp.get() = safe_cast<unsigned int>(data);
				}
				else if (T::typeid == System::Double::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<double>>(this->_instance.get());
					*temp.get() = safe_cast<double>(data);
				}
				else if (T::typeid == System::Byte::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<unsigned char>>(this->_instance.get());
					*temp.get() = safe_cast<unsigned char>(data);
				}
				else if (T::typeid == System::SByte::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<char>>(this->_instance.get());
					*temp.get() = safe_cast<char>(data);
				}
				else if (T::typeid == bool::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<bool>>(this->_instance.get());
					*temp.get() = safe_cast<bool>(data);
				}
				else if (T::typeid == System::String::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<std::string>>(this->_instance.get());
					System::String^ input_sharp_string = safe_cast<System::String^>(data);
					std::string input_string = msclr::interop::marshal_as<std::string>(input_sharp_string);
					*temp.get() = input_string;
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew System::String(e.what()));
			}
		}
		Object^ Get() {
			try {
				if (T::typeid == System::Int32::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<int>>(this->_instance.get());
					int value = 0;
					*temp.get() >> value;

					return value;
				}
				else if (T::typeid == System::UInt32::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<unsigned int>>(this->_instance.get());
					unsigned int value = 0;
					*temp.get() >> value;

					return value;
				}
				else if (T::typeid == System::Double::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<double>>(this->_instance.get());
					double value = 0;
					*temp.get() >> value;

					return value;
				}
				else if (T::typeid == System::Byte::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<unsigned char>>(this->_instance.get());
					unsigned char value = 0;
					*temp.get() >> value;

					return value;
				}
				else if (T::typeid == System::SByte::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<char>>(this->_instance.get());
					char value = 0;
					*temp.get() >> value;

					return value;
				}
				else if (T::typeid == bool::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<bool>>(this->_instance.get());
					bool value = 0;
					*temp.get() >> value;

					return value;
				}
				else if (T::typeid == System::String::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<std::string>>(this->_instance.get());
					std::string return_string = "";
					(*temp) >> return_string;
					return gcnew System::String(return_string.c_str());
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew System::String(e.what()));
			}
		}
		Variable<T>^ Delay(unsigned int delay) {
			auto temp = std::static_pointer_cast<spider::variable<double>>(this->_instance.get());
			temp->delay(delay);

			return this;
		}
		Variable<T>^ Receive(IntPtr data, unsigned int element_count) {
			try {
				if (T::typeid == System::Int32::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<int>>(this->_instance.get());
					temp->receive((int*)data.ToPointer(), element_count);
				}
				else if (T::typeid == System::UInt32::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<unsigned int>>(this->_instance.get());
					temp->receive((unsigned int*)data.ToPointer(), element_count);
				}
				else if (T::typeid == System::Double::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<double>>(this->_instance.get());
					temp->receive((double*)data.ToPointer(), element_count);
				}
				else if (T::typeid == System::Byte::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<unsigned char>>(this->_instance.get());
					temp->receive((unsigned char*)data.ToPointer(), element_count);
				}
				else if (T::typeid == System::Byte::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<bool>>(this->_instance.get());
					temp->receive((bool*)data.ToPointer(), element_count);
				}
				else if (T::typeid == System::SByte::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<char>>(this->_instance.get());
					temp->receive((char*)data.ToPointer(), element_count);
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew System::String(e.what()));
			}
			return this;
		}
		Variable<T>^ Send(IntPtr data, unsigned int element_count) {
			try {
				if (T::typeid == System::Int32::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<int>>(this->_instance.get());
					temp->send((int *)data.ToPointer(), element_count);
				}
				else if (T::typeid == System::UInt32::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<unsigned int>>(this->_instance.get());
					temp->send((unsigned int*)data.ToPointer(), element_count);
				}
				else if (T::typeid == System::Double::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<double>>(this->_instance.get());
					temp->send((double*)data.ToPointer(), element_count);
				}
				else if (T::typeid == System::Byte::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<unsigned char>>(this->_instance.get());
					temp->send((unsigned char*)data.ToPointer(), element_count);
				}
				else if (T::typeid == bool::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<bool>>(this->_instance.get());
					temp->send((bool*)data.ToPointer(), element_count);
				}
				else if (T::typeid == System::SByte::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<char>>(this->_instance.get());
					temp->send((char*)data.ToPointer(), element_count);
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew System::String(e.what()));
			}
			return this;
		}
		Variable<T>^ Block(bool enable) {
			try {
				if (T::typeid == System::Int32::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<int>>(this->_instance.get());
					temp->block(enable);
				}
				else if (T::typeid == System::UInt32::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<unsigned int>>(this->_instance.get());
					temp->block(enable);
				}
				else if (T::typeid == System::Double::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<double>>(this->_instance.get());
					temp->block(enable);
				}
				else if (T::typeid == System::Byte::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<unsigned char>>(this->_instance.get());
					temp->block(enable);
				}
				else if (T::typeid == System::Byte::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<bool>>(this->_instance.get());
					temp->block(enable);
				}
				else if (T::typeid == System::SByte::typeid) {
					auto temp = std::static_pointer_cast<spider::variable<char>>(this->_instance.get());
					temp->block(enable);
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew System::String(e.what()));
			}
			return this;
		}
		String^ Type() {
			try {
				return gcnew System::String(this->_instance->type().c_str());
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew System::String(e.what()));
			}
			
		}
		String^ Name() {
			try {
				return gcnew System::String(this->_instance->name().c_str());
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew System::String(e.what()));
			}
		}

		

	};
	


	ref class Function;


	public delegate void FunctionTrace(Function^ instance);
	public delegate void NativeFunctionTrace(spider::function* instance);

	public ref class Function {

	private:



		FunctionTrace^ _functionTrace;
		NativeFunctionTrace^ _nativeFunctionTrace;
		IntPtr NativeCallbackHandle;
		GCHandle BlockGCHandleSharp;
		GCHandle BlockGCHandleNative;


		mananged_shared_ptr<spider::function> _instance;


		void NativeCallback(spider::function * callback) {
			try {
				this->_functionTrace->Invoke(this);
			}
			catch (Exception^ e) {
				System::Console::WriteLine(e->Message);
			}
			
		}

		void SharpCallback(Function^ instance) {

		}


	public:



		Function(String^ name) {

			try {
				auto _name = msclr::interop::marshal_as<std::string>(name);
				this->_instance = new spider::function(_name);

			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}

			
		}




		Function(String^ name, FunctionTrace^ delegate) {
			try {
				auto _name = msclr::interop::marshal_as<std::string>(name);

				this->_functionTrace = gcnew FunctionTrace(this, &SPIDER::Function::SharpCallback);
				this->BlockGCHandleSharp = GCHandle::Alloc(this->_functionTrace);

				this->_functionTrace = static_cast<FunctionTrace^> (Delegate::Combine(this->_functionTrace, delegate));
				

				this->_nativeFunctionTrace = gcnew NativeFunctionTrace(this, &SPIDER::Function::NativeCallback);
				this->BlockGCHandleNative = GCHandle::Alloc(this->_functionTrace);
				this->NativeCallbackHandle = Marshal::GetFunctionPointerForDelegate(this->_nativeFunctionTrace);
				auto native_pointer = static_cast<void(*)(spider::function*)>(this->NativeCallbackHandle.ToPointer());



				this->_instance = new spider::function(_name, native_pointer);
				std::cout << "native name : " << _name << std::endl;
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}
		}

		~Function() {
			//if (this->BlockGCHandleNative.IsAllocated == true) this->BlockGCHandleNative.Free();
			//if (this->BlockGCHandleSharp.IsAllocated == true) this->BlockGCHandleSharp.Free();
			//this->!Function();
		}
		!Function() {
			//this->_instance.Dispose();
		}

		void Call() {
			try {
				auto function = this->_instance.get();
				(*function.get())();
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));

			}
		}

		Function^ Complete() {
			try {
				this->_instance->complete();
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}

			return this;
		}

		Function^ Args() {
			try {
				this->_instance->args();
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}

			return this;
		}

		Function^ Returns() {
			try {
				this->_instance->returns();
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}

			return this;
		}

		Function^ Delay(unsigned int delay) {
			try {
				this->_instance->delay(delay);
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}

			return this;
		}


		Function^ Lock() {
			try {
				this->_instance->lock();
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}

			return this;
		}

		Function^ UnLock() {
			try {
				this->_instance->unlock();
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}

			return this;
		}

		generic<typename T> Function^ Arg(String^ name) {
			auto _name = msclr::interop::marshal_as<std::string>(name);
			try {
				if (T::typeid == System::Int32::typeid) {
					this->_instance->arg<int>(_name);
				}
				else if (T::typeid == System::UInt32::typeid) {
					this->_instance->arg<unsigned int>(_name);
				}
				else if (T::typeid == System::Double::typeid) {
					this->_instance->arg<double>(_name);
				}
				else if (T::typeid == System::Byte::typeid) {
					this->_instance->arg<unsigned char>(_name);
				}
				else if (T::typeid == System::SByte::typeid) {
					this->_instance->arg<char>(_name);
				}
				else if (T::typeid == bool::typeid) {
					this->_instance->arg<bool>(_name);
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}
			return this;
		}

		generic<typename T> Function^ Arg(String^ name, unsigned int element_count) {
			auto _name = msclr::interop::marshal_as<std::string>(name);
			try {
				if (T::typeid == System::Int32::typeid) {
					this->_instance->arg<int>(_name, element_count);
				}
				else if (T::typeid == System::UInt32::typeid) {
					this->_instance->arg<int>(_name, element_count);
				}
				else if (T::typeid == System::Double::typeid) {
					this->_instance->arg<int>(_name, element_count);
				}
				else if (T::typeid == System::Byte::typeid) {
					this->_instance->arg<int>(_name, element_count);
				}
				else if (T::typeid == System::SByte::typeid) {
					this->_instance->arg<int>(_name, element_count);
				}
				else if (T::typeid == System::String::typeid) {
					this->_instance->arg<std::string>(_name, element_count);
				}
				else if (T::typeid == bool::typeid) {
					this->_instance->arg<bool>(_name, element_count);
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}
			return this;
		}


		generic<typename T> Function^ Ret(String^ name) {
			auto _name = msclr::interop::marshal_as<std::string>(name);
			try {
				if (T::typeid == System::Int32::typeid) {
					this->_instance->ret<int>(_name);
				}
				else if (T::typeid == System::UInt32::typeid) {
					this->_instance->ret<unsigned int>(_name);
				}
				else if (T::typeid == System::Double::typeid) {
					this->_instance->ret<double>(_name);
				}
				else if (T::typeid == System::Byte::typeid) {
					this->_instance->ret<unsigned char>(_name);
				}
				else if (T::typeid == System::SByte::typeid) {
					this->_instance->ret<char>(_name);
				}
				else if (T::typeid == bool::typeid) {
					this->_instance->ret<bool>(_name);
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}
			return this;
		}

		generic<typename T> Function^ Ret(String^ name, unsigned int element_count) {
			auto _name = msclr::interop::marshal_as<std::string>(name);
			try {
				if (T::typeid == System::Int32::typeid) {
					this->_instance->ret<int>(_name, element_count);
				}
				else if (T::typeid == System::UInt32::typeid) {
					this->_instance->ret<int>(_name, element_count);
				}
				else if (T::typeid == System::Double::typeid) {
					this->_instance->ret<int>(_name, element_count);
				}
				else if (T::typeid == System::Byte::typeid) {
					this->_instance->ret<int>(_name, element_count);
				}
				else if (T::typeid == System::SByte::typeid) {
					this->_instance->ret<int>(_name, element_count);
				}
				else if (T::typeid == System::String::typeid) {
					this->_instance->ret<std::string>(_name, element_count);
				}
				else if (T::typeid == bool::typeid) {
					this->_instance->ret<bool>(_name, element_count);
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}
			return this;
		}

		generic<typename T> Function^ Push(String^ name, Object^ value) {
			auto _name = msclr::interop::marshal_as<std::string>(name);
			try {
				if (T::typeid == System::Int32::typeid) {
					int _value = safe_cast<int>(value);
					this->_instance->push<int>(_name, _value);
				}
				else if (T::typeid == System::UInt32::typeid) {
					unsigned int _value = safe_cast<unsigned int>(value);
					this->_instance->push<unsigned int>(_name, _value);
				}
				else if (T::typeid == System::Double::typeid) {
					double _value = safe_cast<double>(value);
					this->_instance->push<double>(_name, _value);
				}
				else if (T::typeid == System::Byte::typeid) {
					unsigned char _value = safe_cast<unsigned char>(value);
					this->_instance->push<unsigned char>(_name, _value);
				}
				else if (T::typeid == System::SByte::typeid) {
					char _value = safe_cast<char>(value);
					this->_instance->push<char>(_name, _value);
				}
				else if (T::typeid == System::String::typeid) {
					System::String^ input_sharp_string = safe_cast<System::String^>(value);
					std::string temp = msclr::interop::marshal_as<std::string>(input_sharp_string);
					this->_instance->push<std::string>(_name, temp);
				}
				else if (T::typeid == bool::typeid) {
					bool _value = safe_cast<bool>(value);
					this->_instance->push<bool>(_name, _value);
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}
			return this;
		}

		generic<typename T> Function^ Push(String^ name, IntPtr pointer, unsigned int element_count) {
			auto _name = msclr::interop::marshal_as<std::string>(name);
			try {
				if (T::typeid == System::Int32::typeid) {
					auto _pointer = static_cast<int*>(pointer.ToPointer());
					this->_instance->push<int>(_name, _pointer, element_count);
				}
				else if (T::typeid == System::UInt32::typeid) {
					auto _pointer = static_cast<unsigned int*>(pointer.ToPointer());
					this->_instance->push<unsigned int>(_name, _pointer, element_count);
				}
				else if (T::typeid == System::Double::typeid) {
					auto _pointer = static_cast<double*>(pointer.ToPointer());
					this->_instance->push<double>(_name, _pointer, element_count);
				}
				else if (T::typeid == System::Byte::typeid) {
					auto _pointer = static_cast<unsigned char*>(pointer.ToPointer());
					this->_instance->push<unsigned char>(_name, _pointer, element_count);
				}
				else if (T::typeid == System::SByte::typeid) {
					auto _pointer = static_cast<char*>(pointer.ToPointer());
					this->_instance->push<char>(_name, _pointer, element_count);
				}
				else if (T::typeid == bool::typeid) {
					auto _pointer = static_cast<bool*>(pointer.ToPointer());
					this->_instance->push<bool>(_name, _pointer, element_count);
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}
			return this;
		}


		generic<typename T> Function^ Get(String^ name, [Out]int% value) {
			auto _name = msclr::interop::marshal_as<std::string>(name);
			try {
				if(T::typeid == System::Int32::typeid){
					int _value = 0;
					this->_instance->get<int>(_name, &_value);
					value = _value;
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}
			return this;
		}

		generic<typename T> Function^ Get(String^ name, [Out]unsigned int% value) {
			auto _name = msclr::interop::marshal_as<std::string>(name);
			try {
				if (T::typeid == System::UInt32::typeid) {
					unsigned int _value = 0;
					this->_instance->get<unsigned int>(_name, &_value);
					value = _value;
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}
			return this;
		}

		generic<typename T> Function^ Get(String^ name, [Out]double% value) {
			auto _name = msclr::interop::marshal_as<std::string>(name);
			try {
				if (T::typeid == System::Double::typeid) {
					double _value = 0;
					this->_instance->get<double>(_name, &_value);
					value = _value;
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}
			return this;
		}

		generic<typename T> Function^ Get(String^ name, [Out]char% value) {
			auto _name = msclr::interop::marshal_as<std::string>(name);
			try {
				if (T::typeid == char::typeid) {
					char _value = 0;
					this->_instance->get<char>(_name, &_value);
					value = _value;
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}
			return this;
		}

		generic<typename T> Function^ Get(String^ name, [Out]unsigned char% value) {
			typedef unsigned char uchar;
			auto _name = msclr::interop::marshal_as<std::string>(name);
			try {
				if (T::typeid == uchar::typeid) {
					unsigned char _value = 0;
					this->_instance->get<unsigned char>(_name, &_value);
					value = _value;
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}
			return this;
		}

		generic<typename T> Function^ Get(String^ name, [Out]bool% value) {

			auto _name = msclr::interop::marshal_as<std::string>(name);
			try {
				if (T::typeid == bool::typeid) {
					bool _value = 0;
					this->_instance->get<bool>(_name, &_value);
					value = _value;
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}
			return this;
		}

		generic<typename T> Function^ Get(String^ name, [Out]System::String^% value) {

			auto _name = msclr::interop::marshal_as<std::string>(name);
			try {
				if (T::typeid == System::String::typeid) {
					std::string temp;
					this->_instance->get<std::string>(_name, &temp);
					value = gcnew System::String(temp.c_str());
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}
			return this;
		}

		generic<typename T> Function^ Get(String^ name, IntPtr pointer, unsigned int element_count) {
			auto _name = msclr::interop::marshal_as<std::string>(name);
			try {
				if (T::typeid == System::Int32::typeid) {
					auto _pointer = static_cast<int*>(pointer.ToPointer());
					this->_instance->get<int>(_name, _pointer, element_count);
				}
				else if (T::typeid == System::UInt32::typeid) {
					auto _pointer = static_cast<unsigned int*>(pointer.ToPointer());
					this->_instance->get<unsigned int>(_name, _pointer, element_count);
				}
				else if (T::typeid == System::Double::typeid) {
					auto _pointer = static_cast<double*>(pointer.ToPointer());
					this->_instance->get<double>(_name, _pointer, element_count);
				}
				else if (T::typeid == System::Byte::typeid) {
					auto _pointer = static_cast<unsigned char*>(pointer.ToPointer());
					this->_instance->get<unsigned char>(_name, _pointer, element_count);
				}
				else if (T::typeid == System::SByte::typeid) {
					auto _pointer = static_cast<char*>(pointer.ToPointer());
					this->_instance->get<char>(_name, _pointer, element_count);
				}
				else if (T::typeid == bool::typeid) {
					auto _pointer = static_cast<bool*>(pointer.ToPointer());
					this->_instance->get<bool>(_name, _pointer, element_count);
				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}
			return this;
		}

	};


}

#endif