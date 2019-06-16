// SFML_little_wrapper.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include"smolSf.h"


int main()
{
	smolSf::smol_window w(2, 2, 0, 0);
	smolSf::smol_window w2(2, 2, { 0,2 }, { 1 });
	smolSf::smol_window w3(2, 2, 1, 0);


	smolSf::character_size = 16;
	w2.font.loadFromFile("default.ttf");

	int frames = 60;
	
	smolSf::add_on_key_up(sf::Keyboard::Escape, [&]() { smolSf::all_close(); });

	while (smolSf::all_isOpen()) {
		smolSf::smol_helper help;

		w2 << "Framerate: " << frames;
		w2 << "\nRays: " << frames * 156 << smolSf::endl;
		w2.clear();
	}
	std::cout << "Hello World!\n";
}
