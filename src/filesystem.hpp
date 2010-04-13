#ifndef FILESYSTEM_HPP_INCLUDED
#define FILESYSTEM_HPP_INCLUDED

#include <boost/filesystem.hpp>

#include <SFML/System.hpp>

#include <string>

namespace Filesystem {
    typedef boost::filesystem::path path;
	sf::Uint8 make_dir(const std::string &dir);
	sf::Uint8 remove_file(const std::string &file);
	sf::Uint8 copy_file(const std::string &source, const std::string &dest);
	boost::filesystem::path get_cwd();
}

#endif
