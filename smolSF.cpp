// SFML_little_wrapper.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include"smolSf.h"


int main()
{
	smolSf::smol_window w(2, 2, 0, 0);
	smolSf::smol_window w2(2, 2, 0, 1);
	smolSf::smol_window w3(2, 2, 1, 0);


	smolSf::character_size = 30;
	w2.font.loadFromFile("default.ttf");

	int frames = 60;
	
	smolSf::add_on_key_down(sf::Keyboard::W, [&]() { frames += 2 * (rand() % 2) - 1; });
	smolSf::add_on_key_up(sf::Keyboard::W, [&]() { frames += 2 * (rand() % 2) - 1; });

	while (smolSf::all_isOpen()) {
		smolSf::any_isOpen();
		smolSf::smol_helper help;

		sf::Event event;
		while (smolSf::all_pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				smolSf::all_close();
		}
		

		w2 << "Framerate: " << frames << smolSf::endl;
		w2 << "Rays: " << frames * 156 << smolSf::endl;
		
	}
	std::cout << "Hello World!\n";
	std::cout << smolSf::smol_window::count;
}
