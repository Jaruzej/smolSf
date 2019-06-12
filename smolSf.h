#pragma once

#include"pch.h"
#include<SFML/Graphics.hpp>
#include<iostream>
#include<algorithm>
#include<array>
#include<sstream>
#include<functional>

namespace smolSf {

	static size_t character_size = 24;
	constexpr auto string_for_one_line_size = "fox";
	constexpr size_t padding_between_lines = 10;

	constexpr size_t title_size = 38;
	constexpr size_t taskbar_size = 50;

	const size_t desktop_width = sf::VideoMode::getDesktopMode().width;
	const size_t desktop_height = sf::VideoMode::getDesktopMode().height;

	//Keyboard state
	std::array<bool, sf::Keyboard::KeyCount> isKeyPressed	= { false };
	std::array<bool, sf::Keyboard::KeyCount> isKeyDown		= { false };
	std::array<bool, sf::Keyboard::KeyCount> isKeyUp		= { false };
	
	bool any_key_pressed();
	bool any_key_down();

	//Mouse state
	std::array<bool, sf::Mouse::ButtonCount> isMouseButtonPressed	= { false };
	std::array<bool, sf::Mouse::ButtonCount> isMouseButtonDown		= { false };
	std::array<bool, sf::Mouse::ButtonCount> isMouseButtonUp		= { false };
	
	sf::Vector2i mouse_position;
	sf::Vector2i mouse_delta;
	
	void set_mouse_position(sf::Vector2i&);

	constexpr auto endl = "\n";

	namespace {
		bool isKeyPressedLast[sf::Keyboard::KeyCount] = { false };
		sf::Vector2i last_mouse_position;
		std::vector<std::pair<sf::Keyboard::Key, std::function<void()>>> on_key_down_functions;
		std::vector<std::pair<sf::Mouse::Button, std::function<void()>>> on_button_down_functions;

		std::vector<std::pair<sf::Keyboard::Key, std::function<void()>>> on_key_up_functions;
		std::vector<std::pair<sf::Mouse::Button, std::function<void()>>> on_button_up_functions;
	}

	void poll_keyboard();
	void poll_mouse();	


	void add_on_key_down(sf::Keyboard::Key, std::function<void() > );
	void add_on_button_down(sf::Mouse::Button, std::function<void() >);

	void add_on_key_up(sf::Keyboard::Key, std::function<void() >);
	void add_on_button_up(sf::Mouse::Button, std::function<void() >);

	class smol_window {
		static inline std::vector<smol_window*> all_windows;
		sf::Vector2f current_text_pos;

	public:
		static inline size_t count = 0;
		sf::RenderWindow window;
		sf::Texture screen;
		sf::Font font;
	public:
		smol_window(size_t size_x, size_t size_y, std::string name);
		template<typename T>
		smol_window(T sub_x, T sub_y, std::initializer_list<T> x_pos, std::initializer_list<T> y_pos);
		template<typename T>
		smol_window(T sub_x, T sub_y, T x_pos, T y_pos);

		~smol_window();

		template<class T>
		void draw(T* pixel_data);
		template<class T>
		void draw(T* pixel_data, size_t x, size_t y, size_t x_pos, size_t y_pos);
		template<class T>
		void draw(std::shared_ptr<T> pixel_data);
		void show(sf::Uint8* pixel_data);
		void show(sf::Uint8* pixel_data, size_t x, size_t y, size_t x_pos, size_t y_pos);

		bool isOpen();
		void clear();
		void close();
		void display();
		bool pollEvent(sf::Event&);

		friend bool all_isOpen();
		friend bool any_isOpen();
		friend void all_display();
		friend void all_close();
		friend void all_clear();

		friend bool all_pollEvent(sf::Event&);

		template<typename T>
		friend smol_window& operator<<(smol_window&, const T&);

		sf::Vector2u size();
	};


	class smol_helper{
	public:
		smol_helper();
		~smol_helper();
	};


	template<typename T>
	std::string convert_T_to_str(const T& s);
	size_t count_newlines(std::string s);

}


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

template<typename T>
T get(const std::initializer_list<T>& l, const size_t n) {
	return *(l.begin() + n);
}

template<typename T>
smolSf::smol_window::smol_window(T sub_x, T sub_y, std::initializer_list<T> x_pos, std::initializer_list<T> y_pos) {
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
	window.create(sf::VideoMode(window_size.x, window_size.y - title_size), "ok");
	all_windows.push_back(this);
	count++;
	screen.create(window_size.x, window_size.y - title_size);
	window.setPosition(sf::Vector2i(get(square_size, 0) * get(x_pos, 0), (get(square_size, 1)) * get(y_pos, 0)));
}

template<typename T>
smolSf::smol_window::smol_window(T sub_x, T sub_y, T x_pos, T y_pos) : smol_window(sub_x, sub_y, { x_pos }, { y_pos }) {}

smolSf::smol_window::smol_window(size_t size_x, size_t size_y, std::string name) {
	window.create(sf::VideoMode(size_x, size_y), std::move(name));
	all_windows.push_back(this);
	count++;
	screen.create(size_x, size_y);
};


