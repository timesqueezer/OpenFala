#include <iostream>

#include "resources.hpp"

ResourceManager::ResourceManager() {};
ResourceManager::~ResourceManager() {};

bool ResourceManager::AddImage(const Filesystem::path& path, const std::string& imgname,
    const sf::Uint16& width, const sf::Uint16& height) {
    Magick::Image mimage;
    sf::Image sfimage;
    std::string basename = boost::filesystem::basename(path / "cached/" / imgname);

    if(m_images.count(basename) != 0)
        return true;

    try {
        std::cout << "Caching image ";
        mimage.backgroundColor(Magick::Color(0,0,0,65535));
        mimage.read((path / imgname).string());
        std::cout << path / imgname << std::endl;
        Filesystem::make_dir((path / "cached").string());
        Magick::Geometry ss(width, height);
        mimage.sample(Magick::Geometry(width, height));
        mimage.depth(8);

        mimage.write((path / "cached/" / basename).string()+".png");
        sfimage.LoadFromFile((path / "cached/" / basename).string()+".png");
        sfimage.SetSmooth(false);
        m_images.insert(std::pair<std::string, sf::Image>(basename, sfimage));
    }
    catch (std::exception &error_) {
        std::cout << "ERROR: " << error_.what() << std::endl;
        return true;
    }
    return false;
}

sf::Image* ResourceManager::GetImage(const std::string& img) {
    sf::Image *a = &ResourceManager::m_images[img];
    return a;
};
