#ifndef GAMESTATE_HPP_INCLUDED
#define GAMESTATE_HPP_INCLUDED

#include <SFML/Window.hpp>
//#include "gameengine.hpp"

class GameEngine;

class IGameState{
    public:
        virtual void Init(GameEngine* game) = 0;
        virtual void Cleanup() = 0;

        virtual void Pause() = 0;
        virtual void Resume() = 0;

        virtual void HandleEvents() = 0;
        virtual void Update() = 0;
        virtual void Draw() = 0;

    private:
        const sf::Input* mInput;

    protected:
        GameEngine* mGameEngine;
        IGameState(){}
};

#endif
