#ifndef UTILITY_HPP_INCLUDED
#define UTILITY_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <boost/program_options.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace po = boost::program_options;

namespace Utility {
    uint16_t rndInt(uint16_t, uint16_t);
    float rndFloat(float, float);
	std::vector<uint16_t> splitSize (const std::string&);
    po::variables_map usage ( int&, char**);
    sf::Shape RoundedRectangle(float P1X, float P1Y, float P2X, float P2Y,
                               float radius, const sf::Color& Col,
                               float Outline=0.f, const sf::Color& OutlineCol=sf::Color(0,0,0,0));
}


#endif
