#ifndef UTILITY_HPP_INCLUDED
#define UTILITY_HPP_INCLUDED

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <ImageMagick/Magick++.h>

#include <boost/cstdint.hpp>

#include <string>
#include <map>
#include <iostream>

#include "filesystem.hpp"

namespace utility {
    uint16_t rnd_int(uint16_t, uint16_t);
    float rnd_float(float, float);

    class ResourceLoader {
    	public:
    		ResourceLoader();
    		~ResourceLoader();
    		uint8_t AddImage(const filesystem::path&, const std::string&,
    			const uint16_t&, const uint16_t&);
    		sf::Image* GetImage(const std::string&);
    	private:
    		std::map<std::string, sf::Image> images;
	};
}

#endif
