#include <iostream>
#include <windows.h>
#include "ScreenBuffer/ScreenBuffer.h"
#include "ScreenBuffer/Color.h"


int main()
{
	//Set screen to 80x80 window, active screen buffer, disable mouse capture and resizing
	ScreenBuffer::SetupBufferScreen(80, 80, 8, 8);

	
	// fill circle at position 40,40 with 10 radius by whitespaces with white color on background
	ScreenBuffer::fillCircle(40, 40, 10, ' ', Color::BG_WHITE);
	// draw a circle 
	ScreenBuffer::drawCircle(40, 40, 8, ' ', Color::BG_BLACK);
	//draw a rectangle
	ScreenBuffer::drawRect(29,29,51,51, ' ', Color::BG_BLUE);
	//fill a rectangle
	ScreenBuffer::fillRect(32, 40, 48, 32, ' ', Color::BG_WHITE);
	//draw a line from 10,50 to 20,10
	ScreenBuffer::drawLine(30, 70, 50, 70, ' ', Color::BG_BLUE);
	//draw a string
	ScreenBuffer::drawString(30, 60, "HELLO SCREEN BUFFER!", Color::FG_MAGENTA);

	//Call when you done drawing to display them to the console
	ScreenBuffer::drawToConsole();

	//Wait...
	std::cin.get();
	// depose when finish using
	ScreenBuffer::deposeBuffer();


}
