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

	sf::Shape RoundedRectangle(float P1X, float P1Y, float P2X, float P2Y,
                               float radius, const sf::Color& Col,
                               float Outline, const sf::Color& OutlineCol){

        // radius can't be bigger than half of a side
        if (2*radius > abs(P1X-P2X))
            radius = abs(P1X-P2X) / 2;
        if (2*radius > abs(P1Y-P2Y))
            radius = abs(P1Y-P2Y) / 2;

        static const int NbSegments = 40;
        static const float PI = 3.141592654f;

        sf::Shape S;
        sf::Vector2f TL(P1X+radius,P1Y+radius);
        sf::Vector2f TR(P2X-radius,P1Y+radius);
        sf::Vector2f BL(P1X+radius,P2Y-radius);
        sf::Vector2f BR(P2X-radius,P2Y-radius);

        // Assemble visual shape
        for (int i = 0; i <= NbSegments; ++i) {
            float Angle = i * 2 * PI / NbSegments;

            sf::Vector2f Offset(cos(Angle), sin(Angle));

            // Move quarter circles to Rectangle corners (TL, TB, RL, RB)
            if ((Angle >= 0 or Angle>=2*PI)     and  Angle <= PI/2)
                S.AddPoint(BR + Offset * radius, Col, OutlineCol);
            if (Angle >= PI/2   and  Angle <= PI)
                S.AddPoint(BL + Offset * radius, Col, OutlineCol);
            if (Angle >= PI     and  Angle <= PI*1.5)
                S.AddPoint(TL + Offset * radius, Col, OutlineCol);
            if (Angle >= PI*1.5 and  Angle <= 2*PI)
                S.AddPoint(TR + Offset * radius, Col, OutlineCol);
        }
        S.SetOutlineWidth(Outline);
        return S;
    }



    sf::Shape GradientRectangle(float P1X, float P1Y, float P2X, float P2Y,
                                const sf::Color& Col1, const sf::Color& Col2,
                                float Outline, const sf::Color& OutlineCol1,
                                const sf::Color& OutlineCol2){

        sf::Shape S;
        S.AddPoint( sf::Vector2f(P1X, P1Y) , Col1, OutlineCol1);  // Top left
        S.AddPoint( sf::Vector2f(P1X, P2Y) , Col2, OutlineCol2);  // Bottom left
        S.AddPoint( sf::Vector2f(P2X, P2Y) , Col2, OutlineCol2);  // Bottom right
        S.AddPoint( sf::Vector2f(P2X, P1Y) , Col1, OutlineCol1);  // Top right
        S.SetOutlineWidth(Outline);
        return S;
    }
}
