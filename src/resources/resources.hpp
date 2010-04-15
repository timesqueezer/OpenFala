#ifndef RESOURCES_HPP_INCLUDED
#define RESOURCES_HPP_INCLUDED

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <ImageMagick/Magick++.h>
#include <boost/serialization/singleton.hpp>
#include <boost/lexical_cast.hpp>
#include <map>

#include "../filesystem/filesystem.hpp"

class ResourceManager : public boost::serialization::singleton<ResourceManager> {
public:
    ResourceManager();
    ~ResourceManager();
    bool AddImage(const Filesystem::path& path, const std::string& imgname,
        const sf::Uint16& width, const sf::Uint16& height, std::string key="");
    sf::Image* GetImage(const std::string&);
private:
    std::map<std::string, sf::Image> m_images;
};

#endif
