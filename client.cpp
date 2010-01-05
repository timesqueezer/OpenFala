#include <SFML/System.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/multi_array.hpp>

#include <iostream>
#include <string>

#include "client.hpp"
#include "server.hpp"
#include "network.hpp"
#include "block.hpp"
#include "utility.hpp"


ClientApp::ClientApp(const uint16_t& p, const Network::IPAddress& bind_ad,
						const std::string& size, const std::string& name) {
	// app.Create(sf::VideoMode::GetMode(0), "OpenFala", 0,
	// sf::WindowSettings::WindowSettings ( 24, 8, 4));
	width = utility::splitSize(size)[0];
	height = utility::splitSize(size)[1];
	app.Create(sf::VideoMode(width, height, 32), "OpenFala");
	input = &app.GetInput();
	bind_address = bind_ad;
	port = 41311; // port to send
	m_name = name;

	#ifdef DEBUG

    debugcircle = sf::Shape::Circle(100, 100, 50, sf::Color(128, 192, 255));
	inforect = sf::Shape::Rectangle(0, app.GetHeight() - 40, app.GetWidth(),
                                 app.GetHeight(), sf::Color(20, 20, 20));
	#endif

    Socket.Bind(41312); // port to bind
    Socket.SetBlocking(false);
	app.SetFramerateLimit(60);

	ratio = height / 15;

	// Loading Images

	path = filesystem::get_cwd();
	Loader = utility::ResourceLoader::ResourceLoader();
	Loader.AddImage(operator/(path, "data/images/").string(), "block-rock.png", ratio, ratio);
	Loader.AddImage(operator/(path, "data/images/").string(), "block-sky.png", ratio, ratio);
	Loader.AddImage(operator/(path, "data/images/").string(), "block-grass.png", ratio, ratio);
	Loader.AddImage(operator/(path, "data/images/").string(), "block-dirt.png", ratio, ratio);
	Loader.AddImage(operator/(path, "data/images/").string(), "block-lava.png", ratio, ratio);
	Loader.AddImage(operator/(path, "data/images/").string(), "test.png", ratio, ratio);
	Loader.AddImage(operator/(path, "data/images/debug/").string(), "sky.png", ratio, ratio);
	Loader.AddImage(operator/(path, "data/images/debug/").string(), "grass.png", ratio, ratio);
	Loader.AddImage(operator/(path, "data/images/debug/").string(), "ground.png", ratio, ratio);
	Loader.AddImage(operator/(path, "data/images/debug/").string(), "sky-out.png", ratio, ratio);
	Loader.AddImage(operator/(path, "data/images/debug/").string(), "grass-out.png", ratio, ratio);
	Loader.AddImage(operator/(path, "data/images/debug/").string(), "ground-out.png", ratio, ratio);
	Loader.AddImage(operator/(path, "data/images/debug/").string(), "highlight.png", ratio, ratio);

	blocknbx = app.GetWidth() / ratio;
	blocknby = app.GetHeight() / ratio;
	std::cout << blocknbx << " " << blocknby << " " << ratio;

	m_blocks.resize(extents[blocknbx][blocknby]);
	int i = 0;
	for (short unsigned int x = 0;x < blocknbx;++x) {
		for (short unsigned int y = 0;y<blocknby;++y) {

            if ((x < (blocknbx - 20) / 2) or (x > (blocknbx - ((blocknbx - 20) / 2) ))) {
            	// This is to create only m_blocks in the non playable area
            	if (y < 10) {
					m_blocks[x][y] = new Block(x*ratio, y*ratio, Loader.GetImage("sky-out"));
				} else if (y == 10) {
				m_blocks[x][y] = new Block(x*ratio, y*ratio, Loader.GetImage("grass-out"));
				} else {
					m_blocks[x][y] = new Block(x*ratio, y*ratio, Loader.GetImage("ground-out"));
		        }

            } else {
            	// TODO: other graphics here, this is the playable area
            	if (y < 10) {
					m_blocks[x][y] = new Block(x*ratio, y*ratio, Loader.GetImage("sky"));
				} else if (y == 10) {
				m_blocks[x][y] = new Block(x*ratio, y*ratio, Loader.GetImage("grass"));
				} else {
					m_blocks[x][y] = new Block(x*ratio, y*ratio, Loader.GetImage("ground"));
		        }
            }
        }
			++i;
	}

	highlightblock = new Block(0, 0, Loader.GetImage("highlight"));

	mode = 1; // set to build mode
}

