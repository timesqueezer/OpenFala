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




#include "cpGUI.hpp"
using namespace cp;


/// *********************************************************
/// Class that contains all objects/controls in the GUI.
/// Used mostly for setting focus and keyboard controls.
/// *********************************************************
cpGuiContainer::cpGuiContainer()
{
	clock.Reset();
	control.reserve(10);
	acceptsKeys = true;
	useArrowKeys = true;
	bShow = true;
	rightleft = 1;

	// Find the Windows directory, and load a font
	// segoeui for Vista
	// tahoma for XP
	// arial if the others aren't present
	//
	// If you're not in Windows, you're on your own and you'll
	// have to settle for the default font, or use the
	// cpObject::SetFont() function.
	defaultFont = new sf::Font(sf::Font::GetDefaultFont());
	defaultFilename = "Default";
#ifdef WIN32
	char tempdir[MAX_PATH];
	GetWindowsDirectoryA(tempdir, sizeof(tempdir)/sizeof(char));
	std::string dir;
	dir.assign(tempdir);

	defaultFilename = dir + "/Fonts/segoeui.ttf";
	if(!defaultFont->LoadFromFile(defaultFilename, 20))
	{
		defaultFilename = dir + "/Fonts/tahoma.ttf";
		if(!defaultFont->LoadFromFile(defaultFilename, 20))
		{
			defaultFilename = dir + "/Fonts/arial.ttf";
			defaultFont->LoadFromFile(defaultFilename, 20);
		}
	}
#endif
	std::string key = defaultFilename;
	if(key != "Default")
		key = key + ";20";
	FontData data = {defaultFont, 1};
	fontMap.insert(std::make_pair(key , data));
}

cpGuiContainer::~cpGuiContainer()
{
	while(fontMap.begin() != fontMap.end())
	{
		delete fontMap.begin()->second.myFont;
		fontMap.erase(fontMap.begin());
	}
}

void cpGuiContainer::ReleaseUnusedFonts()
{
	for(FontMap::iterator iter = fontMap.begin(); iter != fontMap.end(); iter++)
	{
		iter->second.timesUsed = 0;
	}

	for(unsigned int t=0; t < control.size(); t++)
	{
		std::string key = "Default";
		if(control[t]->Font != "Default")
		{
			std::stringstream size;
			size << control[t]->textFont->GetCharacterSize();
			key = control[t]->Font + ";" + size.str();
		}
		FontMap::iterator it = fontMap.find(key);
		it->second.timesUsed++;
	}
	FontMap::iterator it1 = fontMap.begin();
	FontMap::iterator it2 = fontMap.end();
	FontMap::iterator itTemp;

	while(it1 != it2)
	{
		if(it1->second.timesUsed == 0)
		{
			if(it1->first != defaultFilename + ";20" && it1->first != "Default")
			{
				sf::Font *oldFont = it1->second.myFont;
				delete oldFont;
				fontMap.erase(it1++);
			}
			else
				++it1;
		}
		else
			++it1;
	}
}

sf::Font* cpGuiContainer::GetFont(std::string filename, unsigned int fontSize)
{
	std::string key = filename;
	if(filename != "Default")
	{
		std::stringstream size;
		size << fontSize;
		key = filename + ";" + size.str();
	}
	FontMap::iterator it = fontMap.find(key);
	if(it == fontMap.end())
	{
		sf::Font *font = new sf::Font;
		if(font->LoadFromFile(filename, fontSize))
		{
			FontData data = {font, 1};
			it = fontMap.insert(std::make_pair(key, data)).first;
		}
		else
			throw std::runtime_error("Font " + filename + " could not be loaded");
	}
	sf::Font *result = it->second.myFont;

	return result;
}

/// All controls that can accept focus or use the
/// mouse wheel should be registered to the GuiContainer.
void cpGuiContainer::Register(cpObject *object)
{
	control.push_back(object);
}

/// This is used to "unregister" a control that has been
/// registered. Used in classes that are a compilation
/// of different objects when each separate object should
/// not receive focus.
void cpGuiContainer::Unregister(cpObject *object)
{
	for(unsigned int t=0; t < control.size(); t++)
	{
		if(control[t] == object)
			control.erase(control.begin()+t);
	}
}

/// Sets or removes focus from an object that is registered
/// to the GuiContainer.
void cpGuiContainer::SetObjectFocus(cpObject *object, bool focused)
{
	if(control.size() == 0)
		return;

	for(unsigned int t = 0; t < control.size(); t++)
	{
		if(control[t] == object)
			control[t]->hasFocus = focused;
		else
			control[t]->hasFocus = false;
	}
}

/// Cause the next object to receive focus.
/// This is not based on screen position.
/// It is based on the order of registration.
void cpGuiContainer::NextObjectFocus()
{
	if(control.size() == 0)
		return;

	bool show = false;
	for(unsigned int t=0; t < control.size(); t++)
	{
		if(control[t]->GetShow())
			show = true;
	}
	if(!show)
		return;

	int focus = -1;
	int size = control.size();
	for(int t = 0; t < size; t++)
	{
		if(control[t]->hasFocus)
			focus = t;
	}
	if(focus > -1)
		control[focus]->hasFocus = false;
	do
	{
		focus = focus + (1 * rightleft);
		if(focus >= size)
			focus = 0;
		if(focus < 0)
			focus = size - 1;
	}while(!control[focus]->GetShow());

	control[focus]->hasFocus = true;
}

/// Sets whether you can use Tab to move focus to the next object.
/// Also sets whether the arrow keys can move the focus.
/// Classes that use arrow keys locally should set
/// UseArrowKeys to false.
void cpGuiContainer::AcceptsKeys(bool accept, bool UseArrowKeys)
{
	acceptsKeys = accept;
	useArrowKeys = UseArrowKeys;
}

/// Used in the event loop to process the focus changing keys.
/// This function also sends the mouse wheel delta to the
/// classes that need that information.
void cpGuiContainer::ProcessKeys(sf::Event *evt)
{
	if(control.size() == 0)
		return;

	int focus = -1;

	for(unsigned int t=0; t < control.size(); t++)
	{
		control[t]->wheelDelta = 0;
		if(control[t]->hasFocus)
			focus = t;
	}
	if(focus > -1)
	{
		if(control[focus]->useWheel)
			control[focus]->wheelDelta = evt->MouseWheel.Delta;
	}


	if(!acceptsKeys)
		return;

	if((evt->Type == sf::Event::KeyPressed) && (evt->Key.Code == sf::Key::Tab))
	{
		rightleft = 1;
		NextObjectFocus();
	}

	if(!useArrowKeys)
		return;

	if(evt->Type == sf::Event::KeyPressed && ((evt->Key.Code == sf::Key::Left)
		|| (evt->Key.Code == sf::Key::Up)))
	{
		rightleft = -1;
		NextObjectFocus();
	}

	if(evt->Type == sf::Event::KeyPressed && ((evt->Key.Code == sf::Key::Right)
		|| (evt->Key.Code == sf::Key::Down)))
	{
		rightleft = 1;
		NextObjectFocus();
	}



}

/// Show or hide every control that is registered
/// to the GuiContainer.
void cpGuiContainer::Show(bool show)
{
	if(control.size() == 0)
		return;

	for(unsigned int t=0; t < control.size(); t++)
		control[t]->Show(show);
	bShow = show;
}

/// Returns whether or not all contained objects are shown.
bool cpGuiContainer::GetShow()
{
	return bShow;
}



/// *********************************************************************
/// Create constructor and functions for cpObject.
/// cpObject is the base class that all other controls inherit from.
/// *********************************************************************
cpObject::cpObject(sf::RenderWindow* parent, cpGuiContainer *GUI, std::string label,
				   float posx, float posy, float width, float height)
{
	GUI->Register(this);

	Width = width;
	Height = height;
	PosX = posx; // control's x & y position
	PosY = posy;
	Parent = parent; // the render window that the control is displayed on
	bDefaultSize = false; // did user set the size or leave it default?
	mouseIn = false;  // is the mouse pointer inside the control?
	lbuttonDown = false; // is the left mouse button down?
	returnDown = false;  // is the return key down?
	hasFocus = false;  // Does this control have the focus?
	bShow = true;  // Should the control be drawn?
	useWheel = false;
	keyCode = 0;
	wheelDelta = 0;
	gui = GUI;
	Font = gui->defaultFilename;


	if(width == CP_DEFAULT || height == CP_DEFAULT)
		bDefaultSize = true;

	backgroundColor = sf::Color(240,240,240);
	backgroundColorTemp = backgroundColor;
	outlineColor = sf::Color(0,0,0);
	outlineColorTemp = outlineColor;
	mouseoverColor = sf::Color(167,217,245);

	SetFont(Font,20);
	if(Font == "Default")
		Label.SetSize(20);

	// Set the generic attributes of the control's Label
	textColor = sf::Color::Black;
	Label.SetText(label);
	Label.SetColor(textColor);

	SetPosition(PosX, PosY);  // if PosX or PosY is invalid, this deals with it

	// create the generic background of a control
	backRect.AddPoint(PosX+2, PosY, backgroundColor);
	backRect.AddPoint(PosX+Width-2, PosY, backgroundColor);
	backRect.AddPoint(PosX+Width, PosY+2, backgroundColor);
	backRect.AddPoint(PosX+Width, PosY+Height-2, backgroundColor);
	backRect.AddPoint(PosX+Width-2, PosY+Height, backgroundColor);
	backRect.AddPoint(PosX+2, PosY+Height, backgroundColor);
	backRect.AddPoint(PosX, PosY+Height-2, backgroundColor);
	backRect.AddPoint(PosX, PosY+2, backgroundColor);
	backRect.SetOutlineWidth(1.f);
	backRect.EnableOutline(true);

}


/// If the given position is invalid, this deals with it
/// This function also updates the control's position when the user moves it
/// by calling this function
void cpObject::SetPosition(float posx, float posy)
{
	PosX = posx;
	PosY = posy;
	if(PosX < 0)
		PosX = 0;
	if(PosY < 0)
		PosY = 0;
	if(PosX > Parent->GetWidth() - Label.GetRect().GetWidth())
		PosX = Parent->GetWidth() - Label.GetRect().GetWidth();
	if(PosY > Parent->GetHeight() - Label.GetRect().GetHeight())
		PosY = Parent->GetHeight() - Label.GetRect().GetHeight();
}

