#pragma once

#include"pch.h"
#include<SFML/Graphics.hpp>
#include<iostream>
#include<algorithm>
#include<array>
#include<sstream>
#include<functional>

/*
smolSf provides a small wrapper around SFML 2.5.1 mainly aimed at personal raymarching/spherer tracing project.

smolSf uses SFML RenderWindow to interact with Windows windows, which means that alot of functions such as "clear",
"getSize" work the same as in SFML; though some of them are missing, as I felt they were unnecessary.

In addition smolSf adds extended keyboard and mouse support as well as a way to print text to a window using stream semantics.
Thera are also all_* functions that call given function for all contructed windows.

smol_helper is used to automate the process of polling inputs, clearing screen (when its constructed) and displaying to all windows (when destructed).
So that only calling draw function is necessary.

More information and examples at https://github.com/Jaruzej/smolSf

These .libs are necessary for smolSf: sfml-graphics.lib,sfml-window.lib,sfml-system-d.lib.
*/



namespace smolSf {

	constexpr auto default_window_title = "smol window";

	//Easiest way of finding a size of line of characters was to just create one then get its size.
	constexpr auto string_for_one_line_size = "fox";
	
	//Pixel size used for streaming text to window.
	size_t character_size = 24;
	size_t padding_between_lines = 10;

	//Standard Windows title size and standard Windows taskbar in pixels, this is not DPI aware(to-do).
	size_t title_size = 38;
	size_t taskbar_size = 50;

	const size_t desktop_width = sf::VideoMode::getDesktopMode().width;
	const size_t desktop_height = sf::VideoMode::getDesktopMode().height;

	//Keyboard states that are updated when pollKeyboard is called.
	std::array<bool, sf::Keyboard::KeyCount> isKeyPressed	= { false };
	std::array<bool, sf::Keyboard::KeyCount> isKeyDown		= { false };
	std::array<bool, sf::Keyboard::KeyCount> isKeyUp		= { false };
	
	[[nodiscard]] bool any_key_pressed();
	[[nodiscard]] bool any_key_down();
	[[nodiscard]] bool any_key_up();


	//Mouse states that are updated when pollMouse is called.
	std::array<bool, sf::Mouse::ButtonCount> isMouseButtonPressed	= { false };
	std::array<bool, sf::Mouse::ButtonCount> isMouseButtonDown		= { false };
	std::array<bool, sf::Mouse::ButtonCount> isMouseButtonUp		= { false };
	
	sf::Vector2i mouse_position;
	//mouse_delta is a diffrence between position at current pollMouse and last pollMouse.
	sf::Vector2i mouse_delta;
	
	void set_mouse_position(sf::Vector2i&);

	//Alternative to writing '\n' when using streaming to window.
	constexpr auto endl = '\n';

	//All members of impl namespace shouldn't be touched, they are supposed to be only used by other public functions.
	namespace impl {
		bool isKeyPressedLast[sf::Keyboard::KeyCount] = { false };
		std::array<bool, sf::Mouse::ButtonCount> isMouseButtonPressedLast = { false };
		sf::Vector2i last_mouse_position;

		std::vector<std::pair<sf::Keyboard::Key, std::function<void()>>> on_key_pressed_functions;
		std::vector<std::pair<sf::Mouse::Button, std::function<void()>>> on_button_pressed_functions;

		std::vector<std::pair<sf::Keyboard::Key, std::function<void()>>> on_key_down_functions;
		std::vector<std::pair<sf::Mouse::Button, std::function<void()>>> on_button_down_functions;

		std::vector<std::pair<sf::Keyboard::Key, std::function<void()>>> on_key_up_functions;
		std::vector<std::pair<sf::Mouse::Button, std::function<void()>>> on_button_up_functions;
	}

	void poll_keyboard();
	void poll_mouse();	


	//All on_key functions take in a key and a lambda that will be called during the next pollKeyboard or pollMouse call.
	//Functions will be called in order 'down','up','pressed'.
	void add_on_key_pressed(sf::Keyboard::Key, std::function<void() >);
	void add_on_button_pressed(sf::Mouse::Button, std::function<void() >);

	void add_on_key_down(sf::Keyboard::Key, std::function<void() > );
	void add_on_button_down(sf::Mouse::Button, std::function<void() >);

	void add_on_key_up(sf::Keyboard::Key, std::function<void() >);
	void add_on_button_up(sf::Mouse::Button, std::function<void() >);


	class smol_window {
	private:
		static inline std::vector<smol_window*> all_windows;
		sf::Vector2f current_text_pos;
		sf::RenderWindow window;
		sf::Texture screen;
		
	public:
		sf::Font font;

		smol_window(size_t size_x, size_t size_y, std::string name = smolSf::default_window_title);
		smol_window(size_t size_x, size_t size_y,size_t pos_x,size_t pos_y, std::string name = smolSf::default_window_title);
		
