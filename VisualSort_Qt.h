#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VisualSort_main_menu.h"
#include "VisualSort_Qt_loading.h"
#include "VisualSort_SFML.h"

class VisualSort_Qt : public QMainWindow
{
	Q_OBJECT

public:
	VisualSort_Qt(QWidget* parent = Q_NULLPTR);
	~VisualSort_Qt();

private:
	Ui::VisualSort_Qt_SFMLClass* ui;
	int number_of_el;
	int delay;
	int sort_num;
	bool option_sound;
private slots:
	void on_delay_valueChanged();
	void on_ok_clicked();
};
