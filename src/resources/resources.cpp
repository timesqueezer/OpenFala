#include <iostream>

#include "resources.hpp"

ResourceManager::ResourceManager() {};
ResourceManager::~ResourceManager() {};

bool ResourceManager::AddImage(const Filesystem::path& path, const std::string& imgname,
    const sf::Uint16& width, const sf::Uint16& height, std::string key) {

    // create Original File Path
    std::string originalFile = (path / imgname).string();

    // if the optional param key is not given, use the basename as key
    if(key == "") {
        key = boost::filesystem::basename(originalFile);
    }

    // Create Cache Paths
    Filesystem::path cacheDir = (path / "cached").string();
    std::string cacheFile = (cacheDir / key).string()+".png";

    // if an image with that key already exists in the dictionary, return
    if(m_images.count(key) != 0) {
        return true;
    }

    try {
        // Log output
        std::cout << "Caching image " << originalFile << std::endl;

        // Create cache directory
        Filesystem::make_dir(cacheDir.string());

        // Load, convert and save image (originalFile > cacheFile)
        Magick::Image mimage;
        mimage.backgroundColor(Magick::Color(0,0,0,65535));
        mimage.read(originalFile);
        mimage.zoom(Magick::Geometry(width, height));
        mimage.depth(8);
        mimage.write(cacheFile);

        // Load cached File
        sf::Image sfimage;
        sfimage.LoadFromFile(cacheFile);
        sfimage.SetSmooth(false);

        // Save loaded Image in Dictionary
        m_images.insert(std::pair<std::string, sf::Image>(key, sfimage));
    }
    catch (std::exception &error_) {
        std::cout << "ERROR: " << error_.what() << std::endl;
        return false;
    }
    return true;
}

sf::Image* ResourceManager::GetImage(const std::string& img) {
    sf::Image *a = &ResourceManager::m_images[img];
    return a;
};
