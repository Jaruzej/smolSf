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

	sf::Uint8* t = new sf::Uint8[4 * w2.size().x*w2.size().y];
	std::fill_n(t, 4 * w2.size().x*w2.size().y, 255);
	for (size_t i = 0; i < w2.size().x*w2.size().y; i++)
		t[4 * i + 1] = 150;
	while (smolSf::all_isOpen()) {
		//smolSf::all_display();
		sf::Event event;
		while (smolSf::all_pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				smolSf::all_close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			w2.show(t);

	}
	std::cout << "Hello World!\n";
	std::cout << smolSf::smol_window::count;
}
