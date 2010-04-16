#ifndef PlayState_H
#define PlayState_H

#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/multi_array.hpp>
#include <boost/ptr_container/ptr_container.hpp>

#include <ImageMagick/Magick++.h>
#include <string>

#include "../gui/cpGUI.hpp"
#include "../filesystem/filesystem.hpp"
#include "../resources/resources.hpp"
#include "../block/block.hpp"

#include "gamestate.hpp"

typedef boost::multi_array<Block*, 2> blockarray2d;
typedef boost::multi_array<Block*, 1> blockarray1d;

class PlayState : public IGameState
{
public:
	void Init(GameEngine* game);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw();

	static PlayState* Instance() {
		return &m_PlayState;
	}

    sf::Uint8 GetNonPlayableAreaSize();

	// places a block to the specified area
    void PlaceBlock(int x, int y);
    void PlaceCannonTurret(int x, int y);
    void PlaceRocketLauncher(int x, int y);
    void PlacePowerGenerator(sf::Uint16 x, sf::Uint16 y);

    // Check if mouse is in legal area for building.
    bool MouseInPlayableArea();

    // check if the coordinates are in a legal area
    bool InPlayableArea(int, int);
    // Get the block beneath mouse cursor.
    sf::Vector2<sf::Uint16> GetMouseBlock(bool OriginIsPlayableArea);


protected:
	PlayState() { }

private:
	static PlayState m_PlayState;

    // Graphics
    sf::Shape mGuiActionBar;
    cp::cpGuiContainer mGui;
    sf::Shape mGuiModeIndicator;
    cp::cpImageButton *mGuiBuildModeButton;
    cp::cpImageButton *mGuiDemolishModeButton;

	sf::String mInfoLabel;
	sf::Shape mInfoBar;

	sf::Shape mSkyRect;
	boost::ptr_vector<sf::Sprite> mClouds;

	// Connection details
	sf::Uint16 m_port;
	sf::Uint16 m_clientport;
	std::string m_name;
    sf::IPAddress m_bindaddress;
    sf::SocketUDP Socket;
    sf::Packet SendPacket;
    sf::Packet RecvPacket;

    sf::Uint16 m_cl_id;
	sf::String mousepos;

    // Blocks
	float m_ratio;
	blockarray2d::extent_gen extents;
	blockarray2d m_blocks;
    blockarray1d m_mpos;

	sf::Uint16 m_blocknbx;
	sf::Uint16 m_blocknby;

	int mode;

};

#endif

