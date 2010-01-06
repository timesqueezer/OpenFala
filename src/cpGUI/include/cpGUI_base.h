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



#ifndef CPGUI_BASE
#define CPGUI_BASE

#include <SFML/Graphics.hpp>
#include <string.h>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <math.h>
#include <stdexcept>

namespace cp
{
	enum
	{
		CP_DEFAULT = -1,
		CP_ST_MOUSE_ENTER,
		CP_ST_MOUSE_EXIT,
		CP_ST_MOUSE_IN,
		CP_ST_MOUSE_LBUTTON_DOWN,
		CP_ST_MOUSE_LBUTTON_RELEASED,
		CP_ST_CHANGED,
		CP_ST_NONE,
		CP_TXT_RIGHT_ALIGN,
		CP_TXT_LEFT_ALIGN,
		CP_TXT_CENTER_ALIGN
	};

	class cpObject;

	struct FontData
	{
		sf::Font *myFont;
		short timesUsed;
	};

	class cpGuiContainer
	{
	public:
		cpGuiContainer();
		~cpGuiContainer();
		void Register(cpObject* object);
		void Unregister(cpObject* object);
		void SetObjectFocus(cpObject* object, bool focused);
		void NextObjectFocus();
		void AcceptsKeys(bool accept, bool UseArrowKeys);
		void ProcessKeys(sf::Event *evt);
		void Show(bool show);
		bool GetShow();
		void ReleaseUnusedFonts();
		sf::Font* GetFont(std::string filename, unsigned int fontSize=20);

		std::vector<cpObject*> control;
		std::string defaultFilename;
		bool acceptsKeys, useArrowKeys, bShow;
		sf::Clock clock;
		
	private:
		typedef std::map<std::string, FontData> FontMap;
		std::map<std::string, FontData> fontMap;
		short rightleft;
		sf::Font *defaultFont;
	};

	class cpObject
	{
	public:
		cpObject(sf::RenderWindow *parent, cpGuiContainer *GUI, std::string label,
			float posx=0, float posy=0, float width=CP_DEFAULT, float height=CP_DEFAULT);
		virtual bool SetSize(float width, float height);
		virtual void SetPosition(float posx, float posy);
		virtual void SetFont(std::string filename, unsigned int size);
		virtual void SetBackgroundColor(sf::Color color);
		virtual void SetFontStyle(unsigned long TextStyle);
		virtual void SetFontSize(unsigned int size);
		virtual void SetLabelColor(sf::Color color);
		virtual void SetLabelText(std::string text);
		virtual int CheckState(const sf::Input *input);
		virtual void Show(bool show);
		void SetFocus(bool focus);
		void SetMouseoverColor(sf::Color color);
		bool GetShow();
		sf::Vector2f GetSize();
		sf::Vector2f GetPosition();
		sf::Color GetBackgroundColor();
		sf::Color GetMouseoverColor();
		unsigned long GetFontStyle();
		sf::Color GetLabelColor();
		std::string GetLabelText();

		bool hasFocus, useWheel;
		int wheelDelta, keyCode;
		std::string Font;
		sf::Font *textFont;

	protected:
		virtual void CreateRects(std::string label);
		bool CheckMouseEntered(const sf::Input *input);
		void DrawDashedLine(sf::Vector2f start, sf::Vector2f end);
		void DrawDashedRect(float posx, float posy, float width, float height);
		void SetUseWheel(bool use);

		float Width, Height, PosX, PosY, labelX, labelY;
		sf::RenderWindow* Parent;
		sf::Color backgroundColor, backgroundColorTemp, textColor, outlineColor;
		sf::Color outlineColorTemp, mouseoverColor;
		sf::String Label, tempLabel;
		sf::Shape backRect;
		bool bDefaultSize, mouseIn, lbuttonDown, returnDown, bShow;
		cpGuiContainer *gui;
	};


	class cpButton : public cpObject
	{
	public:
		cpButton();
		cpButton(sf::RenderWindow* parent, cpGuiContainer *GUI, std::string label, 
			float posx=0, float posy=0, float width=CP_DEFAULT, float height=CP_DEFAULT);
		virtual void Draw();
		virtual bool SetSize(float width, float height);
		void SetPosition(float posx, float posy);
		void SetFont(std::string filename, unsigned int size);
		void SetBackgroundColor(sf::Color color);
		void SetFontStyle(unsigned long TextStyle);
		void SetFontSize(unsigned int size);
		void SetLabelColor(sf::Color color);
		void SetLabelText(std::string text);
		void Show(bool show);
		virtual int CheckState(const sf::Input *input);

	protected:
		virtual void CreateRects(std::string label);
		void TestLabelFit();

		sf::Color backgroundColor2;
		sf::Color backgroundColor2Temp;

		sf::Shape gradient;
		bool labelInside;
	};


	class cpScrollBar
	{
	public:
		cpScrollBar();
		cpScrollBar(sf::RenderWindow *parent, cpGuiContainer *GUI, float length,
			int ViewableLines, int TotalLines, float posx, float posy);
		~cpScrollBar();
		void Draw();
		int CheckState(const sf::Input *input);
		void MoveSlider(short direction);
		void Repopulate(int ViewableLines, int TotalLines);
		void SetPosition(float posx, float posy);
		void SetSize(float length, int ViewableLines);

	private:
		void MoveLines();
		void CreateRects();
		void CheckSliderValid();
		bool CheckMouseEntered(const sf::Input *input);
		int CheckMouseSlider(const sf::Input *input);

		sf::RenderWindow *Parent;
		sf::Shape groove, sliderShape, upArrow, downArrow, line1, line2, line3;
		sf::Clock clock, clock2;
		sf::Color backgroundColor, backgroundColor2;
		cpGuiContainer *gui;
		cpButton *upButton, *downButton;
		float Length, PosX, PosY, lineInc, currentPosition, slLength, mouseY;
		int viewableLines, totalLines;
		bool mouseDown, clickGroove, sliderDrag, slDown;
	};
}

#endif