		/*These constructors use convention that is inspired by matlab subplot.
		  sub_x,sub_y determine in how many cells screen will be split, while x_pos,y_pos determine which of these cells will be taken.
		  Cells should be in range [0,sub_x) and [0,sub_y).
		  Using x_pos >= sub_x and y_pos >= sub_y might lead to errors. 
		*/ 
		smol_window(size_t sub_x, size_t sub_y, std::initializer_list<size_t> x_pos, std::initializer_list<size_t> y_pos, std::string name = smolSf::default_window_title);
		smol_window(size_t sub_x, size_t sub_y, size_t x_pos, size_t y_pos, std::string name = smolSf::default_window_title);
		smol_window(size_t sub_x, size_t sub_y, std::initializer_list<size_t> x_pos, size_t y_pos, std::string name = smolSf::default_window_title);
		smol_window(size_t sub_x, size_t sub_y, size_t x_pos, std::initializer_list<size_t> y_pos, std::string name = smolSf::default_window_title);

		~smol_window();

		//Draw takes in any pointer to a type that is convertible to sf::Uint8. pixel_data is converted to a sprite which is drawn to the window.
		template<class T>
		void draw(T* pixel_data);
		template<class T>
		void draw(T* pixel_data, size_t x, size_t y, size_t x_pos, size_t y_pos);


		//Calles clear,draw,display.
		void show(sf::Uint8* pixel_data);
		void show(sf::Uint8* pixel_data, size_t x, size_t y, size_t x_pos, size_t y_pos);

		//These functions are the same as their SFML counterparts.
		void setFramerateRate(size_t);
		[[nodiscard]] bool isOpen();
		void clear(sf::Color = sf::Color::Black);
		void close();
		void display();
		[[nodiscard]] bool pollEvent(sf::Event&);

		//sf::RenderWindow::capture is deprecated, this capture creates sf::Texture that is updated 
		//with content of this window then converted to sf::Image
		[[nodiscard]] sf::Image capture();


		//Calles given functions on all constructed windows.
		friend bool all_isOpen();
		friend void all_display();
		friend void all_close();
		friend void all_clear();
		friend void all_clear(sf::Color);
		friend bool all_pollEvent(sf::Event&);

		//Checks if any constructed window is open.
		friend bool any_isOpen();
		
		//This operator is used to write to a window as if it was a console.
		//Any type that is has a overloaded << operator to a ostream will work with this function.
		template<typename T>
		friend smol_window& operator<<(smol_window&, const T&);

		[[nodiscard]] sf::Vector2u getSize();
	};

	
	class smol_helper{
	public:
		smol_helper();
		~smol_helper();
	};

	//Helper function needed for the operator <<.
	template<typename T>
	[[nodiscard]] std::string convert_T_to_str(const T& s);
	[[nodiscard]] size_t count_newlines(std::string s);

}

///////////////////////////////////////////////////////////////////
//DRAW
//////////////////////////////////////////////////////////////////


template<class T>
void smolSf::smol_window::draw(T* pixel_data) {
	static_assert(std::is_convertible_v<T, sf::Uint8>, "cannot convert type to sf::Uint8");
	screen.update(pixel_data);
	sf::Sprite s(screen);
	window.draw(s);
}
template<class T>
void smolSf::smol_window::draw(T* pixel_data, size_t x, size_t y, size_t x_pos, size_t y_pos) {
	static_assert(std::is_convertible_v<T, sf::Uint8>, "cannot convert type to sf::Uint8");
	screen.update(pixel_data,x_pos,y_pos,x,y);
	sf::Sprite s(screen);
	window.draw(s);
}

///////////////////////////////////////////////////////////////////
//CONSTRUCTORS
//////////////////////////////////////////////////////////////////

//get function is implamented since initializer_list doesn't have any generic accesor.
template<typename T>
[[nodiscard]] T get(const std::initializer_list<T>& l, const size_t n) {
	return *(l.begin() + n);
}


smolSf::smol_window::smol_window(size_t sub_x, size_t sub_y, std::initializer_list<size_t> x_pos, std::initializer_list<size_t> y_pos, std::string name) {
	auto square_size = { desktop_width / sub_x,(desktop_height - taskbar_size) / sub_y };
	sf::Vector2u window_size;
	if (x_pos.size() == 2)
		window_size.x = (get(x_pos, 1) - get(x_pos, 0) + 1)*get(square_size, 0);
	if (x_pos.size() == 1)
		window_size.x = get(square_size, 0);
	if (y_pos.size() == 2)
		window_size.y = (get(y_pos, 1) - get(y_pos, 0) + 1)*get(square_size, 1);
	if (y_pos.size() == 1)
		window_size.y = get(square_size, 1);
	window.create(sf::VideoMode(window_size.x, window_size.y - title_size), name );
	all_windows.push_back(this);
	screen.create(window_size.x, window_size.y - title_size);
	window.setPosition(sf::Vector2i(get(square_size, 0) * get(x_pos, 0), (get(square_size, 1)) * get(y_pos, 0)));
}

