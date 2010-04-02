#include <iostream>

#include "resources.hpp"

ResourceManager::ResourceManager() {};
ResourceManager::~ResourceManager() {};

uint8_t ResourceManager::AddImage(const Filesystem::path& path, const std::string& imgname,
    const uint16_t& width, const uint16_t& height) {
    Magick::Image mimage;
    sf::Image sfimage;
    std::string basename = boost::filesystem::basename(path / "cached/" / imgname);
    try {
        std::cout << "Caching image ";
        mimage.read((path / imgname).string());
        std::cout << path / imgname << std::endl;
        Filesystem::make_dir((path / "cached").string());
        Magick::Geometry ss(width, height);
        mimage.sample(Magick::Geometry(width, height));
        mimage.depth(8);
        mimage.write((path / "cached/" / basename).string()+".tga");
        sfimage.LoadFromFile((path / "cached/" / basename).string()+".tga");
        ResourceManager::m_images.insert(std::pair<std::string, sf::Image>(basename, sfimage));
    }
    catch (std::exception &error_) {
        std::cout << "ERROR: " << error_.what() << std::endl;
        return 1;
    }
    return 0;
}

sf::Image* ResourceManager::GetImage(const std::string& img) {
    sf::Image *a = &ResourceManager::m_images[img];
    return a;
};
