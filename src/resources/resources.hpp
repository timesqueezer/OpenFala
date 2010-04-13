#ifndef RESOURCES_HPP_INCLUDED
#define RESOURCES_HPP_INCLUDED

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <ImageMagick/Magick++.h>

#include <boost/lexical_cast.hpp>

#include <map>

#include "../filesystem/filesystem.hpp"

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();
    bool AddImage(const Filesystem::path&, const std::string&,
                  const sf::Uint16&, const sf::Uint16&);
    sf::Image* GetImage(const std::string&);
private:
    std::map<std::string, sf::Image> m_images;
};

#endif