/// Change the size of the control.
/// If the given size is smaller than the size of the control's
/// Label, it returns false and nothing is changed.
bool cpObject::SetSize(float width, float height)
{
	if(Label.GetRect().GetWidth() > Width ||
		Label.GetRect().GetHeight() > Height)
		return false;
	Width = width;
	Height = height;
	bDefaultSize = false;
	return true;
}

/// Sets the font of the control's Label.
/// Send a filename of "Default" to use SFML's
/// default font.
void cpObject::SetFont(std::string filename, unsigned int size)
{
	if(filename == "Default")
		filename = gui->defaultFilename;
	textFont = gui->GetFont(filename, size);
	Label.SetFont(*textFont);
	Label.SetSize((float)textFont->GetCharacterSize());
	if(filename == "Default")
		Label.SetSize((float)size);
	Font = filename;
	gui->ReleaseUnusedFonts();
}

/// Sets the controls generic background color
void cpObject::SetBackgroundColor(sf::Color color)
{
	backgroundColor = color;
	backgroundColorTemp = color;
}

/// Sets the font style for the control's Label.
/// sf::String::Bold | sf::String::Italic | sf::String::Underlined
/// or sf::String::Regular
void cpObject::SetFontStyle(unsigned long TextStyle)
{
	Label.SetStyle(TextStyle);
}

/// Change the font size. The font is actually reloaded
/// with the new size for better quality.
void cpObject::SetFontSize(unsigned int size)
{
	tempLabel = Label;
	SetFont(Font, size);
}

/// Set's color of control's text
void cpObject::SetLabelColor(sf::Color color)
{
	textColor = color;
	Label.SetColor(textColor);
}

/// Sets the text of the control's Label
void cpObject::SetLabelText(std::string text)
{
	Label.SetText(text);
}

/// This function checks for mouse events within the control.
/// it updates the look of the control according to the event.
/// it also returns an integer enumeration member to inform the user.
/// possible return values are:
/// CP_ST_NONE, CP_ST_MOUSE_ENTER, CP_ST_MOUSE_EXIT,
/// CP_ST_MOUSE_IN, CP_ST_LBUTTON_DOWN, CP_ST_LBUTTON_RELEASED.
///
/// The difference between CP_ST_MOUSE_ENTER & CP_ST_MOUSE_IN
/// is that IN is active the entire time the mouse is inside the control
/// and ENTER is only active right when the mouse enters the control
int cpObject::CheckState(const sf::Input *input)
{
	int state = CP_ST_NONE;

	if(!bShow)
		return state;

	outlineColor = sf::Color::Black;
	outlineColorTemp = outlineColor;

	if(CheckMouseEntered(input))
	{
		if(mouseIn)
		{
			outlineColor = mouseoverColor;
			outlineColorTemp = outlineColor;
			state = CP_ST_MOUSE_IN;
			mouseIn = true;
		}

		if(!mouseIn)
		{
			state = CP_ST_MOUSE_ENTER;
			mouseIn = true;
		}

		if(input->IsMouseButtonDown(sf::Mouse::Left))
		{
			outlineColor = sf::Color(44, 98, 139);
			outlineColorTemp = outlineColor;
			state = CP_ST_MOUSE_LBUTTON_DOWN;
			lbuttonDown = true;
			gui->SetObjectFocus(this, true);
		}

		if(!input->IsMouseButtonDown(sf::Mouse::Left) &&
			lbuttonDown)
		{
			state = CP_ST_MOUSE_LBUTTON_RELEASED;
			lbuttonDown = false;
		}
	}

	else
	{
		if(mouseIn)
		{
			outlineColor = sf::Color::Black;
			outlineColorTemp = outlineColor;
			state = CP_ST_MOUSE_EXIT;
			mouseIn = false;
		}
	}

	if(hasFocus && gui->acceptsKeys && input->IsKeyDown(sf::Key::Return))
	{
		outlineColor = sf::Color(44, 98, 139);
		outlineColorTemp = outlineColor;
		state = CP_ST_MOUSE_LBUTTON_DOWN;
		returnDown = true;
		gui->SetObjectFocus(this, true);
	}

	if(hasFocus && !input->IsKeyDown(sf::Key::Return) &&
		gui->acceptsKeys && returnDown)
	{
		state = CP_ST_MOUSE_LBUTTON_RELEASED;
		returnDown = false;
	}

	return state;
}

/// Checks to see if mouse pointer is inside control.  Returns true if it is.
bool cpObject::CheckMouseEntered(const sf::Input *input)
{
	sf::Vector2f mousePos = Parent->ConvertCoords(input->GetMouseX(), input->GetMouseY());
	return (mousePos.x >= PosX && mousePos.x <= PosX+Width &&
        mousePos.y >= PosY && mousePos.y <= PosY+Height);
}

/// sets the Label's text and recreates the generic background
/// area with the updated attributes.  This should happen after
/// any change that is graphical.
void cpObject::CreateRects(std::string label)
{
	Label.SetText(label);
	Label.SetColor(textColor);

	backRect.SetPointPosition(0,PosX+2, PosY);
	backRect.SetPointPosition(1,PosX+Width-2, PosY);
	backRect.SetPointPosition(2,PosX+Width, PosY+2);
	backRect.SetPointPosition(3,PosX+Width, PosY+Height-2);
	backRect.SetPointPosition(4,PosX+Width-2, PosY+Height);
	backRect.SetPointPosition(5,PosX+2, PosY+Height);
	backRect.SetPointPosition(6,PosX, PosY+Height-2);
	backRect.SetPointPosition(7,PosX, PosY+2);
	for(int t=0; t<8; t++)
	{
		backRect.SetPointColor(t, backgroundColor);
		backRect.SetPointOutlineColor(t, outlineColor);
	}
}

/// Returns a Vector2f with the object's position coordinates
sf::Vector2f cpObject::GetPosition()
{
	return sf::Vector2f(PosX, PosY);
}

/// Returns a Vector2f with the object's width & height
sf::Vector2f cpObject::GetSize()
{
	return sf::Vector2f(Width, Height);
}

/// Returns the object's background color
sf::Color cpObject::GetBackgroundColor()
{
	return backgroundColor;
}

/// Returns an unsigned long of the style
unsigned long cpObject::GetFontStyle()
{
	return Label.GetStyle();
}

/// Returns the label's text color
sf::Color cpObject::GetLabelColor()
{
	return textColor;
}

/// Returns the Label's text
std::string cpObject::GetLabelText()
{
	return Label.GetText();
}

/// This function draws a dashed line that will make up
/// one side of a dashed rectangle for a focused object
void cpObject::DrawDashedLine(sf::Vector2f start, sf::Vector2f end)
{
	bool on = true;
	sf::Vector2f current = end-start;
	float length = sqrtf(current.x * current.x + current.y * current.y);
	float pos = 0.f;

	current /= length;

	while((pos+2)<=length)
	{
		if(on)
		{
			sf::Vector2f begin = start + pos * current;
			sf::Vector2f end = start + (pos + 2) * current;
			sf::Shape line = sf::Shape::Line(begin.x, begin.y, end.x, end.y, 1.0f,
				outlineColor, 0.f, sf::Color::Black);
			Parent->Draw(line);
		}
		on = !on;
		pos += 2;
	}
}

/// This function draws the four dashed-line sides of a rectangle
/// to show that certain objects have focus
void cpObject::DrawDashedRect(float posx, float posy, float width, float height)
{
	DrawDashedLine(sf::Vector2f(posx, posy), sf::Vector2f(posx+width, posy));
	DrawDashedLine(sf::Vector2f(posx+width, posy), sf::Vector2f(posx+width, posy+height));
	DrawDashedLine(sf::Vector2f(posx+width, posy+height), sf::Vector2f(posx, posy+height));
	DrawDashedLine(sf::Vector2f(posx, posy+height), sf::Vector2f(posx, posy));
}

/// Causes an object to be shown or hidden
void cpObject::Show(bool show)
{
	bShow = show;
}

/// Returns whether or not the object is shown.
bool cpObject::GetShow()
{
	return bShow;
}

/// Forces an object to receive focus
void cpObject::SetFocus(bool focus)
{
	gui->SetObjectFocus(this, focus);
}

/// Allows an object to receive mouse wheel delta info
/// from the GuiContainer
void cpObject::SetUseWheel(bool use)
{
	useWheel = use;
}

void cpObject::SetMouseoverColor(sf::Color color)
{
	mouseoverColor = color;
}

sf::Color cpObject::GetMouseoverColor()
{
	return mouseoverColor;
}


/// *********************************************************************************
/// create cpButton constructor which inherits from
/// cpObject.  Also create all functions for cpButton.
/// Some functions are unique, but most augment functions from cpObject
/// *********************************************************************************
cpButton::cpButton(sf::RenderWindow* parent, cpGuiContainer *GUI, std::string label,
				   float posx, float posy, float width, float height) : cpObject(parent,
				   GUI, label, posx, posy, width, height)
{
	labelInside = true;

	backgroundColor2 = sf::Color(170,170,170);
	backgroundColor2Temp = backgroundColor2;

	TestLabelFit();

	gradient.AddPoint(PosX+2, PosY+Height/2, backgroundColor);
	gradient.AddPoint(PosX+Width-2, PosY+Height/2, backgroundColor);
	gradient.AddPoint(PosX+Width-2, PosY+Height-3, backgroundColor2);
	gradient.AddPoint(PosX+Width-3, PosY+Height-2, backgroundColor2);
	gradient.AddPoint(PosX+3, PosY+Height-2, backgroundColor2);
	gradient.AddPoint(PosX+2, PosY+Height-3, backgroundColor2);
}

cpButton::cpButton() : cpObject(NULL, NULL, ""){}

/// If the button is shown, everything in the button is drawn.
/// If the button has focus, a dashed rectangle is drawn.
void cpButton::Draw()
{
	if(!bShow)
		return;

	Parent->Draw(backRect);
	Parent->Draw(gradient);
	Parent->Draw(Label);

	if(hasFocus && labelInside)
		DrawDashedRect(PosX+2, PosY+2, Width-4, Height-4);

}

/// Sets the button's position in the window
void cpButton::SetPosition(float posx, float posy)
{
	cpObject::SetPosition(posx, posy);
	CreateRects(Label.GetText());
}