template<class T>
void smolSf::smol_window::draw(std::shared_ptr<T> pixel_data) {
	draw(pixel_data.get());
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
void smolSf::smol_window::clear() {
	current_text_pos = {};
	window.clear();
}
bool smolSf::smol_window::isOpen() {
	return window.isOpen();
}
void smolSf::smol_window::close() {
	window.close();
}

bool smolSf::smol_window::pollEvent(sf::Event& e) {
	return window.pollEvent(e);
}
sf::Vector2u smolSf::smol_window::size() {
	return window.getSize();
}

smolSf::smol_window::~smol_window() { 
	all_windows.erase(std::remove(all_windows.begin(), all_windows.end(), this),all_windows.end());
	count--; 
}


///////////////////////////////////////////////////////////////////
//All all_ functions are equivelent to calling given function on all constructed smolsf windows.
//////////////////////////////////////////////////////////////////

void smolSf::all_display() {
	for (auto win : smolSf::smol_window::all_windows)
		win->display();
}

bool smolSf::any_isOpen() {
	bool temp = false;
	for (auto win : smolSf::smol_window::all_windows)
		temp = temp || win->isOpen();
	return temp;
}


bool smolSf::all_isOpen() {
	bool temp = true;
	for (auto win : smolSf::smol_window::all_windows)
		temp = temp && win->isOpen();
	return temp;
}

bool smolSf::all_pollEvent(sf::Event& e) {
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

////////////////////////////////////////////////////////
smolSf::smol_helper::smol_helper() {
	smolSf::all_clear();
	smolSf::poll_keyboard();
	smolSf::poll_mouse();
}
smolSf::smol_helper::~smol_helper() {
	smolSf::all_display();
}

void smolSf::poll_keyboard() {
	for (size_t key = sf::Keyboard::A; key < sf::Keyboard::KeyCount; ++key) {
		smolSf::isKeyPressed[key] =  sf::Keyboard::isKeyPressed((sf::Keyboard::Key) key);
		smolSf::isKeyDown	[key] =  smolSf::isKeyPressed[key] && !smolSf::isKeyPressedLast[key];
		smolSf::isKeyUp		[key] = !smolSf::isKeyPressed[key] &&  smolSf::isKeyPressedLast[key];
		smolSf::isKeyPressedLast[key] = smolSf::isKeyPressed[key];
	}
	for (auto [key, f] : smolSf::on_key_down_functions) {
		if (smolSf::isKeyDown[key])
			f();
	}
	for (auto [key, f] : smolSf::on_key_up_functions) {
		if (smolSf::isKeyUp[key])
			f();
	}
}

void smolSf::poll_mouse() {
	for (size_t button = sf::Mouse::Left; button < sf::Mouse::ButtonCount; ++button) {
		
		smolSf::mouse_position = sf::Mouse::getPosition();
		smolSf::mouse_delta = smolSf::mouse_position - smolSf::last_mouse_position;
		smolSf::last_mouse_position = smolSf::mouse_position;

		smolSf::isKeyPressed[button] =  sf::Mouse::isButtonPressed((sf::Mouse::Button) button);
		smolSf::isKeyDown	[button] =  smolSf::isKeyPressed[button] && !smolSf::isKeyPressedLast[button];
		smolSf::isKeyUp		[button] = !smolSf::isKeyPressed[button] &&  smolSf::isKeyPressedLast[button];
		smolSf::isKeyPressedLast[button] = smolSf::isKeyPressed[button];
	}
	for (auto [button, f] : smolSf::on_button_down_functions) {
		if (smolSf::isMouseButtonDown[button])
			f();
	}
	for (auto [button, f] : smolSf::on_button_down_functions) {
		if (smolSf::isMouseButtonUp[button])
			f();
	}
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
smolSf::smol_window& smolSf::operator<<(smolSf::smol_window& win,  const T& s) {

	std::string converted = convert_T_to_str(s);

	size_t newline_count = count_newlines(converted);

	sf::Text t(converted,win.font, character_size);
	t.setPosition(win.current_text_pos);
	
	win.window.draw(t);

	sf::Text t2(smolSf::string_for_one_line_size, win.font, character_size);
	
	win.current_text_pos += sf::Vector2f(t.getGlobalBounds().width, (t2.getGlobalBounds().height+smolSf::padding_between_lines)*newline_count);

	if (newline_count > 0 && *(--converted.end()) == '\n')
		win.current_text_pos.x = 0;

	return win;
}


void smolSf::add_on_key_down(sf::Keyboard::Key key, std::function<void()> f ) {
	smolSf::on_key_down_functions.push_back({ key,f });
}


void smolSf::add_on_button_down(sf::Mouse::Button button, std::function<void()> f) {
	smolSf::on_button_down_functions.push_back({ button,f });
}

void smolSf::add_on_key_up(sf::Keyboard::Key key, std::function<void() > f) {
	smolSf::on_key_up_functions.push_back({ key,f });
}
void smolSf::add_on_button_up(sf::Mouse::Button button, std::function<void() > f) {
	smolSf::on_button_up_functions.push_back({ button,f });
}