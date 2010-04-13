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

    // Check if mouse is in legal area for building.
    bool MouseInPlayableArea();

    // check if the coordinates are in a legal area
    bool InPlayableArea(int, int);
    // Get the block beneath mouse cursor.
    sf::Uint16 GetMouseBlock(char, char);


protected:
	PlayState() { }

private:
	static PlayState m_PlayState;

    ResourceManager ResMgr;

	sf::Uint16 m_port;
	sf::Uint16 m_clientport;
	std::string m_name;
    sf::IPAddress m_bindaddress;
    sf::SocketUDP Socket;
    sf::Packet SendPacket;
    sf::Packet RecvPacket;

    sf::Uint16 m_cl_id;
	sf::String mousepos;

	float m_ratio;
	blockarray2d::extent_gen extents;
	blockarray2d m_blocks;
     blockarray1d m_mpos;

	int m_blocknbx;
	int m_blocknby;

	int mode; // 0 - normal mode ; 1 - build mode ; 2 - demolition mode

	sf::String fps;
	sf::Shape inforect;

	sf::Shape mSkyRect;

	boost::ptr_vector<sf::Sprite> mClouds;
};

#endif