/// Sets the width & height dimensions of the button
bool cpButton::SetSize(float width, float height)
{
	if(!cpObject::SetSize(width, height))
		return false;
	CreateRects(Label.GetText());
	return true;
}

/// Loads a font to use on the Label
void cpButton::SetFont(std::string filename, unsigned int size)
{
	cpObject::SetFont(filename, size);
	CreateRects(Label.GetText());
}

/// Sets the button's background color
void cpButton::SetBackgroundColor(sf::Color color)
{
	cpObject::SetBackgroundColor(color);
	int r=0;
	int g=0;
	int b=0;
	if(backgroundColor.r > 70)
		r = backgroundColor.r - 70;
	if(backgroundColor.g > 70)
		g = backgroundColor.g - 70;
	if(backgroundColor.b > 70)
		b = backgroundColor.b - 70;
	backgroundColor2 = sf::Color(r,g,b);
	backgroundColor2Temp = backgroundColor2;
	CreateRects(Label.GetText());
}

/// Sets the Label's font style: Bold, Italic, Underlined, Regular
void cpButton::SetFontStyle(unsigned long TextStyle)
{
	cpObject::SetFontStyle(TextStyle);
	CreateRects(Label.GetText());
}

/// Sets the Label's font size.  The font is reloaded with
/// the new size for optimal quality
void cpButton::SetFontSize(unsigned int size)
{
	cpObject::SetFontSize(size);
	TestLabelFit();
	CreateRects(Label.GetText());
}

/// Sets the text color
void cpButton::SetLabelColor(sf::Color color)
{
	cpObject::SetLabelColor(color);
	CreateRects(Label.GetText());
}

/// Sets the text shown in the button
void cpButton::SetLabelText(std::string text)
{
	cpObject::SetLabelText(text);
	CreateRects(Label.GetText());
}

/// This function checks for mouse events within the control.
/// it updates the look of the control according to the event.
/// it also returns an integer enumeration member to inform the user.
/// possible return values are:
/// CP_ST_NONE, CP_ST_MOUSE_ENTER, CP_ST_MOUSE_EXIT,
/// CP_ST_MOUSE_IN, CP_ST_LBUTTON_DOWN, CP_ST_LBUTTON_RELEASED.
///
/// The difference between CP_ST_MOUSE_ENTER & CP_ST_MOUSE_IN
/// is that IN is active the entire time the mouse is inside the control
/// and ENTER is only active right when the mouse enters the control
int cpButton::CheckState(const sf::Input *input)
{
	int state = cpObject::CheckState(input);

	backgroundColor2 = backgroundColor2Temp;

	if(state == CP_ST_MOUSE_IN)
	{
		backgroundColor2 = mouseoverColor;
	}
	if(state == CP_ST_MOUSE_LBUTTON_DOWN)
	{
		backgroundColor2 = sf::Color(44, 98, 139);
	}

	CreateRects(Label.GetText());
	return state;
}

/// Updates the look of all the different parts of the button
void cpButton::CreateRects(std::string label)
{
	cpObject::CreateRects(label);
	if(Label.GetRect().GetWidth() > Width || bDefaultSize)
	{
		Width = Label.GetRect().GetWidth() + 10;
	}
	if(Label.GetRect().GetHeight() > Height || bDefaultSize)
	{
		Height = Label.GetRect().GetHeight() + 10;
	}
	labelX = PosX+(Width/2.f) - Label.GetRect().GetWidth()/2.f;
	labelY = PosY+(Height/2.f) - Label.GetRect().GetHeight()/1.7f;
	Label.SetPosition(labelX, labelY);

	gradient.SetPointPosition(0,PosX+2, PosY+Height/2);
	gradient.SetPointPosition(1,PosX+Width-2, PosY+Height/2);
	gradient.SetPointPosition(2,PosX+Width-2, PosY+Height-3);
	gradient.SetPointPosition(3,PosX+Width-3, PosY+Height-2);
	gradient.SetPointPosition(4,PosX+3, PosY+Height-2);
	gradient.SetPointPosition(5,PosX+2, PosY+Height-3);
	gradient.SetPointColor(0, backgroundColor);
	gradient.SetPointColor(1, backgroundColor);
	for(int t=2; t<6; t++)
		gradient.SetPointColor(t, backgroundColor2);
}

/// Sets whether the button is shown or hidden
void cpButton::Show(bool show)
{
	cpObject::Show(show);
	CreateRects(Label.GetText());
}

/// Tests to see if the Label's text can fit inside the button
void cpButton::TestLabelFit()
{
	if(!labelInside)
		return;
	if(Label.GetRect().GetWidth() > Width || bDefaultSize)
	{
		Width = Label.GetRect().GetWidth() + 10;
		bDefaultSize = true;
	}
	if(Label.GetRect().GetHeight() > Height || bDefaultSize)
	{
		Height = Label.GetRect().GetHeight() + 10;
	}
	labelX = PosX+(Width/2) - Label.GetRect().GetWidth()/2;
	labelY = PosY+(Height/2.f) - Label.GetRect().GetHeight()/1.7f;
	Label.SetPosition(labelX, labelY);
}


/// ******************************************************************
/// A class of buttons that contain an image.
/// These buttons cannot be resized; the size is based on the
/// size of the image sent to it.  Also, there is no text label.
/// ******************************************************************
cpImageButton::cpImageButton(sf::RenderWindow *parent, cpGuiContainer *GUI,
							 sf::Image *image, float posx, float posy) : cpObject(parent,
							 GUI, "", posx, posy, (float)image->GetWidth()+3,
								(float)image->GetHeight()+3)
{
	sprite.SetImage(*image);
	sprite.SetPosition(posx + 1, posy + 1);
}

cpImageButton::cpImageButton() : cpObject(NULL, NULL, ""){}

/// Clears the sprite and loads the new image into it.
/// the sprite is used to display the image in the button.
void cpImageButton::SetImage(sf::Image *image)
{
	Width = (float)image->GetWidth()+3;
	Height = (float)image->GetHeight()+3;
	sprite.SetImage(sf::Image());
	sprite.SetImage(*image);
	CreateRects("");
}

/// Draws the different parts that make up an image button
void cpImageButton::Draw()
{
	if(!bShow)
		return;

	Parent->Draw(backRect);
	Parent->Draw(sprite);

	if(hasFocus)
		DrawDashedRect(PosX+2, PosY+2, Width-4, Height-4);
}

/// This function checks for mouse events within the control.
/// it updates the look of the control according to the event.
/// it also returns an integer enumeration member to inform the user.
/// possible return values are:
/// CP_ST_NONE, CP_ST_MOUSE_ENTER, CP_ST_MOUSE_EXIT,
/// CP_ST_MOUSE_IN, CP_ST_LBUTTON_DOWN, CP_ST_LBUTTON_RELEASED.
///
/// The difference between CP_ST_MOUSE_ENTER & CP_ST_MOUSE_IN
/// is that IN is active the entire time the mouse is inside the control
/// and ENTER is only active right when the mouse enters the control
int cpImageButton::CheckState(const sf::Input *input)
{
	int state = cpObject::CheckState(input);

	if(state == CP_ST_MOUSE_LBUTTON_DOWN)
		backgroundColor = sf::Color(140, 140, 140);

	if(state == CP_ST_MOUSE_LBUTTON_RELEASED)
		backgroundColor = backgroundColorTemp;

	CreateRects("");
	return state;
}

/// Sets the image button's width & height dimensions
bool cpImageButton::SetSize(float width, float height)
{
	return false;
}

/// Sets the image button's position coordinates in the window
void cpImageButton::SetPosition(float posx, float posy)
{
	cpObject::SetPosition(posx, posy);
	CreateRects("");
}

/// Sets the image button's background color
void cpImageButton::SetBackgroundColor(sf::Color color)
{
	cpObject::SetBackgroundColor(color);
	CreateRects("");
}

/// Sets whether the image button is shown or hidden
void cpImageButton::Show(bool show)
{
	cpObject::Show(show);
	CreateRects("");
}


/// **********************************************************
/// A simple on/off check box with a label
/// **********************************************************
cpCheckBox::cpCheckBox(sf::RenderWindow *parent, cpGuiContainer *GUI,
					   std::string label, float posx, float posy) : cpButton(parent,
					   GUI, label, posx, posy, CP_DEFAULT, CP_DEFAULT)
{
	labelInside = false;
	isChecked = false;
	Width = Label.GetRect().GetHeight() - 4;
	Height = Width;
	CreateRects(Label.GetText());
}

cpCheckBox::cpCheckBox() : cpButton(){}

/// Updates the look of the check box
void cpCheckBox::CreateRects(std::string label)
{
	Label.SetText(label);
	Label.SetColor(textColor);
	Width = Label.GetRect().GetHeight() - 4;
	if(Width < 20)
		Width = 20;
	Height = Width;


	backRect = sf::Shape::Rectangle(PosX, PosY, PosX+Width, PosY+Height,
		backgroundColor, 1.f, outlineColor);

	gradient = sf::Shape::Rectangle(PosX+2, PosY+2, PosX+Width-2,
		PosY+Height-2, backgroundColor, 1.f, backgroundColor2);
	sf::Color color = sf::Color(backgroundColor2.r+35,
		backgroundColor2.g+35, backgroundColor2.b+35);
	gradient.SetPointColor(0, backgroundColor2);
	gradient.SetPointColor(1,color);
	gradient.SetPointColor(2, backgroundColor);
	gradient.SetPointColor(3, color);

	float thick = Width/10;

	line1 = sf::Shape::Line(PosX+3, PosY+(Height/2), PosX+thick*2, PosY+(Height/2),
		thick, textColor);
	line2 = sf::Shape::Line(PosX+thick*2, PosY+(Height/2), PosX+Width-(Width*2/3), PosY+Height-3,
		thick, textColor);
	line3 = sf::Shape::Line(PosX+Width-(Width*2/3), PosY+Height-3, PosX+Width-3, PosY+3,
		thick, textColor);

	Label.SetPosition(PosX + Width + 4, PosY+Height/3 - Label.GetRect().GetHeight()/2);
}

/// Draws all the parts of the checkbox
void cpCheckBox::Draw()
{
	if(!bShow)
		return;

	cpButton::Draw();

	if(isChecked)
	{
		Parent->Draw(line1);
		Parent->Draw(line2);
		Parent->Draw(line3);
	}
}

