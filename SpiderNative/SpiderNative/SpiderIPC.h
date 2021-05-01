#ifndef SPIDERIPC
#define SPIDERIPC

#include <string>
#include <memory>

#pragma warning(disable : 4251)

#ifdef SPIDER_IPC_API_EXPORT
#define SPIDER_API __declspec(dllexport)
#else
#define SPIDER_API __declspec(dllimport)
#endif


namespace spider {

	// pimple class
	class spider_pimpl;

	// template class structures
	template <typename T> class variable {};


	// template specialized class
	#undef native_type
	#define native_type double
	template <> class __declspec(dllexport) variable<native_type> {

	private:
		native_type* instance;
		unsigned int delay_time;
		std::shared_ptr<spider_pimpl> pimpl;
		unsigned int element_count;
	public:

		variable(std::string name);
		variable(std::string name, unsigned int element_cout, unsigned int delay);
		variable(std::string name, unsigned int delay);
		~variable();
		void operator=(native_type data);
		void operator>>(native_type&);
		spider::variable<native_type>& delay(unsigned int delay);
		spider::variable<native_type>& receive(native_type* data, unsigned int element_count);
		spider::variable<native_type>& send(native_type* data, unsigned int element_count);
	};



	// template specialized class
	#undef native_type
	#define native_type int
	template <> class __declspec(dllexport) variable<native_type> {

	private:
		native_type* instance;
		unsigned int delay_time;
		std::shared_ptr<spider_pimpl> pimpl;
		unsigned int element_count;
	public:

		variable(std::string name);
		variable(std::string name, unsigned int element_cout, unsigned int delay);
		variable(std::string name, unsigned int delay);
		~variable();
		void operator=(native_type data);
		void operator>>(native_type&);
		spider::variable<native_type>& delay(unsigned int delay);
		spider::variable<native_type>& receive(native_type* data, unsigned int element_count);
		spider::variable<native_type>& send(native_type* data, unsigned int element_count);
	};

	// template specialized class
	#undef native_type
	#define native_type unsigned int
	template <> class __declspec(dllexport) variable<native_type> {

	private:
		native_type* instance;
		unsigned int delay_time;
		std::shared_ptr<spider_pimpl> pimpl;
		unsigned int element_count;
	public:

		variable(std::string name);
		variable(std::string name, unsigned int element_cout, unsigned int delay);
		variable(std::string name, unsigned int delay);
		~variable();
		void operator=(native_type data);
		void operator>>(native_type&);
		spider::variable<native_type>& delay(unsigned int delay);
		spider::variable<native_type>& receive(native_type* data, unsigned int element_count);
		spider::variable<native_type>& send(native_type* data, unsigned int element_count);
	};


	// template specialized class
    #undef native_type
    #define native_type char
	template <> class __declspec(dllexport) variable<native_type> {

	private:
		native_type* instance;
		unsigned int delay_time;
		std::shared_ptr<spider_pimpl> pimpl;
		unsigned int element_count;
	public:

		variable(std::string name);
		variable(std::string name, unsigned int element_cout, unsigned int delay);
		variable(std::string name, unsigned int delay);
		~variable();
		void operator=(native_type data);
		void operator>>(native_type&);
		spider::variable<native_type>& delay(unsigned int delay);
		spider::variable<native_type>& receive(native_type* data, unsigned int element_count);
		spider::variable<native_type>& send(native_type* data, unsigned int element_count);
	};


	// template specialized class
	#undef native_type
	#define native_type unsigned char
	template <> class __declspec(dllexport) variable<native_type> {

	private:
		native_type* instance;
		unsigned int delay_time;
		std::shared_ptr<spider_pimpl> pimpl;
		unsigned int element_count;
	public:

		variable(std::string name);
		variable(std::string name, unsigned int element_cout, unsigned int delay);
		variable(std::string name, unsigned int delay);
		~variable();
		void operator=(native_type data);
		void operator>>(native_type&);
		spider::variable<native_type>& delay(unsigned int delay);
		spider::variable<native_type>& receive(native_type* data, unsigned int element_count);
		spider::variable<native_type>& send(native_type* data, unsigned int element_count);
	};

};

#endif