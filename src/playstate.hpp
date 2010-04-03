#ifndef PlayState_H
#define PlayState_H

#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/multi_array.hpp>

#include <ImageMagick/Magick++.h>

#include <string>

#include "network.hpp"
#include "filesystem.hpp"
#include "resources.hpp"
#include "block.hpp"

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

	// places a block to the specified area
    void PlaceBlock(int x, int y);

    // Check if mouse is in legal area for building.
    bool MouseInPlayableArea();

    // check if the coordinates are in a legal area
    bool InPlayableArea(int, int);
    // Get the block beneath mouse cursor.
    sf::Uint16 GetMouseBlock(char, char);

    // Get the client socket.
    Network::Socket& GetSocket();

protected:
	PlayState() { }

private:
	static PlayState m_PlayState;

    ResourceManager ResMgr;

	uint16_t m_port;
	uint16_t m_clientport;
	std::string m_name;
    Network::IPAddress m_bindaddress;
    Network::Socket Socket;
    Network::Packet SendPacket;
    Network::Packet RecvPacket;

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
};

#endif

