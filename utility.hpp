#ifndef UTILITY_HPP_INCLUDED
#define UTILITY_HPP_INCLUDED

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <ImageMagick/Magick++.h>

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "filesystem.hpp"

namespace po = boost::program_options;

namespace utility {
    uint16_t rndInt(uint16_t, uint16_t);
    float rndFloat(float, float);
	std::vector<uint16_t> splitSize (const std::string&);
    po::variables_map usage ( int&, char**);

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
