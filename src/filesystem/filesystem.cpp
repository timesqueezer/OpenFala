#include <boost/filesystem.hpp>

#include "filesystem.hpp"

namespace Filesystem {
	sf::Uint8 make_dir(const std::string &dir) {
		if (!boost::filesystem::exists(dir)) {
			boost::filesystem::create_directory(dir);
			return 0;
		}
		else {
            return 1;
		}
	}

	sf::Uint8 remove_file(const std::string &file) {
		if (boost::filesystem::exists(file)) {
			boost::filesystem::remove(file);
			return 0;
		}
		else {
            return 1;
		}
	}

	sf::Uint8 copy_file(const std::string &source, const std::string &dest) {
		if (!boost::filesystem::exists(dest)) {
			if (boost::filesystem::exists(source)) {
				boost::filesystem::copy_file(source, dest);
				return 0;
			}
            else {
                return 1;
            }
		}
		else {
            return 1;
		}
	}

    boost::filesystem::path get_cwd() {
        boost::filesystem::path path(boost::filesystem::current_path());
        return path;
    }
}
