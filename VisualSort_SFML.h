#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <iostream>
#include <chrono>
#include <random>
#include <cmath>

class VisualSort_SFML {
public:
	VisualSort_SFML(const int number_of_el, const int delay, const int sort_num, const bool option_sound, QWidget* parent = nullptr);
	VisualSort_SFML();
	~VisualSort_SFML();
private:
	QWidget* parent;
	sf::RenderWindow window;
	sf::Font font;
	const std::string path_to_font = "media/font/";
	const std::string font_file = "Roboto-Regular.ttf";
	sf::Text text;
	const int number_of_el;
	const int delay;
	const int sort_num;
	const bool option_sound;
	int el_changes = 0;
	sf::Event event;
	sf::RectangleShape* lines;
	const std::string path_to_sound = "media/sound/";
	const int num_of_sound_files = 7;
	const std::string file_location[7] = { "piano-c_C_major.wav", "piano-d_D_major.wav", "piano-e_E_major.wav",
		"piano-f_F_major.wav", "piano-g_G_major.wav", "piano-a_A_major.wav", "piano-b_B_major.wav" };
	sf::SoundBuffer* file_buffers;
	sf::Sound* sound;
	int* sound_transition_index;
	int sound_transition = 0;
	std::chrono::time_point<std::chrono::system_clock> global_start;
	std::chrono::time_point<std::chrono::system_clock> global_end = std::chrono::time_point<std::chrono::system_clock>();
	std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> start;
	std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> end;
	void display_sort(int sort_num);
	void draw();
	void draw_change(int index);
	void swap(int a, int b);
	void end_phase();
	// bubble sort
	void bubble_sort();
	// quick sort
	void quick_sort(int low, int high);
	int partition(int low, int high);
	// selection sort
	void selection_sort();
	// insertion sort
	void insertion_sort();
	// merge sort
	void merge_sort(int begin, int end);
	void merge(int left, int mid, int right);
	// heap sort
	void heap_sort(int n);
	void heapify(int n, int i);
};