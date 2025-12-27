#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <thread>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include "BTime.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1

#ifdef _DEBUG
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#else
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0

#endif // end _DEBUG


enum class log_level 
{
	FATAL,
	ERROR,
	WARN,
	INFO,
	DEBUG,
	TRACE,
	LOG_LEVEL_COUNT
};

const char* log_level_string(log_level lvl);


constexpr size_t LOG_SIZE_T = 1024;

struct Log
{
	log_level lvl;
	ticks_ns timestamp_ns;
	// TODO: frame number, timestamp, etc
	char msg[LOG_SIZE_T];
};

class Logger {
	Logger();
	~Logger();

	std::ofstream log_file;
	std::thread log_thread;
	std::deque<Log> log_queue;
	std::atomic<bool> exit_flag = false;
	bool done_processing = false;
	std::mutex queue_mtx;
	std::condition_variable wait_var;

	void process_logs();

public:
	static Logger& get_instance();
	void log(log_level lvl, std::string_view msg);
	void flush_log_queue();

};

#define LOG_FATAL(message) Logger::get_instance().log(log_level::FATAL, message)
#define LOG_ERROR(message) Logger::get_instance().log(log_level::ERROR, message)

#if LOG_WARN_ENABLED
#define LOG_WARN(message) Logger::get_instance().log(log_level::WARN, message)

#else
#define LOG_WARN(message)

#endif // end LOG_WARN_ENABLED


#if LOG_INFO_ENABLED
#define LOG_INFO(message) Logger::get_instance().log(log_level::INFO, message)

#else
#define LOG_INFO(message)

#endif // end LOG_INFO_ENABLED


#ifdef _DEBUG
#define LOG_DEBUG(message) Logger::get_instance().log(log_level::DEBUG, message)
#define LOG_TRACE(message) Logger::get_instance().log(log_level::TRACE, message)

#else
#define LOG_DEBUG(message)
#define LOG_TRACE(message)

#endif // end _DEBUG

#endif