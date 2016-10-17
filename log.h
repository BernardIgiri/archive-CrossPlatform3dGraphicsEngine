#pragma once

namespace lg
{
    class Log;
    namespace util // not for use by users
    {
        void Append(Log &log,const char *s,std::streamsize n);
        // sink object for sending output into log
        class LogSink
		{
		public:
			typedef char						char_type;
			typedef boost::iostreams::sink_tag	category;

			LogSink(Log &log_) : log(log_) {};
			inline std::streamsize write(const char_type* s, std::streamsize n) { Append(log,s,n); return n; };
			Log &log;
		};
		typedef boost::iostreams::stream<util::LogSink> logStream_t;
    };
    // endl flushes output to log
    class endl
    {
    };
    // log modes
    enum LogMode { ERROR,WARNING,MESSAGE };
    // object for setting mode
	class Mode
	{
	    public:
	    Mode(LogMode m) : mode(m) {};
	    LogMode mode;
	};

    // log class
	class Log
	{
	public:
		friend void util::Append(Log &log,const char *s,std::streamsize n);;

        // outputs T object into log
		template <typename T>
		friend Log& operator << (Log& log, const T& s) { log.os<<s;return log; };
		// outputs log to ostream
		friend std::ostream& operator << (std::ostream& os, const Log& log) { os<<log.Message();return os; };

        // default mode is ERROR
		Log(const std::string &filename,const std::string &ownerLabel) : ofs(filename.c_str(),std::ios::out|std::ios::app), ownerLbl(ownerLabel), mode(ERROR) { os.open(*this); };
        // default mode is ERROR
		Log(const char *filename,const char *ownerLabel) : ofs(filename,std::ios::out|std::ios::app), ownerLbl(ownerLabel), mode(ERROR) { os.open(*this); };
		// writes output to log
		~Log() { FlushEntry(); };

		// set logging mode and returns log
		inline Log &Mode(LogMode m) { FlushEntry();mode=m;return *this; }

		// returns a string version of the current error message
		std::string Message() const;
		// flushes output to log and closes entry
		void FlushEntry();
	private:
        LogMode     mode; // log mode
        std::ofstream ofs; // output file stream
		util::logStream_t os; // output stream
		std::string buffer; // output buffer
		std::string ownerLbl; // string label for owner of log
	};

    // sets the mode of the log using the mode object
	template <>
	inline Log& operator << (Log& log, const Mode& m) { log.FlushEntry();log.Mode(m.mode);return log; };
    // flushes output to log
	template <>
	inline Log& operator << (Log& log, const endl& e) { log.FlushEntry();return log; };

	namespace util // low level objects, not for use by end user
	{
		inline void Append(Log &log,const char *s,std::streamsize n) { log.buffer.append(s,n); };
	}
};