smolSf::smol_window::smol_window(size_t sub_x, size_t sub_y, size_t x_pos, size_t y_pos, std::string name) : smol_window(sub_x, sub_y, { x_pos }, { y_pos }, name) {}
smolSf::smol_window::smol_window(size_t sub_x, size_t sub_y, size_t x_pos, std::initializer_list<size_t> y_pos, std::string name) : smol_window(sub_x, sub_y, { x_pos }, y_pos, name) {}
smolSf::smol_window::smol_window(size_t sub_x, size_t sub_y, std::initializer_list<size_t> x_pos, size_t y_pos, std::string name) : smol_window(sub_x, sub_y, x_pos, { y_pos }, name) {}

smolSf::smol_window::smol_window(size_t size_x, size_t size_y, std::string name) {
	window.create(sf::VideoMode(size_x, size_y), std::move(name));
	all_windows.push_back(this);
	screen.create(size_x, size_y);
};

///////////////////////////////////////////////////////////////////
//UTILITIES
//////////////////////////////////////////////////////////////////

void smolSf::smol_window::setFramerateRate(size_t limit) {
	window.setFramerateLimit(limit);
}
void smolSf::smol_window::show(sf::Uint8* pixel_data) {
	window.clear();
	draw(pixel_data);
	display();
}
void smolSf::smol_window::show(sf::Uint8* pixel_data, size_t x, size_t y, size_t x_pos, size_t y_pos) {
	window.clear();
	draw(pixel_data,x,y,x_pos,y_pos);
	display();
}

void smolSf::smol_window::display() {
	window.display();
}
sf::Image smolSf::smol_window::capture() {
	sf::Texture temp;
	temp.update(window);
	return temp.copyToImage();
}

void smolSf::smol_window::clear(sf::Color col) {
	current_text_pos = {};
	window.clear(col);
}
[[nodiscard]] bool smolSf::smol_window::isOpen() {
	return window.isOpen();
}
void smolSf::smol_window::close() {
	window.close();
}

bool smolSf::smol_window::pollEvent(sf::Event& e) {
	return window.pollEvent(e);
}
[[nodiscard]] sf::Vector2u smolSf::smol_window::getSize() {
	return window.getSize();
}

smolSf::smol_window::~smol_window() { 
	all_windows.erase(std::remove(all_windows.begin(), all_windows.end(), this),all_windows.end());
}


///////////////////////////////////////////////////////////////////
//ALL_ FUNCTIONS
//////////////////////////////////////////////////////////////////

void smolSf::all_display() {
	for (auto win : smolSf::smol_window::all_windows)
		win->display();
}

[[nodiscard]] bool smolSf::any_isOpen() {
	bool temp = false;
	for (auto win : smolSf::smol_window::all_windows)
		temp = temp || win->isOpen();
	return temp;
}


[[nodiscard]] bool smolSf::all_isOpen() {
	bool temp = true;
	for (auto win : smolSf::smol_window::all_windows)
		temp = temp && win->isOpen();
	return temp;
}

[[nodiscard]] bool smolSf::all_pollEvent(sf::Event& e) {
	for (auto win : smolSf::smol_window::all_windows) {
		if (win->pollEvent(e))
			return true;
	}
	return false;
}

void smolSf::all_close() {
	for (auto win : smolSf::smol_window::all_windows) {
		win->close();
	}
}
void smolSf::all_clear() {
	for (auto win : smolSf::smol_window::all_windows) {
		win->clear();
	}
}
void smolSf::all_clear(sf::Color col) {
	for (auto win : smolSf::smol_window::all_windows) {
		win->clear(col);
	}
}

///////////////////////////////////////////////////////////////////
//SMOL_HELPER
//////////////////////////////////////////////////////////////////
smolSf::smol_helper::smol_helper() {
	smolSf::all_clear();
	smolSf::poll_keyboard();
	smolSf::poll_mouse();
}
smolSf::smol_helper::~smol_helper() {
	smolSf::all_display();
}

///////////////////////////////////////////////////////////////////
//INPUT
//////////////////////////////////////////////////////////////////

