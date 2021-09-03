#include "VisualSort_Qt.h"

VisualSort_Qt::VisualSort_Qt(QWidget* parent)
	: QMainWindow(parent), ui(new Ui::VisualSort_Qt_SFMLClass)
{
	this->setWindowTitle("Visual Sort");
	ui->setupUi(this);
}

VisualSort_Qt::~VisualSort_Qt()
{
	delete ui;
}

void VisualSort_Qt::on_delay_valueChanged() {
	ui->delay_show->display(ui->delay->value());
}

void VisualSort_Qt::on_ok_clicked() {
	number_of_el = ui->el_num->value();
	delay = ui->delay_show->value();
	sort_num = ui->sort_combo->currentIndex();
	option_sound = ui->option_sound->isChecked();
	this->hide();
	VisualSort_Qt_loading load_window;
	load_window.show();
	VisualSort_SFML sort(number_of_el, delay, sort_num, option_sound, this);
	this->show();
	load_window.hide();
}