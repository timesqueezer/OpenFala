#include <SFML/System.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/multi_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <string>

#include "../server/server.hpp"
#include "../block/block.hpp"
#include "../utility/utility.hpp"
#include "../network/network.hpp"
#include "../entity/Building.hpp"

#include "gameengine.hpp"
#include "gamestate.hpp"
#include "playstate.hpp"
#include "mainmenustate.hpp"


PlayState PlayState::m_PlayState;

void PlayState::Init(GameEngine* game){
    std::cout << "+ Init PlayState" << std::endl;

    mGameEngine = game;

    std::cout << "Init PlayState" << std::endl;

    mInput = & (mGameEngine->app.GetInput());
    mGameEngine->app.SetFramerateLimit(60);

    std::string size = mGameEngine->mProgramOptions["size"].as<std::string>();
    mGameEngine->m_width = Utility::splitSize(size)[0];
	mGameEngine->m_height = Utility::splitSize(size)[1];
	m_bindaddress = mGameEngine->mProgramOptions["address"].as<std::string>();
	m_port = mGameEngine->mProgramOptions["port"].as<sf::Uint16>(); // port for outgoing communication (default 41311)
    m_clientport = 41312; // port for incoming communication
	m_name = mGameEngine->mProgramOptions["name"].as<std::string>();;

	inforect = sf::Shape::Rectangle(0, mGameEngine->app.GetHeight() - 40, mGameEngine->app.GetWidth(),
                                 mGameEngine->app.GetHeight(), sf::Color(20, 20, 20));


    //  ============================

    Socket.Bind(m_clientport); // port for incoming communication
    Socket.SetBlocking(false);

	m_ratio = mGameEngine->m_height / 15;

    // Load images
	ResourceManager& ResMgr = ResourceManager::get_mutable_instance();

	ResMgr.AddImage("data/images/", "block-grass.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "block-dirt.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "block-lava.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "block-rock.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "tower-generic.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "highlight-orange.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "highlight-blue.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "highlight-green.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "highlight-red.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "building.svg", m_ratio*2, m_ratio*2);

	ResMgr.AddImage("data/images/", "cloud01.svg", 3*m_ratio, 3*m_ratio);
	ResMgr.AddImage("data/images/", "cloud02.svg", 3*m_ratio, 3*m_ratio);
	ResMgr.AddImage("data/images/", "cloud03.svg", 3*m_ratio, 3*m_ratio);
	ResMgr.AddImage("data/images/", "cloud04.svg", 3*m_ratio, 3*m_ratio);
	ResMgr.AddImage("data/images/", "cloud05.svg", 3*m_ratio, 3*m_ratio);

	m_blocknbx = mGameEngine->app.GetWidth() / m_ratio;
	m_blocknby = mGameEngine->app.GetHeight() / m_ratio;

    m_mpos.resize(extents[4]);

    //Initialisation of the Shapes to show each player

    m_mpos[0] = new Block(-m_ratio, 0.f, ResMgr.GetImage("highlight-red"), BLOCKTYPE_EMPTY);
    m_mpos[1] = new Block(-m_ratio, 0.f, ResMgr.GetImage("highlight-blue"), BLOCKTYPE_EMPTY);
    m_mpos[2] = new Block(-m_ratio, 0.f, ResMgr.GetImage("highlight-green"), BLOCKTYPE_EMPTY);
    m_mpos[3] = new Block(-m_ratio, 0.f, ResMgr.GetImage("highlight-orange"), BLOCKTYPE_EMPTY);

	m_blocks.resize(extents[m_blocknbx+1][m_blocknby+1]);
	int i = 0;
	for (sf::Uint8 x = 0;x < m_blocknbx;++x) {
		for (sf::Uint8 y = 0;y <= m_blocknby;++y) {
            if ((x < (m_blocknbx - 20) / 2) or (x > (m_blocknbx - ((m_blocknbx - 20) / 2)-1 ))) {
            	// This is to create only m_blocks in the non playable area
            	if (y < 10) {
//					m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("block-sky"), 1);
				} else if (y == 10) {
                    m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("block-lava"), BLOCKTYPE_GROUND);
				} else {
					m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("block-rock"), BLOCKTYPE_GROUND);
		        }
            } else {
                // This is the playable area
            	if (y < 10) {
//					m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("block-sky"), 1);
				} else if (y == 10) {
                    m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("block-grass"), BLOCKTYPE_GROUND);
				} else {
					m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("block-dirt"), BLOCKTYPE_GROUND);
		        }
            }
        }
        ++i;
	}

    // Create Sky Rectangle
    mSkyRect = Utility::GradientRectangle(0,0,mGameEngine->app.GetWidth(),10*m_ratio,
                                          //sf::Color(0,0,60), sf::Color(60,60,60) );
                                          sf::Color(128,128,255), sf::Color(255,240,240) );


    // Create Clouds
    sf::Uint8 numClouds = sf::Randomizer::Random(10,30);
    for (sf::Uint8 i = 0; i<numClouds; ++i){

        sf::Uint16 x = sf::Randomizer::Random(-3*m_ratio,mGameEngine->app.GetWidth() + 3*m_ratio);
        sf::Uint16 y = sf::Randomizer::Random(-3*m_ratio, 6*m_ratio);

        std::string cloudName = "cloud0" + boost::lexical_cast<std::string>(sf::Randomizer::Random(1,5));
        // Create Cloud Sprite
        mClouds.push_back( new sf::Sprite(*ResMgr.GetImage(cloudName), sf::Vector2f(x-100,y)) );
    }

    m_cl_id = 0;
	mode = 1; // set to build mode

    // Server handshake
    SendPacket << PACKET_HANDSHAKE << m_name;
    Socket.Send(SendPacket, m_bindaddress, m_port);
    SendPacket.Clear();
}

