#ifndef SERVICE_LOG_H
#define SERVICE_LOG_H

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

#define LOG_DEBUG	BOOST_LOG_SEV(logger::log, boost::log::trivial::debug)
#define LOG_INFO	BOOST_LOG_SEV(logger::log, boost::log::trivial::info)
#define LOG_WARNING	BOOST_LOG_SEV(logger::log, boost::log::trivial::warning)
#define LOG_ERROR	BOOST_LOG_SEV(logger::log, boost::log::trivial::error)

namespace logger {
	extern boost::log::sources::severity_logger<boost::log::trivial::severity_level> log;
	void init(std::wstring directory);
}

#endif