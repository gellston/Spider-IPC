#ifndef SPIDERIPC
#define SPIDERIPC

#include <string>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>


#pragma warning(disable : 4251)

#ifdef SPIDER_IPC_API_EXPORT
#define SPIDER_API __declspec(dllexport)
#else
#define SPIDER_API __declspec(dllimport)
#endif


namespace spider {


	enum spider_mode {
		create =0,
		open,
	};

	enum spider_access {
		read_only,
		read_write
	};

	enum spider_call_mode {
		subscriber,
		notifier
	};


	// pimple class
	class spider_pimpl;


	class SPIDER_API Ivariable {

	public:
		Ivariable() {};
		virtual ~Ivariable() {};

		virtual std::string type() = 0;
		virtual std::string name() = 0;
	};


	// template class structures
	template <typename T> class variable {};


	// template specialized class
	#undef native_type
	#define native_type double
	template <> class SPIDER_API variable<native_type> : public Ivariable {

	private:
		native_type* instance;
		unsigned int delay_time;
		std::shared_ptr<spider_pimpl> pimpl;
		unsigned int element_count;
		spider::spider_access access;
		std::string type_name;
		std::string _name;
		bool is_block;
	public:

		variable(std::string name, spider_mode mode=spider_mode::create, spider_access access=spider::spider_access::read_write);
		variable(std::string name, unsigned int element_cout, unsigned int delay, spider_mode mode = spider_mode::create, spider_access access = spider::spider_access::read_write);
		variable(std::string name, unsigned int delay, spider_mode mode = spider_mode::create, spider_access access = spider::spider_access::read_write);
		~variable();
		void operator=(native_type data);
		void operator>>(native_type&);
		spider::variable<native_type>& delay(unsigned int delay);
		spider::variable<native_type>& receive(native_type* data, unsigned int element_count);
		spider::variable<native_type>& send(native_type* data, unsigned int element_count);
		spider::variable<native_type>& block(bool enable);
		std::string type() override;
		std::string name() override;
	};

	// template specialized class
	#undef native_type
	#define native_type int
	template <> class SPIDER_API variable<native_type> : public Ivariable {

	private:
		native_type* instance;
		unsigned int delay_time;
		std::shared_ptr<spider_pimpl> pimpl;
		unsigned int element_count;
		spider::spider_access access;
		std::string type_name;
		std::string _name;
		bool is_block;
	public:

		variable(std::string name, spider_mode mode = spider_mode::create, spider_access access = spider::spider_access::read_write);
		variable(std::string name, unsigned int element_cout, unsigned int delay, spider_mode mode = spider_mode::create, spider_access access = spider::spider_access::read_write);
		variable(std::string name, unsigned int delay, spider_mode mode = spider_mode::create, spider_access access = spider::spider_access::read_write);
		~variable();
		void operator=(native_type data);
		void operator>>(native_type&);
		spider::variable<native_type>& delay(unsigned int delay);
		spider::variable<native_type>& receive(native_type* data, unsigned int element_count);
		spider::variable<native_type>& send(native_type* data, unsigned int element_count);
		spider::variable<native_type>& block(bool enable);
		std::string type() override;
		std::string name() override;
	};

	// template specialized class
	#undef native_type
	#define native_type unsigned int
	template <> class SPIDER_API variable<native_type> : public Ivariable {

	private:
		native_type* instance;
		unsigned int delay_time;
		std::shared_ptr<spider_pimpl> pimpl;
		unsigned int element_count;
		spider::spider_access access;
		std::string type_name;
		std::string _name;
		bool is_block;
	public:

		variable(std::string name, spider_mode mode = spider_mode::create, spider_access access = spider::spider_access::read_write);
		variable(std::string name, unsigned int element_cout, unsigned int delay, spider_mode mode = spider_mode::create, spider_access access = spider::spider_access::read_write);
		variable(std::string name, unsigned int delay, spider_mode mode = spider_mode::create, spider_access access = spider::spider_access::read_write);
		~variable();
		void operator=(native_type data);
		void operator>>(native_type&);
		spider::variable<native_type>& delay(unsigned int delay);
		spider::variable<native_type>& receive(native_type* data, unsigned int element_count);
		spider::variable<native_type>& send(native_type* data, unsigned int element_count);
		spider::variable<native_type>& block(bool enable);
		std::string type() override;
		std::string name() override;
	};


