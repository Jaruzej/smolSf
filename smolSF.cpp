
#include <iostream>
#include"smolSf.h"


int main()
{
	smolSf::chrono::tic();
	smolSf::smol_window w(2, 2, 0, 0);
	smolSf::smol_window w2(2, 2, { 0,2 }, { 1 });
	smolSf::smol_window w3(2, 2, 1, 0);
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(smolSf::chrono::toc()).count() << "ms\n";

	smolSf::character_size = 16;
	w2.font.loadFromFile("default.ttf");
	
	smolSf::input::add_on_key_up(sf::Keyboard::Escape, [&]() { smolSf::all_close(); });

	while (smolSf::any_isOpen()) {
		smolSf::smol_helper help;
		smolSf::chrono::tic();

		w2 << "Framerate: " << (size_t) (1/smolSf::chrono::toc_as_fseconds());
	}
}
