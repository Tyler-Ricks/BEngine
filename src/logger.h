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
#include <format>
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


enum class LOG_LEVEL 
{
	FATAL,
	ERROR,
	WARN,
	INFO,
	DEBUG,
	TRACE,
	LOG_LEVEL_COUNT
};

const char* log_level_string(LOG_LEVEL lvl);


constexpr size_t LOG_SIZE_T = 1024;

struct Log
{
	LOG_LEVEL lvl;
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
	void log(LOG_LEVEL lvl, std::string_view msg);

	template<typename... Args>
	void logf(LOG_LEVEL lvl, std::format_string<Args...> format, Args&&... args)
	{
		{
			Log log_entry{};
			log_entry.lvl = lvl;
			log_entry.timestamp_ns = BTime::now_ns();

			auto res = std::format_to_n(
				log_entry.msg,
				LOG_SIZE_T - 1,
				format,
				std::forward<Args>(args)...);

			log_entry.msg[LOG_SIZE_T - 1] = '\0';

			{
				std::lock_guard<std::mutex> lock(queue_mtx);
				log_queue.push_back(log_entry);
				wait_var.notify_one();
			}
		}
	}

	void flush_log_queue();

};


//, makes sure the comma is left out in case there are no args
#define LOGF(lvl, message, ...)												\
	Logger::get_instance().logf(lvl, message, __VA_ARGS__)					\


#define LOG_FATAL(message, ...)												\
	Logger::get_instance().logf(LOG_LEVEL::FATAL,							\
								message, __VA_ARGS__)						\

#define LOG_ERROR(message, ...)												\
	Logger::get_instance().logf(LOG_LEVEL::ERROR,							\
								message, __VA_ARGS__)						\


#if LOG_WARN_ENABLED
#define LOG_WARN(message, ...)												\
	Logger::get_instance().logf(LOG_LEVEL::WARN,							\
								message, __VA_ARGS__)						\

#else
#define LOG_WARN(message, ...) ((void)0)

#endif // end LOG_WARN_ENABLED


#if LOG_INFO_ENABLED
#define LOG_INFO(message, ...)												\
	Logger::get_instance().logf(LOG_LEVEL::INFO,							\
								message, __VA_ARGS__)						\

#else
#define LOG_INFO(message, ...)((void)0)

#endif // end LOG_INFO_ENABLED


#ifdef _DEBUG

#define LOG_DEBUG(message, ...)												\
	Logger::get_instance().logf(LOG_LEVEL::DEBUG,							\
								message, __VA_ARGS__)						\

#define LOG_TRACE(message, ...)												\
	Logger::get_instance().logf(LOG_LEVEL::TRACE,							\
								message, __VA_ARGS__)						\

#else
#define LOG_DEBUG(message, ...)((void)0)
#define LOG_TRACE(message, ...)((void)0)

#endif // end _DEBUG

#endif