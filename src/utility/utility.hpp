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
    // Split size_str seperated by 'x' into vector
    // Example: split "4x4" to int{4,4}
	std::vector<sf::Uint16> splitSize(const std::string& size_str);

    // For boost/program_options. 
    // Returns a variable map with options specified as arguments.
    po::variables_map usage(int& ac, char* av[]);

    // Create a rounded rectangle
    sf::Shape RoundedRectangle(float P1X, float P1Y, float P2X, float P2Y,
                               float radius, const sf::Color& Col,
                               float Outline=0.f, const sf::Color& OutlineCol=sf::Color(0,0,0,0));
}


#endif