	// template specialized class
    #undef native_type
    #define native_type char
	template <> class SPIDER_API variable<native_type> : public Ivariable {

	private:
		native_type* instance;
		unsigned int delay_time;
		std::shared_ptr<spider_pimpl> pimpl;
		unsigned int element_count;
		spider::spider_access access;
		std::string type_name;
		std::string _name;
		bool is_block;
	public:

		variable(std::string name, spider_mode mode = spider_mode::create, spider_access access = spider::spider_access::read_write);
		variable(std::string name, unsigned int element_cout, unsigned int delay, spider_mode mode = spider_mode::create, spider_access access = spider::spider_access::read_write);
		variable(std::string name, unsigned int delay, spider_mode mode = spider_mode::create, spider_access access = spider::spider_access::read_write);
		~variable();
		void operator=(native_type data);
		void operator>>(native_type&);
		spider::variable<native_type>& delay(unsigned int delay);
		spider::variable<native_type>& receive(native_type* data, unsigned int element_count);
		spider::variable<native_type>& send(native_type* data, unsigned int element_count);
		spider::variable<native_type>& block(bool enable);
		std::string type() override;
		std::string name() override;
	};


	// template specialized class
	#undef native_type
	#define native_type unsigned char
	template <> class SPIDER_API variable<native_type> : public Ivariable {

	private:
		native_type* instance;
		unsigned int delay_time;
		std::shared_ptr<spider_pimpl> pimpl;
		unsigned int element_count;
		spider::spider_access access;
		std::string type_name;
		std::string _name;
		bool is_block;
	public:

		variable(std::string name, spider_mode mode = spider_mode::create, spider_access access = spider::spider_access::read_write);
		variable(std::string name, unsigned int element_cout, unsigned int delay, spider_mode mode = spider_mode::create, spider_access access = spider::spider_access::read_write);
		variable(std::string name, unsigned int delay, spider_mode mode = spider_mode::create, spider_access access = spider::spider_access::read_write);
		~variable();
		void operator=(native_type data);
		void operator>>(native_type&);
		spider::variable<native_type>& delay(unsigned int delay);
		spider::variable<native_type>& receive(native_type* data, unsigned int element_count);
		spider::variable<native_type>& send(native_type* data, unsigned int element_count);
		spider::variable<native_type>& block(bool enable);
		std::string type() override;
		std::string name() override;
	};



	// template specialized class
	#undef native_type
	#define native_type std::string
	template <> class SPIDER_API variable<native_type> : public Ivariable {

	private:
		char* character_point;
		unsigned int delay_time;
		std::shared_ptr<spider_pimpl> pimpl;
		unsigned int element_count;
		spider::spider_access access;
		std::string type_name;
		std::string _name;
		bool is_block;
	public:
		variable(std::string name, unsigned int element_cout, unsigned int delay, spider_mode mode = spider_mode::create, spider_access access = spider::spider_access::read_write);
		~variable();
		void operator=(native_type data);
		void operator>>(native_type&);
		spider::variable<native_type>& delay(unsigned int delay);
		spider::variable<native_type>& block(bool enable);
		std::string type() override;
		std::string name() override;
	};


	


	// function
	class SPIDER_API function {
	private:
		std::string _name;
		std::function<void(spider::function *)> lambda;
		std::shared_ptr<spider_pimpl> pimpl;
		spider::spider_call_mode mode;
		std::thread worker;
		bool is_working;





		bool is_complete;
		bool is_args;
		bool is_returns;
		unsigned int delay_value;

		

	public:
		function(std::string name, std::function<void(spider::function*)> lambda);
		function(std::string name);
		~function();


		void operator() ();


		spider::function& complete();
		spider::function& args();
		spider::function& returns();
		spider::function& delay(unsigned int delay);
		spider::function& lock();
		spider::function& unlock();