/// This function checks for mouse events within the control.
/// it updates the look of the control according to the event.
/// it also returns an integer enumeration member to inform the user.
/// possible return values are:
/// CP_ST_NONE, CP_ST_MOUSE_ENTER, CP_ST_MOUSE_EXIT,
/// CP_ST_MOUSE_IN, CP_ST_LBUTTON_DOWN, CP_ST_LBUTTON_RELEASED.
///
/// The difference between CP_ST_MOUSE_ENTER & CP_ST_MOUSE_IN
/// is that IN is active the entire time the mouse is inside the control
/// and ENTER is only active right when the mouse enters the control
int cpCheckBox::CheckState(const sf::Input *input)
{
	int state = cpButton::CheckState(input);

	if(state == CP_ST_NONE)
	{
		if(hasFocus)
			outlineColor = sf::Color(90,90,230);
		else
			outlineColor = sf::Color::Black;
	}
	else
		outlineColor = outlineColorTemp;

	if(state == CP_ST_MOUSE_LBUTTON_RELEASED)
		isChecked = !isChecked;

	CreateRects(Label.GetText());

	return state;
}

/// This is not available.  The size is based on
/// the size of the Label's font.
bool cpCheckBox::SetSize(float width, float height)
{
	return false;
}

/// Set whether or not the box is checked
void cpCheckBox::SetChecked(bool checked)
{
	isChecked = checked;
}

/// Returns the state of the check box
bool cpCheckBox::GetChecked()
{
	return isChecked;
}


/// ***********************************************************
/// Rudimentary text input box.  It only allows you to enter
/// the number of characters that the box can contain.  The
/// font size is tied to the height of the box, so you can't
/// change the font size.
///
/// You can set CP_TXT_LEFT_ALIGN, CP_TXT_RIGHT_ALIGN, or
/// CP_TXT_CENTER_ALIGN.  This simple text box does not
/// support text selection, but you can backspace.
/// ***********************************************************
cpTextInputBox::cpTextInputBox(sf::RenderWindow *parent, cpGuiContainer *GUI,
							   std::string label, float posx, float posy,
							   float width, float height, int Style) : cpObject(
							   parent, GUI, label, posx, posy, width, height)
{
	backgroundColor = sf::Color::White;
	bTooBig = false;
	caretOn = true;
	elapsedTime = 0.f;
	SetFont(Font, (unsigned int)Height-2);
	Label.SetSize(Height-2);
	tempLabel = Label;
	tempText = Label.GetText();

	style = Style;
	if(style < CP_TXT_RIGHT_ALIGN || style > CP_TXT_CENTER_ALIGN)
		style = CP_TXT_LEFT_ALIGN;

	CreateRects(Label.GetText());

}

cpTextInputBox::cpTextInputBox() : cpObject(NULL, NULL, ""){}

/// Updates the look of the text input box.
/// Computes the text alignement and position of the
/// Caret/cursor.
void cpTextInputBox::CreateRects(std::string label)
{
	Label.SetText(label);
	Label.SetColor(textColor);
	Label.SetSize(Height-2);
	tempLabel = Label;

	float thickness = 1.f;
	backRect = sf::Shape::Rectangle(PosX, PosY, PosX+Width, PosY+Height,
		backgroundColor, thickness);
	for(int t=0; t<4; t++)
		backRect.SetPointOutlineColor(t, outlineColor);

	if(style == CP_TXT_RIGHT_ALIGN)
		Label.SetPosition(PosX+Width-Label.GetRect().GetWidth()-8, PosY-Height/6);
	if(style == CP_TXT_LEFT_ALIGN)
		Label.SetPosition(PosX+4, PosY-Height/6);
	if(style == CP_TXT_CENTER_ALIGN)
		Label.SetPosition(PosX+(Width/2)-(Label.GetRect().GetWidth()/2),
			PosY-Height/6);

	caret = sf::Shape::Line(Label.GetPosition().x + Label.GetRect().GetWidth() + 3,
		PosY+2, Label.GetPosition().x + Label.GetRect().GetWidth() + 3, PosY+Height-2,
		1.f, sf::Color(50,50,50));

}

/// Draws the text input box. The cursor/caret is
/// drawn every .75 seconds
void cpTextInputBox::Draw()
{
	if(!bShow)
		return;

	Parent->Draw(backRect);
	Parent->Draw(Label);

	if(gui->clock.GetElapsedTime() - elapsedTime > 0.75)
	{
		elapsedTime = gui->clock.GetElapsedTime();
		caretOn = !caretOn;
	}

	if(hasFocus && caretOn)
		Parent->Draw(caret);
}

/// This function checks for mouse events within the control.
/// it updates the look of the control according to the event.
/// it also returns an integer enumeration member to inform the user.
/// possible return values are:
/// CP_ST_NONE, CP_ST_MOUSE_ENTER, CP_ST_MOUSE_EXIT,
/// CP_ST_MOUSE_IN, CP_ST_LBUTTON_DOWN, CP_ST_LBUTTON_RELEASED.
///
/// The difference between CP_ST_MOUSE_ENTER & CP_ST_MOUSE_IN
/// is that IN is active the entire time the mouse is inside the control
/// and ENTER is only active right when the mouse enters the control
int cpTextInputBox::CheckState(const sf::Input *input)
{
	int state = cpObject::CheckState(input);

	if(state == CP_ST_NONE)
	{
		if(hasFocus)
			outlineColor = sf::Color(90,90,230);
		else
			outlineColor = sf::Color::Black;
	}
	else
		outlineColor = outlineColorTemp;

	CreateRects(Label.GetText());
	return state;
}

/// This function is used within the Event loop.
/// This accepts text input and assigns it to
/// the Label if it fits in the box.
void cpTextInputBox::ProcessTextInput(sf::Event *evt)
{
	if(!hasFocus)
		return;
	if (evt->Type == sf::Event::TextEntered)
    {
        int textSize = tempText.size();
        unsigned short unicode = evt->Text.Unicode;

        if (unicode == 8) //If backspace
		{
            if (textSize > 0)
                tempText.erase(textSize - 1, 1);
		}
        else if (unicode >= 32 && unicode <= 126)
            tempText += (char)unicode;
        else if (unicode >= 192 && unicode <= 255)
            tempText += (char)unicode;
		return;
	}

    tempLabel.SetText(tempText);
	if(CheckTextFit())
		Label.SetText(tempText);
}

/// Limits the size of the Label to the size of the box
bool cpTextInputBox::CheckTextFit()
{
	if(tempLabel.GetRect().GetWidth() > Width - 8)
	{
		tempText.erase(tempText.size()-1, 1);
		return false;
	}
	return true;
}

/// Set the width & height dimensions of the text input box.
/// also reloads the font with the new font size for
/// better text quality
bool cpTextInputBox::SetSize(float width, float height)
{
	Width = width;
	Height = height;
	SetFont(Font, (unsigned int)Height-2);
	CreateRects(Label.GetText());
	return true;
}

/// Sets the coordinates for the position of the text input box
void cpTextInputBox::SetPosition(float posx, float posy)
{
	cpObject::SetPosition(posx, posy);
	CreateRects(Label.GetText());
}

/// The font can be changed, but the font size
/// is determined by the size of the box.
void cpTextInputBox::SetFont(std::string filename, unsigned int size)
{
	cpObject::SetFont(filename, (unsigned int)Height-2);
	CreateRects(Label.GetText());
}

/// Sets the text input box's background color
void cpTextInputBox::SetBackgroundColor(sf::Color color)
{
	cpObject::SetBackgroundColor(color);
	CreateRects(Label.GetText());
}

/// Sets the font's style: Bold, Italic, Underlined, Regular
void cpTextInputBox::SetFontStyle(unsigned long TextStyle)
{
	cpObject::SetFontStyle(TextStyle);
	CreateRects(Label.GetText());
}

/// Not available.  Font size is determined
/// by the box's height.
void cpTextInputBox::SetFontSize(unsigned int size)
{
}

/// Sets the color of the text
void cpTextInputBox::SetLabelColor(sf::Color color)
{
	cpObject::SetLabelColor(color);
	CreateRects(Label.GetText());
}

/// Sets the text displayed in the text input box
void cpTextInputBox::SetLabelText(std::string text)
{
	cpObject::SetLabelText(text);
	CreateRects(Label.GetText());
}


/// **************************************************************
/// A clickable button based on an sf::Shape.
/// **************************************************************
cpShapeButton::cpShapeButton(sf::RenderWindow *parent, cpGuiContainer *GUI, sf::Shape *shape,
							 float posx, float posy) : cpObject(parent, GUI, "", posx, posy)
{
	button = shape;
	ComputeRectDimensions();
	SetPosition(PosX, PosY);

}

cpShapeButton::cpShapeButton() : cpObject(NULL, NULL, ""){}

/// Sets the shape that makes the button
void cpShapeButton::SetShape(sf::Shape *shape)
{
	button = shape;
	ComputeRectDimensions();
	CreateRects(Label.GetText());
}

/// Draws the shape button
void cpShapeButton::Draw()
{
	if(!bShow)
		return;

	Parent->Draw(*button);
}

/// This function checks for mouse events within the control.
/// it updates the look of the control according to the event.
/// it also returns an integer enumeration member to inform the user.
/// possible return values are:
/// CP_ST_NONE, CP_ST_MOUSE_ENTER, CP_ST_MOUSE_EXIT,
/// CP_ST_MOUSE_IN, CP_ST_LBUTTON_DOWN, CP_ST_LBUTTON_RELEASED.
///
/// The difference between CP_ST_MOUSE_ENTER & CP_ST_MOUSE_IN
/// is that IN is active the entire time the mouse is inside the control
/// and ENTER is only active right when the mouse enters the control
int cpShapeButton::CheckState(const sf::Input *input)
{
	int state = cpObject::CheckState(input);
	if(state == CP_ST_NONE)
	{
		if(hasFocus)
			outlineColor = sf::Color(90,90,230);
		else
			outlineColor = sf::Color::Black;
	}
	else
		outlineColor = outlineColorTemp;


	CreateRects("");
	return state;
}

/// This function is unavailable in this class.
bool cpShapeButton::SetSize(float width, float height)
{
	return false;
}