void smolSf::poll_keyboard() {
	for (size_t key = sf::Keyboard::A; key < sf::Keyboard::KeyCount; ++key) {
		smolSf::isKeyPressed[key] =  sf::Keyboard::isKeyPressed((sf::Keyboard::Key) key);
		smolSf::isKeyDown	[key] =  smolSf::isKeyPressed[key] && !smolSf::impl::isKeyPressedLast[key];
		smolSf::isKeyUp		[key] = !smolSf::isKeyPressed[key] &&  smolSf::impl::isKeyPressedLast[key];
		smolSf::impl::isKeyPressedLast[key] = smolSf::isKeyPressed[key];
	}
	for (auto [key, f] : smolSf::impl::on_key_down_functions) 
		if (smolSf::isKeyDown[key])
			f();
	for (auto [key, f] : smolSf::impl::on_key_up_functions) 
		if (smolSf::isKeyUp[key])
			f();
	for (auto[key, f] : smolSf::impl::on_key_pressed_functions)
		if (smolSf::isKeyPressed[key])
			f();
}

void smolSf::poll_mouse() {
	for (size_t button = sf::Mouse::Left; button < sf::Mouse::ButtonCount; ++button) {
		
		smolSf::mouse_position = sf::Mouse::getPosition();
		smolSf::mouse_delta = smolSf::mouse_position - smolSf::impl::last_mouse_position;
		smolSf::impl::last_mouse_position = smolSf::mouse_position;

		smolSf::isMouseButtonPressed[button] =  sf::Mouse::isButtonPressed((sf::Mouse::Button) button);
		smolSf::isMouseButtonDown	[button] =  smolSf::isMouseButtonPressed[button] && !smolSf::impl::isMouseButtonPressedLast[button];
		smolSf::isMouseButtonUp		[button] = !smolSf::isMouseButtonPressed[button] &&  smolSf::impl::isMouseButtonPressedLast[button];
		smolSf::impl::isMouseButtonPressedLast[button] = smolSf::isMouseButtonPressed[button];
	}
	for (auto [button, f] : smolSf::impl::on_button_down_functions) 
		if (smolSf::isMouseButtonDown[button])
			f();
	for (auto [button, f] : smolSf::impl::on_button_up_functions)
		if (smolSf::isMouseButtonUp[button])
			f();
	for (auto[button, f] : smolSf::impl::on_button_pressed_functions)
		if (smolSf::isMouseButtonPressed[button])
			f();
}

void smolSf::set_mouse_position(sf::Vector2i& v) {
	sf::Mouse::setPosition(v);
}

bool smolSf::any_key_pressed() {
	for (const auto key : smolSf::isKeyPressed) {
		if (key)
			return true;
	}
	return false;
}
bool smolSf::any_key_down() {
	for (const auto key : smolSf::isKeyDown) {
		if (key)
			return true;
	}
	return false;
}
bool smolSf::any_key_up() {
	for (const auto key : smolSf::isKeyUp) {
		if (key)
			return true;
	}
	return false;
}

void smolSf::add_on_key_pressed(sf::Keyboard::Key key, std::function<void()> f) {
	smolSf::impl::on_key_pressed_functions.push_back({ key,f });
}

void smolSf::add_on_button_pressed(sf::Mouse::Button button, std::function<void()> f) {
	smolSf::impl::on_button_pressed_functions.push_back({ button,f });
}

void smolSf::add_on_key_down(sf::Keyboard::Key key, std::function<void()> f ) {
	smolSf::impl::on_key_down_functions.push_back({ key,f });
}


void smolSf::add_on_button_down(sf::Mouse::Button button, std::function<void()> f) {
	smolSf::impl::on_button_down_functions.push_back({ button,f });
}

void smolSf::add_on_key_up(sf::Keyboard::Key key, std::function<void() > f) {
	smolSf::impl::on_key_up_functions.push_back({ key,f });
}
void smolSf::add_on_button_up(sf::Mouse::Button button, std::function<void() > f) {
	smolSf::impl::on_button_up_functions.push_back({ button,f });
}

///////////////////////////////////////////////////////////////////
//CONSOLE SIMULATION
//////////////////////////////////////////////////////////////////


template<typename T>
std::string smolSf::convert_T_to_str(const T& s) {
	std::stringstream ss;
	ss << s;
	return ss.str();
}

size_t smolSf::count_newlines(std::string s) {
	size_t newline_count = 0;

	for (auto c : s)
		if (c == '\n')
			newline_count++;

	return newline_count;
}



template<typename T>
smolSf::smol_window& smolSf::operator<<(smolSf::smol_window& win, const T& s) {

	std::string converted = convert_T_to_str(s);
	
	if (converted.size() == 0)
		return win;

	if (*converted.begin() == '\n')
		win.current_text_pos.x = 0;

	size_t newline_count = count_newlines(converted);


	sf::Text t(converted, win.font, character_size);
	t.setPosition(win.current_text_pos);

	win.window.draw(t);

	sf::Text t2(smolSf::string_for_one_line_size, win.font, character_size);

	win.current_text_pos += sf::Vector2f(t.getGlobalBounds().width, (t2.getGlobalBounds().height + smolSf::padding_between_lines)*newline_count);


	if (*(--converted.end()) == '\n')
		win.current_text_pos.x = 0;

	return win;
}

