#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

#include <SFML/Network.hpp>

#include "utility.hpp"

namespace Utility {
    sf::Uint16 rndInt(sf::Uint16 min, sf::Uint16 max) {
        return sf::Randomizer::Random(min, max);
    }
    float rndFloat(float min, float max) {
        return sf::Randomizer::Random(min, max);
    }

	std::vector<sf::Uint16> splitSize(const std::string& size_str) {
		std::vector<sf::Uint16> size_vec;
        sf::Uint8 sep;
		std::string width,height;

		sep = size_str.find("x");

		width = size_str.substr(0,sep);
		height = size_str.substr(sep+1);

		size_vec.push_back(boost::lexical_cast<sf::Uint16>(width));
		size_vec.push_back(boost::lexical_cast<sf::Uint16>(height));

		return size_vec;
	}

	po::variables_map usage(int& ac, char* av[]) {
		// Declare the supported options.
		po::options_description desc("OpenFala multiplayer tower strategy game.\
		\n\nAllowed options");
		desc.add_options()
		    ("help,h", "this message, obviously")
		    ("mode,m", po::value<std::string>()->default_value("normal"),
		        "choose mode <modes: 'client', 'server', 'both', 'normal'>")
		    ("address,a", po::value<std::string>()->default_value("127.0.0.1"),
		    	"get to connect to (ignored for server) <format: xxx.xxx.xxx.xxx>")
		    ("name,n", po::value<std::string>()->default_value("loldude"),
		    	"player name (ignored for server)")
		    ("port,p", po::value<sf::Uint16>()->default_value(41311),
		    	"port to bind to/connect to")
		    ("size,s", po::value<std::string>()->default_value("800x600"),
		    	"sets the window size <e.g.: 800x600>")
		    ("maxplayers,p", po::value<sf::Uint16>()->default_value(4),
		    	"maximum number of players (ignored for client)")
		    //("verbose,v", po::value<sf::Uint16>()->implicit_value(1), "be verbose")
		    //("quiet,q", po::value<sf::Uint16>()->implicit_value(1), "be quiet")
		;

		po::variables_map vm;
		po::store(po::parse_command_line(ac, av, desc), vm);
		po::notify(vm);

		if (vm.count("help")) {
		    std::cout << desc << "\n";
		    exit(1);
		}

		std::string mode = vm["mode"].as<std::string>();
		if ((mode != "client") and (mode != "server") and (mode != "both") and (mode != "normal")) {
			std::cout << "ERROR: You need to provide a valid mode!";
			exit(1);
		}

		sf::IPAddress address(vm["address"].as<std::string>());
        if (!address.IsValid()) {
			std::cout << "ERROR: Invalid address! " << std::endl;
			exit(1);
		}

		return vm;
	}
}