/// Moves the shape button to new coordinates
void cpShapeButton::SetPosition(float posx, float posy)
{
	PosX = posx;
	PosY = posy;
	button->Move(PosX-relX, PosY-relY);
	relX = PosX;
	relY = PosY;
	CreateRects("");
}

/// Updates the look of the shape button
void cpShapeButton::CreateRects(std::string label)
{
	button->SetOutlineWidth(1.f);
	for(unsigned int t=0; t < button->GetNbPoints(); t++)
		button->SetPointOutlineColor(t, outlineColor);
}

/// Computes the width and height of the rectangle
/// that the shape fits in. Also the coordinates of
/// the top left corner
void cpShapeButton::ComputeRectDimensions()
{
	int left=0, right=0, top=0, bottom=0;
	for(unsigned int t=1; t < button->GetNbPoints(); t++)
	{
		if(button->GetPointPosition(t).x < button->GetPointPosition(t-1).x)
			left = t;
		if(button->GetPointPosition(t).x > button->GetPointPosition(t-1).x)
			right = t;
		if(button->GetPointPosition(t).y < button->GetPointPosition(t-1).y)
			top = t;
		if(button->GetPointPosition(t).y > button->GetPointPosition(t-1).y)
			bottom = t;
	}
	Width = button->GetPointPosition(right).x - button->GetPointPosition(left).x;
	Height = button->GetPointPosition(bottom).y - button->GetPointPosition(top).y;
	relX = button->GetPointPosition(left).x;
	relY = button->GetPointPosition(top).y;
}

/// This function is unavailable in this class
void cpShapeButton::SetFont(std::string filename, unsigned int size){}

/// This function is unavailable in this class
void cpShapeButton::SetBackgroundColor(sf::Color color){}

/// This function is unavailable in this class
void cpShapeButton::SetFontSize(unsigned int size){}

/// This function is unavailable in this class
void cpShapeButton::SetFontStyle(unsigned long TextStyle){}

/// This function is unavailable in this class
void cpShapeButton::SetLabelColor(sf::Color color){}

/// This function is unavailable in this class
void cpShapeButton::SetLabelText(std::string text){}


/// ********************************************************************
/// Vertical scrollbar
///
/// ********************************************************************
cpScrollBar::cpScrollBar(sf::RenderWindow *parent, cpGuiContainer *GUI,
						 float length, int ViewableLines, int TotalLines,
						 float posx, float posy)
{
	Parent = parent;
	gui = GUI;
	Length = length;
	viewableLines = ViewableLines;
	totalLines = TotalLines;
	PosX = posx;
	PosY = posy;

	if(viewableLines >= totalLines)
		viewableLines = totalLines;
	slLength = Length - 30;
	if(totalLines > 0)
		slLength = (Length-30)*viewableLines/totalLines;

	backgroundColor = sf::Color(240,240,240);
	backgroundColor2 = sf::Color(170,170,170);
	mouseDown = false;
	slDown = false;
	clickGroove = false;
	sliderDrag = false;
	sf::Vector2f mousePos = Parent->ConvertCoords(Parent->GetInput().GetMouseX(),
		Parent->GetInput().GetMouseY());
	mouseY = mousePos.y;
	lineInc = (Length - slLength - 30) / (totalLines-viewableLines);
	clock.Reset();

	upButton = new cpButton(Parent, gui, " ", PosX, PosY, 15,15);
	downButton = new cpButton(Parent, gui, " ", PosX, PosY+Length-15, 15, 15);
	groove = sf::Shape::Rectangle(PosX+1, PosY+15, PosX+14, PosY+Length-15,
		sf::Color(backgroundColor),2.f, sf::Color(200,200,200));

	sliderShape.AddPoint(PosX, PosY+15, backgroundColor);
	sliderShape.AddPoint(PosX+7.5f, PosY+15, backgroundColor);
	sliderShape.AddPoint(PosX+15, PosY+15, backgroundColor2);
	sliderShape.AddPoint(PosX+15, PosY+15+slLength, backgroundColor2);
	sliderShape.AddPoint(PosX+7.5f, PosY+15+slLength, backgroundColor);
	sliderShape.AddPoint(PosX, PosY+15+slLength, backgroundColor);
	sliderShape.SetOutlineWidth(1.f);
	currentPosition = 15;

	upArrow.AddPoint(PosX+7.5f, PosY+2.5f, sf::Color::Black, backgroundColor);
	upArrow.AddPoint(PosX+12, PosY+11, sf::Color::Black, backgroundColor);
	upArrow.AddPoint(PosX+3, PosY+11, sf::Color::Black, backgroundColor);
	upArrow.SetOutlineWidth(1.f);

	downArrow.AddPoint(PosX+3, PosY+Length-11, sf::Color::Black, backgroundColor);
	downArrow.AddPoint(PosX+12, PosY+Length-11, sf::Color::Black, backgroundColor);
	downArrow.AddPoint(PosX+7.5f, PosY+Length-2.5f, sf::Color::Black, backgroundColor);
	downArrow.SetOutlineWidth(1.f);

	MoveLines();

	gui->Unregister(upButton);
	gui->Unregister(downButton);
}

cpScrollBar::cpScrollBar(){}

cpScrollBar::~cpScrollBar()
{
	delete downButton;
	downButton = NULL;
	delete upButton;
	upButton = NULL;
}

/// Draws the different parts that make up a scrollbar
/// if the slider is long enough, draw the "grab lines"
/// in the center
void cpScrollBar::Draw()
{
	Parent->Draw(groove);
	upButton->Draw();
	Parent->Draw(upArrow);
	downButton->Draw();
	Parent->Draw(downArrow);
	Parent->Draw(sliderShape);
	if(slLength > 35)
	{
		Parent->Draw(line1);
		Parent->Draw(line2);
		Parent->Draw(line3);
	}
}

/// This controls the various ways to move the slider
/// and returns an integer that is the position of the slider.
/// This position represents the line to start with.
int cpScrollBar::CheckState(const sf::Input *input)
{
	int upState = upButton->CheckState(input);
	int dnState = downButton->CheckState(input);
	sf::Vector2f mousePos = Parent->ConvertCoords(input->GetMouseX(), input->GetMouseY());
	if(CheckMouseSlider(input) == CP_ST_MOUSE_LBUTTON_DOWN && !mouseDown)
	{
		mouseY = mousePos.y;
		sliderDrag = true;
		mouseDown = true;
	}
	if(sliderDrag && currentPosition > 14 && currentPosition < PosY + Length
		- slLength - 14)
	{
		float temp = mousePos.y;
		currentPosition = currentPosition + temp - mouseY;
		CheckSliderValid();
		MoveLines();
		mouseY = temp;
	}
	if(CheckMouseSlider(input) == CP_ST_MOUSE_LBUTTON_RELEASED ||
		(!input->IsMouseButtonDown(sf::Mouse::Left) && sliderDrag))
	{
		sliderDrag = false;
		mouseDown = false;
	}

	if(upState == CP_ST_MOUSE_LBUTTON_DOWN && !mouseDown && clock.GetElapsedTime() > 0.1f)
	{
		mouseDown = true;
		MoveSlider(-1);
		clock.Reset();
		clock2.Reset();
	}

	if(upState == CP_ST_MOUSE_LBUTTON_DOWN && mouseDown && clock.GetElapsedTime() > 0.7f
		&& clock2.GetElapsedTime() > 0.1f && currentPosition > 14)
	{
		MoveSlider(-1);
		clock2.Reset();
	}
	if(upState == CP_ST_MOUSE_LBUTTON_RELEASED || dnState == CP_ST_MOUSE_LBUTTON_RELEASED)
		mouseDown = false;

	if(dnState == CP_ST_MOUSE_LBUTTON_DOWN && !mouseDown && clock.GetElapsedTime() > 0.1f)
	{
		mouseDown = true;
		MoveSlider(1);
		clock.Reset();
		clock2.Reset();
	}

	if(dnState == CP_ST_MOUSE_LBUTTON_DOWN && mouseDown && clock.GetElapsedTime() > 0.7f
		&& clock2.GetElapsedTime() > 0.1f && currentPosition > 14)
	{
		MoveSlider(1);
		float c1 = clock.GetElapsedTime();
		float c2 = clock2.GetElapsedTime();
		clock2.Reset();
	}
	if(input->IsMouseButtonDown(sf::Mouse::Left) && !mouseDown && !sliderDrag &&
		CheckMouseEntered(input))
		clickGroove = true;

	if(!input->IsMouseButtonDown(sf::Mouse::Left) && clickGroove)
	{
		int d = -1;
		if(mousePos.y > PosY+currentPosition+slLength)
			d = 1;
		for(int t=0; t < viewableLines; t++)
			MoveSlider(d);
		clickGroove = false;
	}

	int position = 0;
	if(Length-30-slLength > 0)
	{
		position = int((currentPosition - 15)/
			(Length-30 - slLength)*(totalLines-viewableLines) + 0.5);
	}

	CreateRects();

	return position;
}

/// This moves the "grab lines" relative to the slider
void cpScrollBar::MoveLines()
{
	float temp = PosY+currentPosition + slLength / 2;
	line1 = sf::Shape::Line(PosX+3, temp-4, PosX+12, temp-4,
		2.f, backgroundColor2, 1.f, backgroundColor);
	line2 = sf::Shape::Line(PosX+3, temp, PosX+12, temp,
		2.f, backgroundColor2, 1.f, backgroundColor);
	line3 = sf::Shape::Line(PosX+3, temp+4, PosX+12, temp+4,
		2.f, backgroundColor2, 1.f, backgroundColor);
}

/// Moves the slider by the amount of one line increment.
/// direction = 1 for down & -1 for up
void cpScrollBar::MoveSlider(short direction)
{
	currentPosition = currentPosition + lineInc * direction;

	CheckSliderValid();

	MoveLines();
}

/// If the slider is too far down, or too far up,
/// put it within the limits.
void cpScrollBar::CheckSliderValid()
{
	if(currentPosition > Length - slLength - 15)
		currentPosition = Length - slLength - 15;
	if(currentPosition < 15)
		currentPosition = 15;
}