void PlayState::Cleanup(){
    std::cout << "- Cleanup PlayState" << std::endl;
    SendPacket << PACKET_GOODBYE << m_cl_id;
    Socket.Send(SendPacket, m_bindaddress, m_port);
    SendPacket.Clear();
    Socket.Close();
}

void PlayState::Pause(){
}

void PlayState::Resume(){
}

void PlayState::HandleEvents(){
    sf::Event Event;
	while (mGameEngine->app.GetEvent(Event)) {
		if (Event.Type == sf::Event::Closed) {
			mGameEngine->Cleanup();
		}
        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape)) {
            // if Escape is pressed, the Main Menu is shown
           	mGameEngine->ChangeState( MainMenuState::Instance() );
        }
        if (Event.Type == sf::Event::MouseButtonPressed) {
            if (mode==1) {
                if (MouseInPlayableArea()) {
                    SendPacket << PACKET_BUILD << (sf::Uint16) ((GetMouseBlock('x','p') - (mGameEngine->m_width / m_ratio - 20)/2)) << GetMouseBlock('y','p');
                    Socket.Send(SendPacket, m_bindaddress, m_port);
                    SendPacket.Clear();
                }
            }
		}
		if (Event.Type == sf::Event::MouseMoved) { // What happens when the mouse is moved
            float mouse_x = (m_mpos[0]->Sprite.GetPosition().x)/m_ratio;
		    float mouse_y = (m_mpos[0]->Sprite.GetPosition().y)/m_ratio;

            if ((GetMouseBlock('x', 'p') != mouse_x) or (GetMouseBlock('y', 'p') != mouse_y)) {
		    	SendPacket << PACKET_MOUSE << (sf::Uint16) (GetMouseBlock('x', 'p')+GetNonPlayableAreaSize()) << GetMouseBlock('y', 'p');
		    	Socket.Send(SendPacket, m_bindaddress, m_port);
		    	SendPacket.Clear();
		    	m_mpos[m_cl_id]->Sprite.SetPosition((GetMouseBlock('x', 'p')+GetNonPlayableAreaSize())*m_ratio, GetMouseBlock('y', 'p')*m_ratio);
		    }
		}
	}
}

void PlayState::Update(){
    sf::Uint16 request_id = 0;
	sf::Uint16 posx = 0;
	sf::Uint16 posy = 0;
	sf::Uint16 cl_id = 0;
    sf::IPAddress svaddress;

    unsigned short svport;


    if (Socket.Receive(RecvPacket, svaddress, svport) == sf::Socket::Done) {
        RecvPacket >> request_id;
        if (request_id == PACKET_BUILD) { // stands for placing a block
            RecvPacket >> posx >> posy >> cl_id;
            PlaceBlock((int) posx, (int) posy);
        }
        if (request_id == PACKET_MOUSE) { //stands for getting the mouse positions
            sf::Uint16 num_clients;
            RecvPacket >> num_clients;
            for (sf::Uint16 i = 0; i < num_clients; ++i) {
                    RecvPacket >> cl_id >> posx >> posy;
                    if (cl_id != m_cl_id) {
                        m_mpos[cl_id]->Sprite.SetPosition((float) posx*m_ratio, (float) posy*m_ratio);
                    }
            }
        }
        if (request_id == PACKET_HANDSHAKE) { // init stuff
            RecvPacket >> cl_id;
            m_cl_id = cl_id;
        }
        //std::cout << request_id << " " << posx << " " << posy << std::endl;
        RecvPacket.Clear();
    }

    // Update World
    mGameEngine->GetWorld().Update();


    // Move clouds
    BOOST_FOREACH(sf::Drawable& cloud , mClouds){
        float x = cloud.GetPosition().x - 0.7;
        if (x<-3*m_ratio) x = mGameEngine->app.GetWidth();
        cloud.SetX(x);

    }
}

