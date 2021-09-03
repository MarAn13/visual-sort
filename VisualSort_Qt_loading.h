#pragma once

#include <QtWidgets/QWidget>
#include <QTimer>
#include "ui_VisualSort_loading_screen.h"
#include <chrono>

class VisualSort_Qt_loading : public QWidget
{
	Q_OBJECT

public:
	VisualSort_Qt_loading(QWidget* parent = nullptr);
	~VisualSort_Qt_loading();
private:
	Ui::Form* ui;
	std::string path_to_folder = "media/sort_anim/";
	const int file_paths_num = 23;
	std::string file_paths[23] = { "sort_state_1.png", "sort_state_2_0.png", "sort_state_2_1.png", "sort_state_3_0.png", "sort_state_3_1.png", "sort_state_4_0.png",
	"sort_state_4_1.png", "sort_state_5_0.png", "sort_state_5_1.png", "sort_state_6_0.png", "sort_state_6_1.png", "sort_state_7_0.png", "sort_state_7_1.png",
		"sort_state_8_0.png", "sort_state_8_1.png", "sort_state_9_0.png", "sort_state_9_1.png", "sort_state_10_0.png", "sort_state_10_1.png", "sort_state_11_0.png",
		"sort_state_11_1.png", "sort_state_12_0.png", "sort_state_12_1.png" };
	int current_path = 0;
	void update_pixelmap();
	void closeEvent(QCloseEvent* event);
};
