#include "utility.hpp"

namespace utility {
    uint16_t rnd_int(uint16_t min, uint16_t max) {
        return sf::Randomizer::Random(min, max);
    }
    float rnd_float(float min, float max) {
        return sf::Randomizer::Random(min, max);
    }


	ResourceLoader::ResourceLoader() {};
	ResourceLoader::~ResourceLoader() {};
	uint8_t ResourceLoader::AddImage(const filesystem::path& path, const std::string& imgname,
		const uint16_t& width, const uint16_t& height) {
		Magick::Image image;
		sf::Image sfimage;
		try {
			std::cout << "Caching image at: " << path << std::endl;
			image.read(operator/(path, imgname).string());
			std::cout << operator/(path, imgname).string() << std::endl;
			filesystem::make_dir(operator/(path, "cached").string());
			Magick::Geometry ss(width, height);
			image.sample(Magick::Geometry(width, height));
			image.write(operator/(operator/(path, "cached/"),imgname).string());
			sfimage.LoadFromFile(operator/(operator/(path, "cached/"),imgname).string());
			std::string basename = boost::filesystem::basename (operator/(operator/(path, "cached/"),imgname));
			ResourceLoader::images.insert(std::pair<std::string, sf::Image>(basename, sfimage));
		}
		catch (std::exception &error_) {
			std::cout << "ERROR: " << error_.what() << std::endl;
			return 1;
		}
		return 0;
	}
	sf::Image* ResourceLoader::GetImage(const std::string& img) {
	    sf::Image *a = &ResourceLoader::images[img];
        return a;
	};
}
