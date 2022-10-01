#include "ServiceLog.h"

namespace logger {
	boost::log::source::severity_logger<boost::log::trivial::severity_level> log;
	void init(std::wstring directory)
	{
		boost::log::add_file_log
		{
			boost::log::keywords::file_name = directory + L"\\[%Y-%m-%d][%H-%M-%S]ServiceBasic_%N.log",
			boost::log::keywords::target = directory,
			boost::log::keywords::rotation_size = 16 * 1024 * 1024,
			boost::log::keywords::min_free_space = 64 * 1024 * 1024,
			boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
			boost::log::keywords::format = "[%TimeStamp%][%Severity%]: %Message%",
			boost::log::keywords::auto_flush = true;
		};
		boost::log::add_common_attributes();
	}
}