void PlayState::Draw(){
    // Clear the screen
    mGameEngine->app.Clear(sf::Color(0,0,0));

    // Draw sky
    mGameEngine->app.Draw(mSkyRect);

    // Draw clouds
    BOOST_FOREACH(sf::Drawable& cloud , mClouds){
        mGameEngine->app.Draw(cloud);
    }

    sf::Uint16 framerate = (sf::Uint16) (1.f / mGameEngine->app.GetFrameTime());

	for(sf::Uint8 x = 0;x<m_blocknbx;++x) {
		for(sf::Uint8 y = 0;y<m_blocknby;++y) {
			if(m_blocks[x][y]!=0 && mode == 1) {
                mGameEngine->app.Draw(m_blocks[x][y]->Sprite);
			}
		}
	}

    // Draw the whole World
	mGameEngine->GetWorld().Draw(mGameEngine->app, m_ratio, sf::Vector2f(0,0));

    for(sf::Uint8 x = 0; x < 4; ++x) {
        mGameEngine->app.Draw(m_mpos[x]->Sprite);
    }


	mGameEngine->app.Draw(inforect);
    fps.SetText("FPS: "+boost::lexical_cast<std::string>(framerate));
	fps.SetPosition(10, mGameEngine->app.GetHeight() - 40);
	mGameEngine->app.Draw(fps);
		mousepos.SetText("MouseX: "+boost::lexical_cast<std::string>(GetMouseBlock('x','p'))+
                       	" MouseY: "+boost::lexical_cast<std::string>(GetMouseBlock('y','p'))+
                        " ID: "+boost::lexical_cast<std::string>(m_cl_id));
	mousepos.SetPosition(150, mGameEngine->app.GetHeight() - 40);
	mGameEngine->app.Draw(mousepos);
}

sf::Uint8 PlayState::GetNonPlayableAreaSize() {
   sf::Uint8 size = (m_blocknbx -20) / 2;
   return size;
}

sf::Uint16 PlayState::GetMouseBlock(char xy, char type) {
    sf::Uint16 block;
    if (type == 'p') { // this stands for playable area
        if (xy == 'x') {
            block = ((mInput->GetMouseX() / m_ratio) - ((m_blocknbx - 20)/2));
            if (block <= 0) {
                block = 0;
            }
        } else {
            block = (mInput->GetMouseY() / m_ratio);
        }
    } else {
        if (xy == 'x') {
            block = (mInput->GetMouseX() / m_ratio);
        } else {
            block = (mInput->GetMouseY() / m_ratio);
        }
    }
    return block;
}

bool PlayState::MouseInPlayableArea() {
    return InPlayableArea(mInput->GetMouseX(), mInput->GetMouseY());
}

bool PlayState::InPlayableArea(int x, int y) {
    bool is_left_of_area  = x < ((mGameEngine->m_width-20*m_ratio)/2) ;
    bool is_right_of_area = x > ((mGameEngine->m_width-(mGameEngine->m_width - 20*m_ratio)/2));
    bool is_underground   = y > 10*m_ratio;

    return !(is_left_of_area or is_right_of_area or is_underground);
}

void PlayState::PlaceBlock(int x, int y) {
    if (!InPlayableArea(x,y)) {
        std::cout << "Cannot place the block right there!";
    } else {
        if (m_blocks[x][y+1]!=0 && m_blocks[x][y+1]->m_type != BLOCKTYPE_EMPTY) {
            Building* b = new Building();
            b->SetImageKey("building");
            b->SetPosition(sf::Vector2f(x,y));
            b->SetDimension(sf::Vector2f(m_ratio, m_ratio));
            mGameEngine->GetWorld().AddEntity(b);

            //m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("building"), BLOCKTYPE_TOWER);
        } else {
            std::cout << "Can't place blocks in the air." << std::endl;
        }
    }
    return;
}
