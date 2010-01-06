//cpGUI - Copyright (c) 2009 Jason Cupp
//
//This software is provided 'as-is', without any express or implied warranty. 
//In no event will the authors be held liable for any damages arising from the 
//use of this software.
//
//Permission is granted to anyone to use this software for any purpose, including 
//commercial applications, and to alter it and redistribute it freely, subject to 
//the following restrictions:
//
//
//1. The origin of this software must not be misrepresented; you must not claim 
//that you wrote the original software. If you use this software in a product, an 
//acknowledgment in the product documentation would be appreciated but is not required.
//
//
//2. Altered source versions must be plainly marked as such, and must not be 
//misrepresented as being the original software.
//
//
//3. This notice may not be removed or altered from any source distribution.




#include <SFML/Graphics.hpp>
#include "cpGUI_base.h"
#include "cpTextBox.h"
#include "cpSelection-DropDownBox.h"
#include "cpImageButton.h"
#include "cpShapeButton.h"
#include "cpCheckBox.h"
#include "cpTextInputBox.h"


int main()
{
	sf::RenderWindow App(sf::VideoMode(800,600,32), "cpGUI");
	App.SetPosition(10,10);

	// Create a GuiContainer to contain all of the controls we create.
	// This is used mainly for setting focus.
	// YOU HAVE TO HAVE ONE OF THESE or nothing will work.

	cp::cpGuiContainer myGUI;


	// *** Create a non-editable read only Text Box & display a txt file ***
	// Parameters: 1. the parent render window; 2. your GuiContainer;
	// 3&4. the x & y coordinates; 5&6. the width & height

	cp::cpTextBox textBox(&App, &myGUI, 20,20,300,300);

	// When setting the font size, the current font is reloaded with the new
	// size for better quality.
	textBox.SetFontSize(14);
	textBox.LoadFile("license.txt");

	
	// *** Create a Selection Box and populate it with choices ***

	cp::cpSelectionBox selBox(&App, &myGUI, 450, 70, 200, 150);

	std::string choices[] = {"Dwarf", "Elf", "Warrior", "Wizard",
		"Adventurer", "Guild Leader", "Cleric", "eight", "nine",
		"ten", "eleven", "twelve", "thirteen", "fourteen"};
	for(int t=0; t < 14; t++)
		selBox.AddChoice(choices[t]);

	selBox.SetFontSize(20);


	// *** Create a Drop Down Box and populate it with choices ***

	cp::cpDropDownBox dropdownBox(&App, &myGUI, "Select your weapon", 450, 250, 200, 20);

	// Set the maximum selection box depth before a scrollbar is used.
	// 100 is default
	dropdownBox.SetMaxDepth(200);

	std::string choices2[] = {"Broad sword", "Axe", "Crossbow", "Long bow", "Mace"};
	for(int t=0; t < 5; t++)
		dropdownBox.AddChoice(choices2[t]);

	
	// *** Create an Image button using an sf::Image ***
	// These buttons do not have a label, and the size is based on the image's size

	sf::Image image;
	if(!image.LoadFromFile("dragon.png"))
		return EXIT_FAILURE;

	cp::cpImageButton imageBtn(&App, &myGUI, &image, 50, 400);


	// *** Create a regular Button with a label ***
	// if you don't set the button big enough for the text,
	// the button will automatically be resized.
	// default fontsize is 20, you can resize the font and the button
	// later.

	cp::cpButton btn(&App, &myGUI, "Exit Game", 200, 440, 70, 30);


	// *** Create a Shape Button from an sf::Shape ***

	sf::Shape shape;
	shape.AddPoint(200,200, sf::Color::Red);
	shape.AddPoint(250,300, sf::Color::Yellow);
	shape.AddPoint(150,300, sf::Color::Blue);

	cp::cpShapeButton shapeBtn(&App, &myGUI, &shape, 350, 420);


	// *** Create a simple on/off Check Box ***
	// The size of the box is based on the size of the Label's font

	cp::cpCheckBox checkBox(&App, &myGUI, "Play background music", 500, 480);
	checkBox.SetFontSize(20);
	checkBox.SetChecked(true); // unchecked by default
	checkBox.SetMouseoverColor(sf::Color(200,100,100));


	// *** Create a Text Input Box which is a one line input box ***
	// We will also create an sf::String do display above it for a description

	sf::String textInputString("Enter your name:", sf::Font::GetDefaultFont(), 20);
	textInputString.SetPosition(500, 400);
	textInputString.SetColor(sf::Color::Black);

	cp::cpTextInputBox textInputBox(&App, &myGUI, "", 500, 430, 230, 25);





	const sf::Input& input = App.GetInput();

	while(App.IsOpened())
	{

		// The standard Event loop

		sf::Event Event;
		while(App.GetEvent(Event))
		{
			if(Event.Type == sf::Event::Closed)
				App.Close();
			if((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
				App.Close();

			// if you are using a Text Input box, this is where you
			// put the function that processes your text input
			textInputBox.ProcessTextInput(&Event);
			
			// This is the function that takes care of which control
			// has focus out of all the controls registered to your
			// GuiContainer.
			myGUI.ProcessKeys(&Event);
		}

		// After the events are processed, we need to check the state of every
		// control we create using your Input as a parameter.
		// Most controls will return an integer that will tell you the
		// mouse activity within that control.  For these controls, the
		// possible return values are included in an enumeration:
		// CP_STATE_MOUSE_ENTER, CP_STATE_MOUSE_EXIT, CP_STATE_MOUSE_IN,
		// CP_STATE_MOUSE_LBUTTON_DOWN, CP_STATE_MOUSE_LBUTTON_RELEASED,
		// CP_STATE_CHANGED, & CP_STATE_NONE
		// you can use these values to decide what to do.

		textBox.CheckState(&input);

		int selection; // an int to store the selection choice from
					   // our selection box & drop down box

		if(selBox.CheckState(&input) == cp::CP_ST_MOUSE_LBUTTON_RELEASED)
		{
			selection = selBox.GetSelection();
			if(selection == 10)
				selBox.RemoveLastChoice();
		}

		if(dropdownBox.CheckState(&input) == cp::CP_ST_MOUSE_LBUTTON_RELEASED)
			selection = dropdownBox.GetSelection();
	
		if(imageBtn.CheckState(&input) == cp::CP_ST_MOUSE_LBUTTON_RELEASED)
			App.Close();

		if(btn.CheckState(&input) == cp::CP_ST_MOUSE_LBUTTON_RELEASED)
			App.Close();

		// Put the results of CheckState for the shape button in an int
		// so we only have to call the function once.
		// If the mouse enters the shape button, the normal button is
		// hidden.  When the mouse exits the shape button, the normal
		// button is shown.  When the shape button is clicked, the App closes
		int state = shapeBtn.CheckState(&input);
		if(state == cp::CP_ST_MOUSE_ENTER)
			btn.Show(false);
		if(state == cp::CP_ST_MOUSE_EXIT)
			btn.Show(true);
		if(state == cp::CP_ST_MOUSE_LBUTTON_RELEASED)
			App.Close();

		// When the Check Box is clicked, store the check state in 'playmusic'
		bool playmusic;
		if(checkBox.CheckState(&input) == cp::CP_ST_MOUSE_LBUTTON_RELEASED)
			playmusic = checkBox.GetChecked();

		std::string name;
		if(textInputBox.CheckState(&input) == cp::CP_ST_MOUSE_LBUTTON_RELEASED)
			name = textInputBox.GetLabelText();



		App.Clear(sf::Color(180,180,180));


		// Every object you create should have the Draw() function
		// called on every cycle.  If you don't want an object to
		// be visible, set Show(false) for that object.
		textBox.Draw();
		selBox.Draw();
		imageBtn.Draw();
		btn.Draw();
		shapeBtn.Draw();
		checkBox.Draw();
		App.Draw(textInputString);
		textInputBox.Draw();
		dropdownBox.Draw(); // drop down boxes should always be drawn last

		App.Display();
	}
	return EXIT_SUCCESS;
}

