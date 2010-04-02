#ifndef MENU_HPP_INCLUDED
#define MENU_HPP_INCLUDED

// main class for menu application.
class MenuApp {
public:
    MenuApp();
    ~MenuApp();

    // initializes menu
    void Init();

    // grabs user input and prepares it for sending to the server
    void HandleInput();

	// input grabbed by HandleInput() is sent to the server while getting new
	// info about the game state from it
    void Update();

    // draws all new data to the screens
    void Draw();

   	// safely shuts down the menu
    void Die();

private:
};

#endif
