// Logger implementation
// Deploys a thread to handle logs that enter the queue in the background



#include "logger.h"


const char* log_level_string(log_level lvl)
{
	switch (lvl)
	{
		case log_level::FATAL: return "FATAL";
		case log_level::ERROR: return "ERROR";
		case log_level::WARN: return "WARN";
		case log_level::INFO: return "INFO";
		case log_level::DEBUG: return "DEBUG";
		case log_level::TRACE: return "TRACE";
	}
	return "UNKOWN";
}


Logger& Logger::get_instance()
{
	static Logger instance;
	return instance;
}

Logger::Logger() : log_file("log.txt", std::ios_base::trunc)
{
	log_thread = std::thread(&Logger::process_logs, this);
}

Logger::~Logger()
{
	exit_flag = true;
	wait_var.notify_all(); 

	{
		std::unique_lock<std::mutex> lock(queue_mtx);
		wait_var.wait(lock, [&]() {return done_processing; });
	}

	if (log_thread.joinable()) { log_thread.join(); }
}

void Logger::log(log_level lvl, std::string_view msg)
{
	{
		Log log_entry{};
		log_entry.lvl = lvl;
		log_entry.timestamp_ns = BTime::now_ns();

		size_t len = std::min(msg.size(), LOG_SIZE_T - 1);
		memcpy(log_entry.msg, msg.data(), len);
		log_entry.msg[len] = '\0';

		std::lock_guard<std::mutex> lock(queue_mtx);
		log_queue.push_back(log_entry);
		wait_var.notify_one();
	}
}



// This may be worth testing. Compare to an implementation that processes one 
//	log at a time, for instance
void Logger::process_logs()
{
	std::unique_lock<std::mutex> lock(queue_mtx);

	while (true)
	{
		// release lock, wait for a message to be in the queue
		wait_var.wait(lock, [&]() { return exit_flag || !log_queue.empty(); });

		if (exit_flag && log_queue.empty()) { 
			done_processing = true;
			wait_var.notify_all();
			break;
		}

		while (!log_queue.empty()) {
			Log log_entry = log_queue.front();
			log_queue.pop_front();

			wait_var.notify_all();

			// TODO: add frame number
			lock.unlock();
			char stamp_buffer[STAMP_SIZE_T];
			BTime::format_stamp_ns(log_entry.timestamp_ns, stamp_buffer);

			log_file
				<< '['
				<< stamp_buffer
				<< ']'
				<< log_level_string(log_entry.lvl)
				<< ": "
				<< log_entry.msg << '\n';
			lock.lock();
		}
	}
}


void Logger::flush_log_queue()
{
	std::unique_lock<std::mutex> lock(queue_mtx);
	wait_var.wait(lock, [&]() { return exit_flag || log_queue.empty(); });
}