/// Checks to see if mouse pointer is inside control.  Returns true if it is.
bool cpScrollBar::CheckMouseEntered(const sf::Input *input)
{
	sf::Vector2f mousePos = Parent->ConvertCoords(input->GetMouseX(), input->GetMouseY());
	return (mousePos.x >= PosX && mousePos.x <= PosX+15 &&
		mousePos.y >= PosY && mousePos.y <= PosY+Length);
}

/// This checks the state of the mouse inside the shape
/// that makes up the slider and returns an integer that
/// represents the mouse activity
int cpScrollBar::CheckMouseSlider(const sf::Input *input)
{
	sf::Vector2f mousePos = Parent->ConvertCoords(input->GetMouseX(), input->GetMouseY());
	if(mousePos.x >= PosX && mousePos.x <= PosX+15 &&
		mousePos.y >= PosY+currentPosition &&
		mousePos.y <= PosY+currentPosition+slLength)
	{
		if(input->IsMouseButtonDown(sf::Mouse::Left))
		{
			slDown = true;
			return CP_ST_MOUSE_LBUTTON_DOWN;
		}
	}
	if(!input->IsMouseButtonDown(sf::Mouse::Left) && slDown)
	{
		slDown = false;
		return CP_ST_MOUSE_LBUTTON_RELEASED;
	}

	return CP_ST_NONE;
}

/// Recompute the slider based on a new number of viewable lines
/// Vs. total lines
void cpScrollBar::Repopulate(int ViewableLines, int TotalLines)
{
	viewableLines = ViewableLines; //14
	totalLines = TotalLines; // 10
	if(viewableLines >= totalLines)
		viewableLines = totalLines; // vl = 10
	slLength = Length - 30;
	if(totalLines > 0)
		slLength = (Length-30)*viewableLines/totalLines;
	lineInc = 0;
	if(totalLines > viewableLines)
		lineInc = (Length - slLength - 30) / (totalLines-viewableLines);
	CreateRects();
}

/// Sets the length of the scroll bar and recomputes the slider
/// based on the new length and new number of viewable lines
void cpScrollBar::SetSize(float length, int ViewableLines)
{
	viewableLines = (int)ViewableLines;
	Length = length;
	if(viewableLines > totalLines-1)
	{
		viewableLines = totalLines;
	}
	slLength = Length - 30;
	if(totalLines > 0)
		slLength = (Length-30)*viewableLines/totalLines;

	currentPosition = 15;
	lineInc = 0;
	if(totalLines > viewableLines)
		lineInc = (Length - slLength - 30) / (totalLines-viewableLines);
	CreateRects();
}

/// Updates the look of the scrollbar
void cpScrollBar::CreateRects()
{
	groove = sf::Shape::Rectangle(PosX+1, PosY+15, PosX+14, PosY+Length-15,
		sf::Color(backgroundColor),2.f, sf::Color(200,200,200));

	sliderShape.SetPointPosition(0,PosX, PosY+currentPosition);
	sliderShape.SetPointPosition(1,PosX+7.5f, PosY+currentPosition);
	sliderShape.SetPointPosition(2,PosX+15, PosY+currentPosition);
	sliderShape.SetPointPosition(3,PosX+15, PosY+currentPosition+slLength);
	sliderShape.SetPointPosition(4,PosX+7.5f, PosY+currentPosition+slLength);
	sliderShape.SetPointPosition(5,PosX, PosY+currentPosition+slLength);
	sliderShape.SetOutlineWidth(1.f);
	for(int t=0; t < 6; t++)
		sliderShape.SetPointOutlineColor(t, sf::Color::Black);
	MoveLines();

	upButton->SetPosition(PosX, PosY);
	downButton->SetPosition(PosX, PosY+Length-15);

	upArrow.SetPointPosition(0,PosX+7.5f, PosY+2.5f);
	upArrow.SetPointPosition(1,PosX+12, PosY+11);
	upArrow.SetPointPosition(2,PosX+3, PosY+11);

	downArrow.SetPointPosition(0,PosX+3, PosY+Length-11);
	downArrow.SetPointPosition(1,PosX+12, PosY+Length-11);
	downArrow.SetPointPosition(2,PosX+7.5f, PosY+Length-2.5f);
}

/// Sets new coordinates for the position of the scrollbar
void cpScrollBar::SetPosition(float posx, float posy)
{
	currentPosition = 15;
	PosX = posx;
	PosY = posy;
	CreateRects();
}




/// *************************************************************************
/// This class is a selection box.  Choices can be added, and
/// those choices will be listed in the box.  A selection can
/// be made either by clicking on the choice, or with the keyboard
/// *************************************************************************
cpSelectionBox::cpSelectionBox(sf::RenderWindow *parent, cpGuiContainer *GUI,
							   float posx, float posy, float width, float height) :
cpObject(parent, GUI, "", posx, posy, width, height)
{
	currentSelection = 0;
	currentStartingLine = 0;
	backgroundColor = sf::Color::White;
	CreateRects("");
	Label.SetText("Jason Cupp");
	fontSize = textFont->GetCharacterSize();
	viewableLines = int((Height-5)/(fontSize+5));
	mouseDown = false;
	keyDown = false;
	clock.Reset();

	SetUseWheel(true);

	scrollbar = new cpScrollBar(Parent, gui, Height, viewableLines, choices.size(),
		PosX+Width-15, PosY);


	gui->AcceptsKeys(true, false);
}

cpSelectionBox::cpSelectionBox() : cpObject(NULL, NULL, "") {}

cpSelectionBox::~cpSelectionBox()
{
	delete scrollbar;
	scrollbar = NULL;
}

/// Updates the look of the selection box and draws
/// all of the choices with the current selection highlighted
void cpSelectionBox::Draw()
{
	if(!bShow)
		return;

	Parent->Draw(backRect);
	short barWidth = 0;
	if((unsigned int)viewableLines < choices.size())
	{
		scrollbar->Draw();
		barWidth = 15;
	}

	if(choices.size() > 0)
	{
		int max = choices.size();
		if(choices.size() > (unsigned int)viewableLines)
			max = viewableLines;
		if(currentSelection >= currentStartingLine && currentSelection <
			currentStartingLine + max)
		{
			selBox = sf::Shape::Rectangle(PosX, choices[currentSelection].GetPosition().y,
				PosX+Width-barWidth, choices[currentSelection].GetPosition().y+fontSize+5,
				sf::Color::Blue);
			Parent->Draw(selBox);
		}
		for(int t=0; t < max; t++)
		{
			choices[t+currentStartingLine].SetPosition(PosX+10, PosY+((Height-viewableLines*
				(fontSize+5))/2)+((fontSize+5)*t));
			if(currentSelection == t+currentStartingLine)
				choices[t+currentStartingLine].SetColor(sf::Color::White);
			else
				choices[t+currentStartingLine].SetColor(textColor);
			Parent->Draw(choices[t+currentStartingLine]);
		}
	}
}

/// Function to add a choice to the choice list.
/// The new addition will be placed on the bottom of the list.
void cpSelectionBox::AddChoice(std::string choice)
{
	sf::String temp = Label;
	temp.SetText(choice);
	choices.push_back(temp);
	CheckTextWidth();
	scrollbar->Repopulate(viewableLines, choices.size());
}

/// Removes the last choice from the list
void cpSelectionBox::RemoveLastChoice()
{
	if(choices.size() <= 0)
		return;
	choices.pop_back();
	if(currentStartingLine > 0)
		currentStartingLine--;
	if(unsigned(currentSelection) >= choices.size())
		currentSelection -= 1;
	scrollbar->Repopulate(viewableLines, choices.size());
	scrollbar->MoveSlider(0);
}

/// Makes sure the text of all choices fits in the selection box
/// BEWARE: if the text is too wide for the box, it will be
/// truncated never to be seen again.
void cpSelectionBox::CheckTextWidth()
{
	if(choices.size() < 1)
		return;
	std::string temp;
	for(unsigned int t=0; t < choices.size(); t++)
	{
		temp = choices[t].GetText();
		while(choices[t].GetRect().GetWidth() > Width-30)
		{
			temp.erase(temp.length()-1);
			choices[t].SetText(temp);
		}
	}
}

/// This function checks for mouse events within the control.
/// it updates the look of the control according to the event.
/// it also returns an integer enumeration member to inform the user.
/// possible return values are:
/// CP_ST_NONE, CP_ST_MOUSE_ENTER, CP_ST_MOUSE_EXIT,
/// CP_ST_MOUSE_IN, CP_ST_LBUTTON_DOWN, CP_ST_LBUTTON_RELEASED.
///
/// The difference between CP_ST_MOUSE_ENTER & CP_ST_MOUSE_IN
/// is that IN is active the entire time the mouse is inside the control
/// and ENTER is only active right when the mouse enters the control
///
/// In this class, the mouse wheel delta information is taken from
/// the GuiContainer to update the scrollbar
int cpSelectionBox::CheckState(const sf::Input *input)
{
	int state = cpObject::CheckState(input);
	sf::Vector2f mousePos = Parent->ConvertCoords(input->GetMouseX(), input->GetMouseY());

	if(state == CP_ST_NONE)
	{
		if(hasFocus)
			outlineColor = sf::Color(90,90,230);
		else
			outlineColor = sf::Color::Black;
	}
	else
		outlineColor = outlineColorTemp;

	currentStartingLine = scrollbar->CheckState(input);
	if(wheelDelta == 1)
	{
		scrollbar->MoveSlider(-1);
		wheelDelta = 0;
	}
	if(wheelDelta == -1)
	{
		scrollbar->MoveSlider(1);
		wheelDelta = 0;
	}

	if(input->IsKeyDown(sf::Key::Down) && (unsigned int)currentSelection < choices.size()-1
		&& hasFocus && clock.GetElapsedTime() > .3f)
	{
		currentSelection++;
		if(currentSelection >= currentStartingLine+viewableLines)
		{
			scrollbar->MoveSlider(1);
			currentStartingLine++;
		}
		clock.Reset();
	}
	if(input->IsKeyDown(sf::Key::Up) && currentSelection > 0 &&
		hasFocus && clock.GetElapsedTime() > .3)
	{
		currentSelection--;
		if(currentSelection < currentStartingLine)
		{
			scrollbar->MoveSlider(-1);
			currentStartingLine--;
		}
		clock.Reset();
	}


	if(input->IsMouseButtonDown(sf::Mouse::Left))
	{
		if(mousePos.x > PosX && mousePos.x < PosX+Width-15 && mousePos.y > PosY &&
			mousePos.y < PosY+Height)
			mouseDown = true;
	}

	if(input->IsMouseButtonDown(sf::Mouse::Left) && mouseDown)
	{
		unsigned int max = viewableLines;
		if(max > choices.size())
			max = choices.size();
		for(unsigned int t=0; t < max; t++)
		{
			if(mousePos.y >= choices[t+currentStartingLine].GetPosition().y &&
				mousePos.y <= choices[t+currentStartingLine].GetPosition().y+fontSize+5)
				currentSelection = t+currentStartingLine;
		}
		mouseDown = false;
	}
	CreateRects("");
	return state;
}

