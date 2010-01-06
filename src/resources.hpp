#ifndef RESOURCES_HPP_INCLUDED
#define RESOURCES_HPP_INCLUDED

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <ImageMagick/Magick++.h>

#include <boost/lexical_cast.hpp>

#include <map>

#include "filesystem.hpp"

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();
    uint8_t AddImage(const Filesystem::path&, const std::string&,
        const uint16_t&, const uint16_t&);
    sf::Image* GetImage(const std::string&);
private:
    std::map<std::string, sf::Image> m_images;
};

#endif
