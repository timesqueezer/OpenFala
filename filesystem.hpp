#ifndef FILESYSTEM_HPP_INCLUDED
#define FILESYSTEM_HPP_INCLUDED

#include <boost/filesystem.hpp>

#include <string>

namespace filesystem {
	typedef boost::filesystem::path path;
	uint8_t make_dir(const std::string &dir);
	uint8_t remove_file(const std::string &file);
	uint8_t copy_file(const std::string &source, const std::string &dest);
	boost::filesystem::path get_cwd();
}

#endif
