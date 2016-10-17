#pragma once
#include "log.h"

namespace ex
{
	class error : std::exception
	{
	public:
		error() throw() {};
		virtual ~error() throw() {};
		error(lg::Log &log) throw() { Log(*this,log); }; // appends error name to log
		virtual const std::string &Msg() const { return msg; }; // returns error message
		virtual const char *ExName() { return "general fault"; }; // name of exception
	protected:
        // appends error name to log
		void Log(error& e,lg::Log &log)
		{ msg=log.Message();log<<"("<<e.ExName()<<")"; };

		std::string msg; // error message
	};

	class out_of_memory : public error
	{
	public:
		out_of_memory() throw() {};
		virtual ~out_of_memory() throw() {};
		out_of_memory(lg::Log &log) throw() { error::Log(*this,log); }; // appends error name to log
		virtual const char *ExName() { return "out of memory"; }; // name of exception
	};

	class logic_error : public error
	{
	public:
		logic_error() throw() {};
		virtual ~logic_error() throw() {};
		logic_error(lg::Log &log) throw() { error::Log(*this,log); }; // appends error name to log
		virtual const char *ExName() { return "logic error"; }; // name of exception
	};

	class invalid_input : public logic_error
	{
	public:
		invalid_input() throw() {};
		virtual ~invalid_input() throw() {};
		invalid_input(lg::Log &log) throw() { error::Log(*this,log); }; // appends error name to log
		virtual const char *ExName() { return "invalid input"; }; // name of exception
	};

	class value_out_of_bounds : public invalid_input
	{
	public:
		value_out_of_bounds() throw() {};
		virtual ~value_out_of_bounds() throw() {};
		value_out_of_bounds(lg::Log &log) throw() { error::Log(*this,log); }; // appends error name to log
		virtual const char *ExName() { return "value out of bounds"; }; // name of exception
	};

	class expected_object : public invalid_input
	{
	public:
		expected_object() throw() {};
		virtual ~expected_object() throw() {};
		expected_object(lg::Log &log) throw() { error::Log(*this,log); }; // appends error name to log
		virtual const char *ExName() { return "expected object"; }; // name of exception
	};

	class feature_not_found : public logic_error
	{
	public:
		feature_not_found() throw() {};
		virtual ~feature_not_found() throw() {};
		feature_not_found(lg::Log &log) throw() { error::Log(*this,log); }; // appends error name to log
		virtual const char *ExName() { return "required feature not found"; }; // name of exception
	};

	class io_error : public error
	{
	public:
		io_error() throw() {};
		virtual ~io_error() throw() {};
		io_error(lg::Log &log) throw() { error::Log(*this,log); }; // appends error name to log
		virtual const char *ExName() { return "i/o error"; }; // name of exception
	};

	class read_error : public io_error
	{
	public:
		read_error() throw() {};
		virtual ~read_error() throw() {};
		read_error(lg::Log &log) throw() { error::Log(*this,log); }; // appends error name to log
		virtual const char *ExName() { return "read error"; }; // name of exception
	};

	class write_error : public io_error
	{
	public:
		write_error() throw() {};
		virtual ~write_error() throw() {};
		write_error(lg::Log &log) throw() { error::Log(*this,log); }; // appends error name to log
		virtual const char *ExName() { return "write error"; }; // name of exception
	};

	class read_only_error : public write_error
	{
	public:
		read_only_error() throw() {};
		virtual ~read_only_error() throw() {};
		read_only_error(lg::Log &log) throw() { error::Log(*this,log); }; // appends error name to log
		virtual const char *ExName() { return "write attempt to read only data"; }; // name of exception
	};

	class open_failed_error : public io_error
	{
	public:
		open_failed_error() throw() {};
		virtual ~open_failed_error() throw() {};
		open_failed_error(lg::Log &log) throw() { error::Log(*this,log); }; // appends error name to log
		virtual const char *ExName() { return "open failed"; }; // name of exception
	};

	class file_not_found : public open_failed_error
	{
	public:
		file_not_found() throw() {};
		virtual ~file_not_found() throw() {};
		file_not_found(lg::Log &log) throw() { error::Log(*this,log); }; // appends error name to log
		virtual const char *ExName() { return "file not found"; }; // name of exception
	};

	class eof_error : public io_error
	{
	public:
		eof_error() throw() {};
		virtual ~eof_error() throw() {};
		eof_error(lg::Log &log) throw() { error::Log(*this,log); }; // appends error name to log
		virtual const char *ExName() { return "end of file reached"; }; // name of exception
	};

	class disk_full : public io_error
	{
	public:
		disk_full() throw() {};
		virtual ~disk_full() throw() {};
		disk_full(lg::Log &log) throw() { error::Log(*this,log); }; // appends error name to log
		virtual const char *ExName() { return "disk drive is full"; }; // name of exception
	};

	class access_denied : public io_error
	{
	public:
		access_denied() throw() {};
		virtual ~access_denied() throw() {};
		access_denied(lg::Log &log) throw() { error::Log(*this,log); }; // appends error name to log
		virtual const char *ExName() { return "access to resource denied"; }; // name of exception
	};

	class corrupt_data : public io_error
	{
	public:
		corrupt_data() throw() {};
		virtual ~corrupt_data() throw() {};
		corrupt_data(lg::Log &log) throw() { error::Log(*this,log); }; // appends error name to log
		virtual const char *ExName() { return "corrupt stream data"; }; // name of exception
	};

	class unrecognized_file_format : public io_error
	{
	public:
		unrecognized_file_format() throw() {};
		virtual ~unrecognized_file_format() throw() {};
		unrecognized_file_format(lg::Log &log) throw() { error::Log(*this,log); }; // appends error name to log
		virtual const char *ExName() { return "unrecognized file format"; }; // name of exception
	};
};