		template<typename T> spider::function& arg(std::string name);
		template<typename T> spider::function& arg(std::string name, unsigned int element_count);
		template<typename T> spider::function& ret(std::string name);
		template<typename T> spider::function& ret(std::string name, unsigned int element_count);
		template<typename T> spider::function& push(std::string name, T value);
		template<typename T> spider::function& push(std::string name, T* value, unsigned int element_count);
		template<typename T> spider::function& get(std::string name, T* value);
		template<typename T> spider::function& get(std::string name, T* value, unsigned int element_count);
		

		#undef native_type
		#define native_type unsigned char
		template<> spider::function& arg<native_type>(std::string name);
		template<> spider::function& arg<native_type>(std::string name, unsigned int element_count);
		template<> spider::function& ret<native_type>(std::string name);
		template<> spider::function& ret<native_type>(std::string name, unsigned int element_count);
		template<> spider::function& push<native_type>(std::string name, native_type value);
		template<> spider::function& push<native_type>(std::string name, native_type* value, unsigned int element_count);
		template<> spider::function& get<native_type>(std::string name, native_type* value);
		template<> spider::function& get<native_type>(std::string name, native_type* value, unsigned int element_count);


		#undef native_type
		#define native_type char
		template<> spider::function& arg<native_type>(std::string name);
		template<> spider::function& arg<native_type>(std::string name, unsigned int element_count);
		template<> spider::function& ret<native_type>(std::string name);
		template<> spider::function& ret<native_type>(std::string name, unsigned int element_count);
		template<> spider::function& push<native_type>(std::string name, native_type value);
		template<> spider::function& push<native_type>(std::string name, native_type* value, unsigned int element_count);
		template<> spider::function& get<native_type>(std::string name, native_type* value);
		template<> spider::function& get<native_type>(std::string name, native_type* value, unsigned int element_count);
		
		#undef native_type
		#define native_type int
		template<> spider::function& arg<native_type>(std::string name);
		template<> spider::function& arg<native_type>(std::string name, unsigned int element_count);
		template<> spider::function& ret<native_type>(std::string name);
		template<> spider::function& ret<native_type>(std::string name, unsigned int element_count);
		template<> spider::function& push<native_type>(std::string name, native_type value);
		template<> spider::function& push<native_type>(std::string name, native_type* value, unsigned int element_count);
		template<> spider::function& get<native_type>(std::string name, native_type* value);
		template<> spider::function& get<native_type>(std::string name, native_type* value, unsigned int element_count);
		
		#undef native_type
		#define native_type unsigned int
		template<> spider::function& arg<native_type>(std::string name);
		template<> spider::function& arg<native_type>(std::string name, unsigned int element_count);
		template<> spider::function& ret<native_type>(std::string name);
		template<> spider::function& ret<native_type>(std::string name, unsigned int element_count);
		template<> spider::function& push<native_type>(std::string name, native_type value);
		template<> spider::function& push<native_type>(std::string name, native_type* value, unsigned int element_count);
		template<> spider::function& get<native_type>(std::string name, native_type* value);
		template<> spider::function& get<native_type>(std::string name, native_type* value, unsigned int element_count);

		#undef native_type
		#define native_type double
		template<> spider::function& arg<native_type>(std::string name);
		template<> spider::function& arg<native_type>(std::string name, unsigned int element_count);
		template<> spider::function& ret<native_type>(std::string name);
		template<> spider::function& ret<native_type>(std::string name, unsigned int element_count);
		template<> spider::function& push<native_type>(std::string name, native_type value);
		template<> spider::function& push<native_type>(std::string name, native_type* value, unsigned int element_count);
		template<> spider::function& get<native_type>(std::string name, native_type* value);
		template<> spider::function& get<native_type>(std::string name, native_type* value, unsigned int element_count);

		#undef native_type
		#define native_type std::string
		template<> spider::function& arg<native_type>(std::string name, unsigned int element_count);
		template<> spider::function& ret<native_type>(std::string name, unsigned int element_count);
		template<> spider::function& push<native_type>(std::string name, native_type value);
		template<> spider::function& get<native_type>(std::string name, native_type* value);
	};
	
};

#endif