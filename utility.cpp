#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

#include <SFML/Network.hpp>

#include "utility.hpp"

namespace Utility {
    uint16_t rndInt(uint16_t min, uint16_t max) {
        return sf::Randomizer::Random(min, max);
    }
    float rndFloat(float min, float max) {
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
		    ("name,n", po::value<std::string>()->default_value("loldude"),
		    	"player name (ignored for server)")
		    ("port,p", po::value<uint16_t>()->default_value(41312),
		    	"port to bind to")
		    ("size,s", po::value<std::string>()->default_value("800x600"),
		    	"sets the window size <e.g.: 800x600>")
		    ("maxplayers,p", po::value<uint16_t>()->default_value(4),
		    	"maximum number of players (ignored for client)")
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

		std::string mode = vm["mode"].as<std::string>();
		if ((mode != "client") and (mode != "server") and (mode != "both")) {
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