ClientApp::~ClientApp() {}

void ClientApp::Update() {
	sf::Event Event;
	while (app.GetEvent(Event)) {
		if (Event.Type == sf::Event::Closed) {
			app.Close();
		}
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape)) {
           	app.Close();
           }
		if (Event.Type == sf::Event::MouseButtonPressed) {
            if (mode==1) {
                if (MouseInPlayableArea()) {
                    Packet << m_name << (sf::Uint16) (GetMouseBlock('x') - (width / ratio - 20)/2) << GetMouseBlock('y') << "rolf";
                    Socket.Send(Packet, bind_address, port);
                    Packet.Clear();
                }
            }
		}
		if (Event.Type == sf::Event::MouseMoved) {
			Packet << m_name << (sf::Uint16) input->GetMouseX() << (sf::Uint16) input->GetMouseY() << "mouse";
			Socket.Send(Packet, bind_address, port);
			Packet.Clear();
		}
	}

	// Network tests
    // std::cout << input->GetMouseX() << " - " << input->GetMouseY() << "\n";
	//Packet << (sf::Uint16)input->GetMouseX() << (sf::Uint16)input->GetMouseY();
	sf::Uint16 playerid;
	sf::Uint16 posx;
	sf::Uint16 posy;
    Network::IPAddress svaddress;
    unsigned short svport;
    GetSocket().Receive(RecvPacket, svaddress, svport);
    RecvPacket >> playerid >> posx >> posy;
    std::cout << playerid << " rolf " << posx << " " << posy << std::endl;
    RecvPacket.Clear();
}

void ClientApp::Draw() {
	uint16_t framerate = (uint16_t) (1.f / app.GetFrameTime());

	for (short unsigned int x = 0;x<blocknbx;++x) {
		for (short unsigned int y = 0;y<blocknby;++y) {
			if (mode == 1) {
				app.Draw(m_blocks[x][y]->Sprite);
			}
		}
	}
	highlightblock->SetPos((float)GetMouseBlock('x')*ratio, (float)GetMouseBlock('y')*ratio);
	app.Draw(highlightblock->Sprite);



	#ifdef DEBUG
		app.Draw(inforect);
		app.Draw(debugcircle);
		debugcircle.Move(0.5f, 0.5f);
			fps.SetText("FPS: "+boost::lexical_cast<std::string>(framerate));
		fps.SetPosition(10, app.GetHeight() - 40);
		app.Draw(fps);
			mousepos.SetText("MouseX: "+boost::lexical_cast<std::string>(GetMouseBlock('x') - (width / ratio - 20)/2)+
                         	" MouseY: "+boost::lexical_cast<std::string>(GetMouseBlock('y')));
		mousepos.SetPosition(150, app.GetHeight() - 40);
		app.Draw(mousepos);
	#endif
	app.Display();
}

sf::Uint16 ClientApp::GetMouseBlock(char xy) {
	if (xy == 'x') {
		return (sf::Uint16) (input->GetMouseX() / ratio);
	} else {
		return (sf::Uint16) (input->GetMouseY() / ratio);
	}
}

bool ClientApp::MouseInPlayableArea() {
    if ((input->GetMouseX()) < ((width-20*ratio)/2) or
       ((input->GetMouseX()) > ((width-(width - 20*ratio)/2)))) {
        #ifdef DEBUG
            std::cout << "false" << std::endl;
        #endif
        return false;
    }
    if ( input->GetMouseY() > 10*ratio ) {
        #ifdef DEBUG
            std::cout << "false" << std::endl;
        #endif
        return false;
    }
    return true;
}

Network::Socket& ClientApp::GetSocket() {
	return Socket;
}
