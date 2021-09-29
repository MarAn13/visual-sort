#include "VisualSort_SFML.h"

VisualSort_SFML::VisualSort_SFML(const int number_of_el, const int delay, const int sort_num, const bool option_sound, QWidget* parent)
	:number_of_el(number_of_el), delay(delay), sort_num(sort_num), option_sound(option_sound), parent(parent)
{
	try {
		HWND hd = GetDesktopWindow();
		HMONITOR monitor = MonitorFromWindow(hd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO info;
		info.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(monitor, &info);
		int monitor_width = info.rcMonitor.right - info.rcMonitor.left;
		int monitor_height = info.rcMonitor.bottom - info.rcMonitor.top;
		float scale = 0.5;
		window.create(sf::VideoMode(monitor_width * scale, monitor_height * scale), "Visual sort", sf::Style::Titlebar | sf::Style::Close);
		if (!font.loadFromFile(path_to_font + font_file)) {
			throw std::exception("Font file was not found");
		}
		text.setFont(font);
		int character_size = 30;
		text.setCharacterSize(character_size);
		lines = new sf::RectangleShape[number_of_el];
		int num_of_title_lines = 4;
		double info_height = (static_cast<double>(text.getCharacterSize()) + text.getLineSpacing()) * (static_cast<double>(num_of_title_lines) + 1);
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(1, static_cast<double>(window.getSize().y) - info_height);
		double line_width = static_cast<double>(window.getSize().x) / static_cast<double>(number_of_el);
		for (int i = 0; i < number_of_el; i++) {
			lines[i] = sf::RectangleShape(sf::Vector2f(line_width, dist(mt)));
		}
		file_buffers = new sf::SoundBuffer[num_of_sound_files];
		sound = new sf::Sound[num_of_sound_files];
		sound_transition_index = new int[num_of_sound_files];
		for (int i = 0; i < num_of_sound_files; ++i) {
			sf::SoundBuffer buffer;
			std::string file = path_to_sound + file_location[i];
			if (!buffer.loadFromFile(file)) {
				std::string file_exception = "Sound file " + file + " was not found";
				throw std::exception(file_exception.c_str());
			}
			file_buffers[i] = buffer;
			sf::Sound sound_temp;
			sound_temp.setBuffer(file_buffers[i]);
			sound[i] = sound_temp;
			sound_transition_index[i] = static_cast<int>(ceil(static_cast<double>(number_of_el) / num_of_sound_files * (static_cast<double>(i) + 1)));
		}
		global_start = std::chrono::system_clock::now();
		display_sort(sort_num);
	}
	catch (std::exception& e) {
		if (parent) {
			QMessageBox::critical(parent, "Error", e.what());
		}
		else {
			std::cout << e.what() << std::endl;
		}
	}
}

VisualSort_SFML::VisualSort_SFML()
	:number_of_el(100), delay(1), sort_num(0), option_sound(true), parent(nullptr) {
	try {
		HWND hd = GetDesktopWindow();
		HMONITOR monitor = MonitorFromWindow(hd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO info;
		info.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(monitor, &info);
		int monitor_width = info.rcMonitor.right - info.rcMonitor.left;
		int monitor_height = info.rcMonitor.bottom - info.rcMonitor.top;
		float scale = 0.5;
		window.create(sf::VideoMode(monitor_width * scale, monitor_height * scale), "Visual sort", sf::Style::Titlebar | sf::Style::Close);
		if (!font.loadFromFile(path_to_font + font_file)) {
			throw std::exception("Font file was not found");
		}
		text.setFont(font);
		int character_size = 30;
		text.setCharacterSize(character_size);
		lines = new sf::RectangleShape[number_of_el];
		int num_of_title_lines = 4;
		double info_height = (static_cast<double>(text.getCharacterSize()) + text.getLineSpacing()) * (static_cast<double>(num_of_title_lines) + 1);
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(1, static_cast<double>(window.getSize().y) - info_height);
		double line_width = static_cast<double>(window.getSize().x) / static_cast<double>(number_of_el);
		for (int i = 0; i < number_of_el; i++) {
			lines[i] = sf::RectangleShape(sf::Vector2f(line_width, dist(mt)));
		}
		file_buffers = new sf::SoundBuffer[num_of_sound_files];
		sound = new sf::Sound[num_of_sound_files];
		sound_transition_index = new int[num_of_sound_files];
		for (int i = 0; i < num_of_sound_files; ++i) {
			sf::SoundBuffer buffer;
			std::string file = path_to_sound + file_location[i];
			if (!buffer.loadFromFile(file)) {
				const char* file_exception = ("Sound file " + file + " was not found").c_str();
				throw std::exception(file_exception);
			}
			file_buffers[i] = buffer;
			sf::Sound sound_temp;
			sound_temp.setBuffer(file_buffers[i]);
			sound[i] = sound_temp;
			sound_transition_index[i] = static_cast<int>(ceil(static_cast<double>(number_of_el) / num_of_sound_files * (static_cast<double>(i) + 1)));
		}
		global_start = std::chrono::system_clock::now();
		display_sort(sort_num);
	}
	catch (std::exception& e) {
		if (parent) {
			QMessageBox::critical(parent, "Error", e.what());
		}
		else {
			std::cout << e.what() << std::endl;
		}
	}
}

VisualSort_SFML::~VisualSort_SFML()
{
	delete[] lines, file_buffers, sound, sound_transition_index;
}

void VisualSort_SFML::draw()
{
	window.clear(sf::Color::Black);
	std::chrono::duration<double> temp_elapsed_time = std::chrono::system_clock::now() - global_start;
	if (global_end != std::chrono::time_point<std::chrono::system_clock>()) {
		temp_elapsed_time = global_end - global_start;
	}
	std::string temp = std::to_string(temp_elapsed_time.count());
	temp = temp.substr(0, temp.find('.') + 3);
	text.setString("Array elements: " + std::to_string(number_of_el) + '\n' +
		"Actions done: " + std::to_string(el_changes) + '\n' +
		"Duration: " + temp + " seconds" + '\n' +
		"Delay: " + std::to_string(delay) + " milliseconds");
	window.draw(text);
	for (int i = 0; i < number_of_el; i++) {
		lines[i].setFillColor(sf::Color::White);
		lines[i].setPosition(0 + i * lines[i].getSize().x, window.getSize().y - lines[i].getSize().y);
		window.draw(lines[i]);
	}
	window.display();
}

void VisualSort_SFML::draw_change(int index) {
	// line swap sound
	if (option_sound && window.isOpen()) {
		sound[sound_transition].pause();
		if (index < sound_transition_index[sound_transition] && index < sound_transition_index[sound_transition - 1]) {
			sound[sound_transition].stop();
			sound_transition = 0;
		}
		while (index > sound_transition_index[sound_transition]) {
			sound[sound_transition].stop();
			++sound_transition;
		}
		sound[sound_transition].play();
	}
	//  line swap coloring
	lines[index].setFillColor(sf::Color::Red);
	lines[index].setPosition((index)*lines[index].getSize().x, window.getSize().y - lines[index].getSize().y);
	window.draw(lines[index]);
	window.display();
}

void VisualSort_SFML::swap(int a, int b)
{
	sf::RectangleShape line = lines[a];
	lines[a] = lines[b];
	lines[b] = line;
}

void VisualSort_SFML::display_sort(int sort_num)
{
	switch (sort_num) {
	case 0:
		bubble_sort();
		break;
	case 1:
		quick_sort(0, number_of_el - 1);
		break;
	case 2:
		selection_sort();
		break;
	case 3:
		insertion_sort();
		break;
	case 4:
		merge_sort(0, number_of_el - 1);
		break;
	case 5:
		heap_sort(number_of_el);
		break;
	}
	global_end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	end_phase();
}
void VisualSort_SFML::end_phase() {
	sound_transition = 0;
	window.clear(sf::Color::Black);
	start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	for (int i = 0; i < number_of_el; ++i) {
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed) {
				window.close();
				return;
			}
		}
		draw_change(i);
		end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		std::chrono::duration<double> elapsed = end - start;
		if (elapsed >= std::chrono::milliseconds(delay)) {
			draw();
			start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		}
	}
	draw();
	while (window.isOpen()) {
		if (window.waitEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				return;
			}
		}
	}
}
void VisualSort_SFML::bubble_sort() {
	start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	for (int i = 0; i < number_of_el - 1; ++i) {
		for (int j = 0; j < number_of_el - i - 1; ++j) {
			while (window.pollEvent(event))
			{
				// "close requested" event: we close the window
				if (event.type == sf::Event::Closed) {
					window.close();
					return;
				}
			}
			if (lines[j].getSize().y > lines[j + 1].getSize().y) {
				draw_change(j + 1);
				swap(j, j + 1);
				++el_changes;
				end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
				std::chrono::duration<double> elapsed = end - start;
				if (elapsed >= std::chrono::milliseconds(delay)) {
					draw();
					start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
				}
			}
		}
	}
}
int VisualSort_SFML::partition(int low, int high)
{
	while (window.pollEvent(event))
	{
		// "close requested" event: we close the window
		if (event.type == sf::Event::Closed) {
			window.close();
			return 0;
		}
	}
	int pivot = lines[high].getSize().y;
	int i = (low - 1);

	for (int j = low; j <= high - 1; j++)
	{
		if (lines[j].getSize().y < pivot)
		{
			++i;
			draw_change(j);
			swap(i, j);
			++el_changes;
			end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
			std::chrono::duration<double> elapsed = end - start;
			if (elapsed >= std::chrono::milliseconds(delay)) {
				draw();
				start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
			}
		}
	}
	draw_change(high);
	swap(i + 1, high);
	++el_changes;
	end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	std::chrono::duration<double> elapsed = end - start;
	if (elapsed >= std::chrono::milliseconds(delay)) {
		draw();
		start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	}
	return (i + 1);
}
void VisualSort_SFML::quick_sort(int low, int high)
{
	if (low < high)
	{
		int pi = partition(low, high);

		quick_sort(low, pi - 1);
		quick_sort(pi + 1, high);
	}
}
void VisualSort_SFML::selection_sort()
{
	int max_idx;

	for (int i = 0; i < number_of_el - 1; i++)
	{
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed) {
				window.close();
				return;
			}
		}
		max_idx = i;
		for (int j = i + 1; j < number_of_el; j++) {
			if (lines[j].getSize().y < lines[max_idx].getSize().y) {
				max_idx = j;
			}
		}
		draw_change(max_idx);
		swap(i, max_idx);
		++el_changes;
		end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		std::chrono::duration<double> elapsed = end - start;
		if (elapsed >= std::chrono::milliseconds(delay)) {
			draw();
			start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		}
	}
}
void VisualSort_SFML::insertion_sort()
{
	for (int i = 1; i < number_of_el; i++)
	{
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed) {
				window.close();
				return;
			}
		}
		sf::RectangleShape key = lines[i];
		int j = i - 1;
		while (j != -1 && lines[j].getSize().y > key.getSize().y)
		{
			draw_change(j);
			lines[j + 1] = lines[j];
			--j;
			++el_changes;
		}
		draw_change(i);
		lines[j + 1] = key;
		++el_changes;
		end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		std::chrono::duration<double> elapsed = end - start;
		if (elapsed >= std::chrono::milliseconds(delay)) {
			draw();
			start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		}
	}
}
void VisualSort_SFML::merge(int left, int mid, int right)
{
	int subArrayOne = mid - left + 1;
	int subArrayTwo = right - mid;

	sf::RectangleShape* leftArray = new sf::RectangleShape[subArrayOne],
		* rightArray = new sf::RectangleShape[subArrayTwo];

	for (int i = 0; i < subArrayOne; i++)
		leftArray[i] = lines[left + i];
	for (int j = 0; j < subArrayTwo; j++)
		rightArray[j] = lines[mid + 1 + j];

	int indexOfSubArrayOne = 0, // Initial index of first sub-array
		indexOfSubArrayTwo = 0; // Initial index of second sub-array
	int indexOfMergedArray = left; // Initial index of merged array

	// Merge the temp arrays back into array[left..right]
	while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed) {
				window.close();
				return;
			}
		}
		if (leftArray[indexOfSubArrayOne].getSize().y <= rightArray[indexOfSubArrayTwo].getSize().y) {
			draw_change(indexOfMergedArray);
			lines[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
			indexOfSubArrayOne++;
			++el_changes;
		}
		else {
			draw_change(indexOfMergedArray);
			lines[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
			indexOfSubArrayTwo++;
			++el_changes;
		}
		end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		std::chrono::duration<double> elapsed = end - start;
		if (elapsed >= std::chrono::milliseconds(delay)) {
			draw();
			start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		}
		indexOfMergedArray++;
	}
	// Copy the remaining elements of
	// left[], if there are any
	while (indexOfSubArrayOne < subArrayOne) {
		draw_change(indexOfMergedArray);
		lines[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
		indexOfSubArrayOne++;
		indexOfMergedArray++;
		++el_changes;
	}
	// Copy the remaining elements of
	// right[], if there are any
	while (indexOfSubArrayTwo < subArrayTwo) {
		draw_change(indexOfMergedArray);
		lines[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
		indexOfSubArrayTwo++;
		indexOfMergedArray++;
		++el_changes;
	}
	end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	std::chrono::duration<double> elapsed = end - start;
	if (elapsed >= std::chrono::milliseconds(delay)) {
		draw();
		start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	}
}

void VisualSort_SFML::merge_sort(int begin, int end)
{
	if (begin >= end) {
		return;
	}

	int mid = begin + (end - begin) / 2;
	merge_sort(begin, mid);
	merge_sort(mid + 1, end);
	merge(begin, mid, end);
}

void VisualSort_SFML::heapify(int n, int i)
{
	while (window.pollEvent(event))
	{
		// "close requested" event: we close the window
		if (event.type == sf::Event::Closed) {
			window.close();
			return;
		}
	}
	int largest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;

	if (l < n && lines[l].getSize().y > lines[largest].getSize().y)
		largest = l;

	if (r < n && lines[r].getSize().y > lines[largest].getSize().y)
		largest = r;

	if (largest != i) {
		draw_change(largest);
		swap(i, largest);
		++el_changes;
		end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		std::chrono::duration<double> elapsed = end - start;
		if (elapsed >= std::chrono::milliseconds(delay)) {
			draw();
			start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		}

		heapify(n, largest);
	}
}

void VisualSort_SFML::heap_sort(int n)
{
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(n, i);

	for (int i = n - 1; i > 0; i--) {
		draw_change(i);
		swap(0, i);
		++el_changes;

		heapify(i, 0);
	}
}