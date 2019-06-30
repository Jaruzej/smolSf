#ifndef SMOLSF_H
#define SMOLSF_H

#include<SFML/Graphics.hpp>
#include<iostream>
#include<algorithm>
#include<array>
#include<sstream>
#include<functional>
#include<chrono>
#include<queue>
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

	class input {
	private:
		static inline bool isKeyPressedLast[sf::Keyboard::KeyCount] = { false };
		static inline std::array<bool, sf::Mouse::ButtonCount> isMouseButtonPressedLast = { false };
		static inline sf::Vector2i last_mouse_position;

		static inline std::vector<std::pair<sf::Keyboard::Key, std::function<void()>>> on_key_pressed_functions;
		static inline std::vector<std::pair<sf::Mouse::Button, std::function<void()>>> on_button_pressed_functions;

		static inline std::vector<std::pair<sf::Keyboard::Key, std::function<void()>>> on_key_down_functions;
		static inline std::vector<std::pair<sf::Mouse::Button, std::function<void()>>> on_button_down_functions;

		static inline std::vector<std::pair<sf::Keyboard::Key, std::function<void()>>> on_key_up_functions;
		static inline std::vector<std::pair<sf::Mouse::Button, std::function<void()>>> on_button_up_functions;
	public:

		static inline std::array<bool, sf::Keyboard::KeyCount> isKeyPressed = { false };
		static inline std::array<bool, sf::Keyboard::KeyCount> isKeyDown = { false };
		static inline std::array<bool, sf::Keyboard::KeyCount> isKeyUp = { false };

		static void poll_keyboard();
		static void poll_mouse();


		[[nodiscard]] static bool any_key_pressed();
		[[nodiscard]] static bool any_key_down();
		[[nodiscard]] static bool any_key_up();


		//Mouse states that are updated when pollMouse is called.
		static inline std::array<bool, sf::Mouse::ButtonCount> isMouseButtonPressed = { false };
		static inline std::array<bool, sf::Mouse::ButtonCount> isMouseButtonDown = { false };
		static inline std::array<bool, sf::Mouse::ButtonCount> isMouseButtonUp = { false };

		static inline sf::Vector2i mouse_position;
		//mouse_delta is a diffrence between position at current pollMouse and last pollMouse.
		static inline sf::Vector2i mouse_delta;

		static void set_mouse_position(sf::Vector2i&);

		//All on_key functions take in a key and a lambda that will be called during the next pollKeyboard or pollMouse call.
		//Functions will be called in order 'down','up','pressed'.
		static void add_on_key_pressed(sf::Keyboard::Key, std::function<void() >);
		static void add_on_button_pressed(sf::Mouse::Button, std::function<void() >);

		static void add_on_key_down(sf::Keyboard::Key, std::function<void() >);
		static void add_on_button_down(sf::Mouse::Button, std::function<void() >);

		static void add_on_key_up(sf::Keyboard::Key, std::function<void() >);
		static void add_on_button_up(sf::Mouse::Button, std::function<void() >);



	};
	//Alternative to writing '\n' when using streaming to window.
	constexpr auto endl = '\n';

	
	class smol_window {
	private:
		static inline std::vector<smol_window*> all_windows;
		sf::Vector2f current_text_pos;
		sf::RenderWindow window;
		sf::Texture screen;
		
	public:
		sf::Font font;

		//Creates window of size {size_x,size_y}.
		smol_window(size_t size_x, size_t size_y, std::string name = smolSf::default_window_title);
		
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
		void setSize(sf::Vector2u);
		void setPosition(sf::Vector2i);
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

	class chrono {
	private:
		static inline std::queue<std::chrono::steady_clock::time_point> tik_points;
	public:
		//tic, toc works like their matlab counterparts, whenever tic is called current time is pushed into queue.
		//toc return duration between last tic and current time.
		//So tic() tic() toc() toc() would return duration between inner tic-toc and then return outer tic-tok.
		static void tic();
		[[nodiscard]] static std::chrono::nanoseconds toc();
		//If you don't want to deal with std::chrono library this toc will return duration as plain number of seconds.
		[[nodiscard]] static float toc_as_fseconds();
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

void smolSf::smol_window::setSize(sf::Vector2u size) {
	window.setSize(std::move(size));
}
void smolSf::smol_window::setPosition(sf::Vector2i pos) {
	window.setPosition(std::move(pos));
}

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
	smolSf::input::poll_keyboard();
	smolSf::input::poll_mouse();
}
smolSf::smol_helper::~smol_helper() {
	smolSf::all_display();
}

///////////////////////////////////////////////////////////////////
//INPUT
//////////////////////////////////////////////////////////////////

void smolSf::input::poll_keyboard() {
	using namespace smolSf;
	for (size_t key = sf::Keyboard::A; key < sf::Keyboard::KeyCount; ++key) {
		input::isKeyPressed [key] = sf::Keyboard::isKeyPressed((sf::Keyboard::Key) key);
		input::isKeyDown	[key] =  input::isKeyPressed[key] && !input::isKeyPressedLast[key];
		input::isKeyUp			[key] = !input::isKeyPressed[key] &&  input::isKeyPressedLast[key];
		input::isKeyPressedLast	[key] = input::isKeyPressed[key];
	}
	for (auto [key, f] : input::on_key_down_functions) 
		if (input::isKeyDown[key])
			f();
	for (auto [key, f] : input::on_key_up_functions) 
		if (input::isKeyUp[key])
			f();
	for (auto[key, f] : input::on_key_pressed_functions)
		if (input::isKeyPressed[key])
			f();
}

void smolSf::input::poll_mouse() {
	for (size_t button = sf::Mouse::Left; button < sf::Mouse::ButtonCount; ++button) {
		
		mouse_position = sf::Mouse::getPosition();
		mouse_delta = mouse_position - last_mouse_position;
		last_mouse_position = mouse_position;

		isMouseButtonPressed[button] =  sf::Mouse::isButtonPressed((sf::Mouse::Button) button);
		isMouseButtonDown	[button] =  isMouseButtonPressed[button] && !isMouseButtonPressedLast[button];
		isMouseButtonUp		[button] = !isMouseButtonPressed[button] &&  isMouseButtonPressedLast[button];
		isMouseButtonPressedLast[button] = isMouseButtonPressed[button];
	}
	for (auto [button, f] : on_button_down_functions) 
		if (isMouseButtonDown[button])
			f();
	for (auto [button, f] : on_button_up_functions)
		if (isMouseButtonUp[button])
			f();
	for (auto[button, f] : on_button_pressed_functions)
		if (isMouseButtonPressed[button])
			f();
}

void smolSf::input::set_mouse_position(sf::Vector2i& v) {
	sf::Mouse::setPosition(v);
}

bool smolSf::input::any_key_pressed() {
	for (const auto key : isKeyPressed) {
		if (key)
			return true;
	}
	return false;
}
bool smolSf::input::any_key_down() {
	for (const auto key : isKeyDown) {
		if (key)
			return true;
	}
	return false;
}
bool smolSf::input::any_key_up() {
	for (const auto key : isKeyUp) {
		if (key)
			return true;
	}
	return false;
}

void smolSf::input::add_on_key_pressed(sf::Keyboard::Key key, std::function<void()> f) {
	on_key_pressed_functions.push_back({ key,f });
}

void smolSf::input::add_on_button_pressed(sf::Mouse::Button button, std::function<void()> f) {
	on_button_pressed_functions.push_back({ button,f });
}

void smolSf::input::add_on_key_down(sf::Keyboard::Key key, std::function<void()> f ) {
	on_key_down_functions.push_back({ key,f });
}


void smolSf::input::add_on_button_down(sf::Mouse::Button button, std::function<void()> f) {
	on_button_down_functions.push_back({ button,f });
}

void smolSf::input::add_on_key_up(sf::Keyboard::Key key, std::function<void() > f) {
	on_key_up_functions.push_back({ key,f });
}
void smolSf::input::add_on_button_up(sf::Mouse::Button button, std::function<void() > f) {
	on_button_up_functions.push_back({ button,f });
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

//Timing


void smolSf::chrono::tic() {
	tik_points.push(std::chrono::steady_clock::now());
}

std::chrono::nanoseconds smolSf::chrono::toc() {
	if (tik_points.empty()) {
		std::cout << "There is no tic associated with this toc.\n";
		return {};
	}
	std::chrono::nanoseconds duration = std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::steady_clock::now()-tik_points.front());
	tik_points.pop();
	return duration;
}

float smolSf::chrono::toc_as_fseconds() {
	return std::chrono::duration_cast<std::chrono::duration<float>>(toc()).count();
}


#endif