/// Sets the current selection. An integer representing
/// the index of the selection starting at 0.
void cpSelectionBox::SetSelection(int sel)
{
	currentSelection = sel;
}

/// Returns an integer starting at 0 representing the
/// current selection's index.
int cpSelectionBox::GetSelection()
{
	return currentSelection;
}

/// Sets the width & height dimensions of the selection box.
/// BEWARE: if you make the box smaller than the width of your
/// text, your text will be truncated and lost
bool cpSelectionBox::SetSize(float width, float height)
{
	Width = width;
	Height = height;
	CreateRects(Label.GetText());
	viewableLines = int((Height-5)/(fontSize+5));
	scrollbar->SetSize(Height, viewableLines);
	scrollbar->SetPosition(PosX+Width-15, PosY);
	return false;
}

/// Updates the look of the selection box
void cpSelectionBox::CreateRects(std::string label)
{
	cpObject::CreateRects(label);
}

/// Sets the font's style: Bold, Italic, Underlined, Regular
void cpSelectionBox::SetFontStyle(unsigned long TextStyle)
{
	cpObject::SetFontStyle(TextStyle);
	for(unsigned int t=0; t < choices.size(); t++)
		choices[t].SetStyle(TextStyle);
	CheckTextWidth();
}

/// Sets the font's size.  The current font is reloaded
/// with the new size for better quality.
/// BEWARE: if resizing the font makes the text too wide for
/// your box, it will be truncated and lost
void cpSelectionBox::SetFontSize(unsigned int size)
{
	cpObject::SetFontSize(size);
	for(unsigned int t=0; t < choices.size(); t++)
	{
		choices[t].SetFont(*textFont);
		choices[t].SetSize(Label.GetSize());
	}
	fontSize = size;
	viewableLines = int((Height-5)/(fontSize+5));
	CheckTextWidth();
	scrollbar->SetSize(Height, viewableLines);
}

/// Loads a font for the text. Use a filename of "Default" for SFML's
/// default font.
/// BEWARE: if the size the font makes the text too wide for
/// your box, it will be truncated and lost
void cpSelectionBox::SetFont(std::string filename, unsigned int size)
{
	cpObject::SetFont(filename, size);
	for(unsigned int t=0; t < choices.size(); t++)
	{
		choices[t].SetFont(*textFont);
		choices[t].SetSize(float(textFont->GetCharacterSize()));
	}
	fontSize = size;
	viewableLines = int((Height-5)/(fontSize+5));
	CheckTextWidth();
	scrollbar->SetSize(Height, viewableLines);
}

/// Sets the coordinates for the position of the selection box
void cpSelectionBox::SetPosition(float posx, float posy)
{
	cpObject::SetPosition(posx, posy);
	scrollbar->SetPosition(posx+Width-15, posy);
	CreateRects("");
}

/// Sets the background color of the selection box.
void cpSelectionBox::SetBackgroundColor(sf::Color color)
{
	cpObject::SetBackgroundColor(color);
	CreateRects("");
}



// **********************************************************************
// Drop down box
// **********************************************************************
cpDropDownBox::cpDropDownBox(sf::RenderWindow *parent, cpGuiContainer *GUI,
							 std::string label, float posx, float posy,
							 float width, float height) : cpObject(parent,
							 GUI, label, posx, posy, width, height)
{
	backgroundColor = sf::Color::White;
	textColor = sf::Color(180,180,180);
	tempTextColor = sf::Color::Black;
	maxDepth = 100;
	depth = Height;

	CreateRects(label);
	CheckTextWidth();

	downBtn = new cpButton(Parent, gui, " ", PosX+Width-Height, PosY, Height, depth);
	btnBackground = downBtn->GetBackgroundColor();

	downArrow.AddPoint(PosX+Width-Height+Height/3, PosY+Height/3, sf::Color::Black, btnBackground);
	downArrow.AddPoint(PosX+Width-Height+Height*2/3, PosY+Height/3, sf::Color::Black, btnBackground);
	downArrow.AddPoint(PosX+Width-Height+Height/2, PosY+Height*2/3, sf::Color::Black, btnBackground);
	downArrow.SetOutlineWidth(1.f);

	sBox = new cpSelectionBox(Parent, gui, PosX, PosY+Height, Width, Height);
	if(PosY+Height+sBox->GetSize().y > Parent->GetHeight())
		sBox->SetPosition(PosX, PosY-sBox->GetSize().y);
	sBox->Show(false);

	SetFont(Font, (unsigned int)Height-4);


	gui->Unregister(downBtn);
}

cpDropDownBox::~cpDropDownBox()
{
	delete downBtn;
	downBtn = NULL;
	delete sBox;
	sBox = NULL;
}

/// Updates the look of the dropdown box.
void cpDropDownBox::CreateRects(std::string label)
{
	backRect = sf::Shape::Rectangle(PosX, PosY, PosX+Width, PosY+Height, backgroundColor,
		1.f, outlineColor);
	Label.SetPosition(PosX+4, PosY-Height/14);
	Label.SetSize(Height-4);
	Label.SetColor(textColor);
}

/// Moves the Down button and it's arrow to a new position.
void cpDropDownBox::MoveButton()
{
	downArrow.SetPointPosition(0,PosX+Width-Height+Height/3, PosY+Height/3);
	downArrow.SetPointPosition(1,PosX+Width-Height+Height*2/3, PosY+Height/3);
	downArrow.SetPointPosition(2,PosX+Width-Height+Height/2, PosY+Height*2/3);
	downBtn->SetPosition(PosX+Width-Height, PosY);
}

/// Draws the dropdown box. If it is activated, it also
/// draws the dropdown box's selection box.
void cpDropDownBox::Draw()
{
	if(!bShow)
		return;
	Parent->Draw(backRect);
	Parent->Draw(Label);
	downBtn->Draw();
	Parent->Draw(downArrow);
	sBox->Draw();
}

/// Checks to make sure the text fits within the width of the box.
/// BEWARE: if the text is too wide, it will be truncated and lost.
void cpDropDownBox::CheckTextWidth()
{
	std::string temp;
	temp = Label.GetText();
	while(Label.GetRect().GetWidth() > Width-15-Height)
	{
		temp.erase(temp.length()-1);
		Label.SetText(temp);
	}
}

/// This function checks for mouse events within the control.
/// it updates the look of the control according to the event.
/// it also returns an integer enumeration member to inform the user.
/// possible return values are:
/// CP_ST_NONE, CP_ST_MOUSE_ENTER, CP_ST_MOUSE_EXIT,
/// CP_ST_MOUSE_IN, CP_ST_LBUTTON_DOWN, CP_ST_LBUTTON_RELEASED.
///
/// The difference between CP_ST_MOUSE_ENTER & CP_ST_MOUSE_IN
/// is that IN is active the entire time the mouse is inside the control
/// and ENTER is only active right when the mouse enters the control
///
/// The Selection Box part of the drop down box also recieves
/// mouse wheel delta information from the GuiContainer.
int cpDropDownBox::CheckState(const sf::Input *input)
{
	int state = cpObject::CheckState(input);

	if(state == CP_ST_NONE)
	{
		if(hasFocus)
			outlineColor = sf::Color(90,90,230);
		else
			outlineColor = sf::Color::Black;
	}
	else
		outlineColor = outlineColorTemp;


	int btnState = downBtn->CheckState(input);
	int sboxState = sBox->CheckState(input);
	if(sboxState == CP_ST_MOUSE_LBUTTON_RELEASED && sBox->GetShow())
	{
		Label.SetText(sBox->choices[sBox->GetSelection()].GetText());
		sBox->Show(false);
		gui->SetObjectFocus(this, true);
		textColor = tempTextColor;
	}

	if(btnState == CP_ST_MOUSE_LBUTTON_RELEASED || state ==
		CP_ST_MOUSE_LBUTTON_RELEASED)
	{
		sBox->Show(!sBox->GetShow());
		if(sBox->GetShow())
			gui->SetObjectFocus(sBox, true);
	}

	CreateRects(Label.GetText());
	return state;
}

/// Add a choice to the selections.  The new choice
/// will be added at the end of the list.
void cpDropDownBox::AddChoice(std::string choice)
{
	if(depth + Height <= maxDepth)
	{
		sBox->SetSize(Width, depth+Height);
		depth += Height;
	}

	sBox->AddChoice(choice);
	sBox->SetFontSize((unsigned int)Height-4);

	if(PosY+Height+depth > Parent->GetHeight())
		sBox->SetPosition(PosX, PosY-depth);
}

/// Removes the last choice in the list of selections.
void cpDropDownBox::RemoveLastChoice()
{
	sBox->RemoveLastChoice();
}


/// Not available.
/// Font size is determined by size of drop down box.
void cpDropDownBox::SetFontSize(unsigned int size)
{
}

/// Sets the text's font style: Bold, Italic, Underlined, Regular
/// Uses SFML's text styles.
void cpDropDownBox::SetFontStyle(unsigned long TextStyle)
{
	cpObject::SetFontStyle(TextStyle);
	CheckTextWidth();
	sBox->SetFontStyle(TextStyle);
}

/// The font can be changed, but the font size will
/// remain relative to the size of the drop down box
void cpDropDownBox::SetFont(std::string filename, unsigned int size)
{
	cpObject::SetFont(filename, (unsigned int)Height-4);
	Label.SetSize(Height-4);
	if(!sBox->choices.empty())
	{
		for(unsigned int t=0; t < sBox->choices.size(); t++)
		{
			sBox->choices[t].SetFont(*textFont);
			sBox->choices[t].SetSize(Height-4);
		}
	}
}

