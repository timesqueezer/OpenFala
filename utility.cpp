#include "utility.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

#include <SFML/Network.hpp>

namespace utility {
    uint16_t rnd_int(uint16_t min, uint16_t max) {
        return sf::Randomizer::Random(min, max);
    }
    float rnd_float(float min, float max) {
        return sf::Randomizer::Random(min, max);
    }

	std::vector<uint16_t> splitSize (const std::string& size_str) {
		// split "4x4" to int{4,4}
		std::vector<uint16_t> size_vec;
		uint16_t sep;
		std::string width,height;

		sep = size_str.find("x");

		width = size_str.substr(0,sep);
		height = size_str.substr(sep+1);

		size_vec.push_back(boost::lexical_cast<uint16_t>(width));
		size_vec.push_back(boost::lexical_cast<uint16_t>(height));

		return size_vec;
	}

	ResourceLoader::ResourceLoader() {};
	ResourceLoader::~ResourceLoader() {};
	uint8_t ResourceLoader::AddImage(const filesystem::path& path, const std::string& imgname,
		const uint16_t& width, const uint16_t& height) {
		Magick::Image image;
		sf::Image sfimage;
		try {
			std::cout << "Caching image at: " << path << std::endl;
			image.read(operator/(path, imgname).string());
			std::cout << operator/(path, imgname).string() << std::endl;
			filesystem::make_dir(operator/(path, "cached").string());
			Magick::Geometry ss(width, height);
			image.sample(Magick::Geometry(width, height));
			image.write(operator/(operator/(path, "cached/"),imgname).string());
			sfimage.LoadFromFile(operator/(operator/(path, "cached/"),imgname).string());
			std::string basename = boost::filesystem::basename (operator/(operator/(path, "cached/"),imgname));
			ResourceLoader::images.insert(std::pair<std::string, sf::Image>(basename, sfimage));
		}
		catch (std::exception &error_) {
			std::cout << "ERROR: " << error_.what() << std::endl;
			return 1;
		}
		return 0;
	}
	sf::Image* ResourceLoader::GetImage(const std::string& img) {
	    sf::Image *a = &ResourceLoader::images[img];
        return a;
	};
	po::variables_map usage ( int& ac, char *av[] ) {
		// Declare the supported options.
		po::options_description desc("OpenFala multiplayer tower strategy game.\
		\n\nAllowed options");
		desc.add_options()
		    ("help,h", "this message, obviously")
		    ("mode,m", po::value<std::string>()->default_value("both"),
		        "choose mode <modes: 'client', 'server', 'both'>")
		    ("address,a", po::value<std::string>()->default_value("127.0.0.1"),
		    	"get to connect to (ignored for server) <format: xxx.xxx.xxx.xxx>")
		    ("port,p", po::value<uint16_t>()->default_value(1337),
		    	"port to bind to")
		    ("size,s", po::value<std::string>()->default_value("800x600"),
		    	"sets the window size <e.g.: 800x600>")
		    //("verbose,v", po::value<uint16_t>()->implicit_value(1), "be verbose")
		    //("quiet,q", po::value<uint16_t>()->implicit_value(1), "be quiet")
		;

		po::variables_map vm;
		po::store(po::parse_command_line(ac, av, desc), vm);
		po::notify(vm);

		if (vm.count("help")) {
		    std::cout << desc << "\n";
		    exit(1);
		}
		/*
		if (vm.count("client") == vm.count("server") == vm.count("both") == 0) {
			std::cout << "ERROR: You need to provide a valid mode!";
			exit(1);
		}
		*/
		//
		//std::string address = vm["address"].as<std::string>();
		//if (vm["address"].as<std::string>() == "empty") {
		//	vm["address"].as<std::string>() = sf::IPAddress::LocalHost.ToString();
		//}

		sf::IPAddress address(vm["address"].as<std::string>());
		if (!address.IsValid()) {
			std::cout << "ERROR: Invalid address! " << std::endl;
		}

		//if (! vm.at(vm["address"].as<string>()

		return vm;
	}

}
