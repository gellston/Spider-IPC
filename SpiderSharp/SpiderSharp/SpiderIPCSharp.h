#ifndef SPIDER_IPC_SHARP
#define SPIDER_IPC_SHARP

#include <SpiderIPC.h>

#include "managed_shared_ptr.h"

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;


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


	public:

	

		
		Variable(String^ name, SPIDER_MODE mode, SPIDER_ACCESS access) {
			std::string _name = msclr::interop::marshal_as<std::string>(name);
			spider::spider_mode _mode = static_cast<spider::spider_mode>(mode);
			spider::spider_access _access = static_cast<spider::spider_access>(access);


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
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew System::String(e.what()));
			}


		}
		
		~Variable() {

		}
		!Variable() {

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
	

	public ref class Function {
	private:
		mananged_shared_ptr<spider::function> _instance;
	public:
		Function(String^ name) {
			auto _name = msclr::interop::marshal_as<std::string>(name);
			this->_instance = new spider::function(_name);
		}

		~Function() {

		}
		!Function() {

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

				}
				else if (T::typeid == System::Double::typeid) {

				}
				else if (T::typeid == System::Byte::typeid) {

				}
				else if (T::typeid == System::SByte::typeid) {

				}
				else {
					throw std::exception("Type not support");
				}
			}
			catch (std::exception e) {
				throw gcnew Exception(gcnew String(e.what()));
			}
			
		}

	
	};
}

#endif