/// Sets the coordinates for the position of the dropdown box
void cpDropDownBox::SetPosition(float posx, float posy)
{
	cpObject::SetPosition(posx, posy);
	MoveButton();
	sBox->SetPosition(PosX, PosY+Height);
	if(PosY+Height+sBox->GetSize().y > Parent->GetHeight())
		sBox->SetPosition(PosX, PosY-sBox->GetSize().y);
	CreateRects(Label.GetText());
}

/// Sets the background color of the dropdown box & its
/// selection box.
void cpDropDownBox::SetBackgroundColor(sf::Color color)
{
	cpObject::SetBackgroundColor(color);
	sBox->SetBackgroundColor(color);
	CreateRects(Label.GetText());
}

/// Sets the width & height dimensions of the drop down box.
/// doesn't change the depth of the selection box area.
/// Also resizes the text font to fit the new height.
///
/// BEWARE: if resizing causes your text to be too wide to fit,
/// it will be trunctated and lost.
bool cpDropDownBox::SetSize(float width, float height)
{
	Width = width;
	Height = height;
	SetFont(Font, (unsigned int)Height-4);
	CheckTextWidth();
	downBtn->SetSize(Height, Height);
	MoveButton();
	CreateRects(Label.GetText());
	sBox->SetSize(Width, sBox->GetSize().y);
	return false;
}

/// The Selection Box area is based on the number of choices.
/// This sets the maximum depth.  After this depth has been reached,
/// a scroll bar will be used.
void cpDropDownBox::SetMaxDepth(float depth)
{
	maxDepth = depth;
}

/// Sets which choice is the current selection.
/// Accepts an integer starting with 0 that represents
/// the selection's index.
void cpDropDownBox::SetSelection(int sel)
{
	sBox->SetSelection(sel);
}

/// Returns the index of the current selection starting
/// with 0.
int cpDropDownBox::GetSelection()
{
	return sBox->GetSelection();
}

/// Sets the color of the text.
void cpDropDownBox::SetLabelColor(sf::Color color)
{
	cpObject::SetLabelColor(color);
	sBox->SetLabelColor(color);
	tempTextColor = color;
}


/// *************************************************************
/// This is a non-editable, read only text box used
/// for displaying long strings of text, or displaying a
/// txt file.  This class automatically word wraps the text
/// to fit the size of the box.  Minimum size is 200x200.
/// *************************************************************
cpTextBox::cpTextBox(sf::RenderWindow *parent, cpGuiContainer *GUI,
					 float posx, float posy, float width, float height) :
cpObject(parent, GUI, "", posx, posy, width, height)
{
	backgroundColor = sf::Color::White;
	if(Width < 200)
		Width = 200;
	if(Height < 200)
		Height = 200;
	Filename = "";
	fontSize = (unsigned int)Label.GetSize();
	totalLines = 0;
	startingLine = 0;
	viewableLines = int((Height-5)/(fontSize+2));
	clock.Reset();

	scrollbar = new cpScrollBar(Parent, gui, Height, viewableLines, totalLines,
		PosX+Width, PosY);

	gui->AcceptsKeys(true, false);
	SetUseWheel(true);
	CreateRects(Label.GetText());
}

cpTextBox::~cpTextBox()
{
	delete scrollbar;
	scrollbar = NULL;
}

/// Draws the text box, any text loaded, and a
/// scrollbar if necessary.
void cpTextBox::Draw()
{
	if(!bShow)
		return;

	Parent->Draw(backRect);
	if(viewableLines < totalLines)
		scrollbar->Draw();
	if(lineSFStrings.size() < 1)
		return;

	int max = lineSFStrings.size();
	if(max > viewableLines)
		max = viewableLines;
	for(int t=0; t < max; t++)
	{
		lineSFStrings[t+startingLine].SetPosition(PosX+5, PosY+((Height-viewableLines*
				(fontSize+2))/2)+((fontSize+2)*t));
		Parent->Draw(lineSFStrings[t+startingLine]);
	}
}

/// Loads a simple text file, word wraps it to fit the
/// Text Box, and sets totalLines for the total number of lines.
/// Rich text is not supported.
void cpTextBox::LoadFile(std::string filename)
{
	totalLines = 0;
	Filename = filename;
	if(Filename == "")
		return;
	lineSFStrings.clear();

    std::ifstream file;
    file.open(filename.c_str());
    //std::ifstream file(filename);
	if(!file.is_open())
		return;

	std::string Line, tempWord, tempLine;
	sf::String sfTemp;
	std::vector<std::string> tokens;
	sfTemp = Label;
	while(std::getline(file, Line))
	{
		tokens.clear();
		std::stringstream words(Line);
		while(std::getline(words, tempWord, ' '))
		{
			tokens.push_back(tempWord);
		}
		tempLine = "";
		int wordinline = 0;
		for(unsigned int t=0; t < tokens.size(); t++)
		{
			wordinline++;
			bool longword = false;
			sfTemp.SetText(tempLine + tokens[t] + " ");

			if(sfTemp.GetRect().GetWidth() > Width - 10 && wordinline == 1)
			{
				longword = true;
				for(unsigned int r=0; r < tokens[t].length(); r++)
				{
					sfTemp.SetText(tempLine + tokens[t][r]);
					if(sfTemp.GetRect().GetWidth() >= Width-10)
					{
						sfTemp.SetText(tempLine);
						lineSFStrings.push_back(sfTemp);
						totalLines++;
						tempLine = "";
					}
					tempLine = tempLine + tokens[t][r];
				}
			}

			if(sfTemp.GetRect().GetWidth() > Width - 10 && wordinline > 1)
			{
				sfTemp.SetText(tempLine);
				lineSFStrings.push_back(sfTemp);
				totalLines++;
				tempLine = "";
				wordinline = 0;
			}
			if(!longword)
				tempLine = tempLine + tokens[t] + " ";
		}
		sfTemp.SetText(tempLine);
		lineSFStrings.push_back(sfTemp);
		totalLines++;
	}
	file.close();
	scrollbar->Repopulate(viewableLines, totalLines);
}

/// This function checks for mouse events within the control.
/// it updates the look of the control according to the event.
/// it also returns an integer enumeration member to inform the user.
/// possible return values are:
/// CP_ST_NONE, CP_ST_MOUSE_ENTER, CP_ST_MOUSE_EXIT,
/// CP_ST_MOUSE_IN, CP_ST_LBUTTON_DOWN, CP_ST_LBUTTON_RELEASED.
///
/// The difference between CP_ST_MOUSE_ENTER & CP_ST_MOUSE_IN
/// is that IN is active the entire time the mouse is inside the control
/// and ENTER is only active right when the mouse enters the control
///
/// This class accepts mouse wheel delta information from the
/// GuiContainer to update the scrollbar.
int cpTextBox::CheckState(const sf::Input *input)
{
	int state = cpObject::CheckState(input);

	if(wheelDelta == 1)
	{
		scrollbar->MoveSlider(-1);
		wheelDelta = 0;
	}
	if(wheelDelta == -1)
	{
		scrollbar->MoveSlider(1);
		wheelDelta = 0;
	}

	if(input->IsKeyDown(sf::Key::Down) && hasFocus && clock.GetElapsedTime() > .15)
	{
		scrollbar->MoveSlider(1);
		//startingLine++;
		clock.Reset();
	}
	if(input->IsKeyDown(sf::Key::Up) && hasFocus && clock.GetElapsedTime() > .15)
	{
		scrollbar->MoveSlider(-1);
		//startingLine--;
		clock.Reset();
	}
	startingLine = scrollbar->CheckState(input);

	return state;
}

/// Sets the width & height dimensions of the Text Box.
/// BEWARE:
/// The current text file will be reloaded in order to
/// recalculate word wrap based on the new size.
/// Doing this could be time consuming for very large files.
bool cpTextBox::SetSize(float width, float height)
{
	Width = width;
	Height = height;
	viewableLines = int((Height-5)/(fontSize+2));
	LoadFile(Filename);
	CreateRects(Label.GetText());
	scrollbar->SetSize(Height, viewableLines);
	scrollbar->SetPosition(PosX+Width, PosY);
	return true;
}

/// Sets the coordinates for the text box's position.
void cpTextBox::SetPosition(float posx, float posy)
{
	cpObject::SetPosition(posx, posy);
	scrollbar->SetPosition(PosX+Width, PosY);
	CreateRects(Label.GetText());
}

/// Loads a new font for the text.
/// BEWARE: the current file will be reloaded in order to update
/// word wrap for the new font size. This could be time consuming
/// for very large files.
void cpTextBox::SetFont(std::string filename, unsigned int size)
{
	cpObject::SetFont(filename, size);
	fontSize = (unsigned int)Label.GetSize();
	viewableLines = int((Height-5)/(fontSize+2));
	LoadFile(Filename);
	scrollbar->Repopulate(viewableLines, totalLines);
}

/// Sets the background color of the text box.
void cpTextBox::SetBackgroundColor(sf::Color color)
{
	cpObject::SetBackgroundColor(color);
	CreateRects(Label.GetText());
}

/// Sets the font style of the text: Bold, Italic, Underlined, Regular.
/// This uses SFML's font styles.
/// BEWARE: The current file will be reloaded in order to update
/// word wrap in case bolding your print changes the width too much.
void cpTextBox::SetFontStyle(unsigned long TextStyle)
{
	cpObject::SetFontStyle(TextStyle);
	LoadFile(Filename);
	scrollbar->Repopulate(viewableLines, totalLines);
}

/// Sets the font size of the text. The current font is
/// reloaded with the new size for better quality.
/// BEWARE: the current file will be reloaded in order to update
/// word wrap for the new font size. This could be time consuming
/// for very large files.
void cpTextBox::SetFontSize(unsigned int size)
{
	cpObject::SetFontSize(size);
	fontSize = (unsigned int)Label.GetSize();
	viewableLines = int((Height-5)/(fontSize+2));
	LoadFile(Filename);
	scrollbar->Repopulate(viewableLines, totalLines);
}

/// Sets the color of your text.
void cpTextBox::SetLabelColor(sf::Color color)
{
	cpObject::SetLabelColor(color);
	if(lineSFStrings.size() > 0)
	{
		for(unsigned int t=0; t < lineSFStrings.size(); t++)
			lineSFStrings[t].SetColor(color);
	}
}
