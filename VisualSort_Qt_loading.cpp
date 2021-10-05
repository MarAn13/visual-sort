#include "VisualSort_Qt_loading.h"

VisualSort_Qt_loading::VisualSort_Qt_loading(QWidget* parent)
	: QWidget(parent), ui(new Ui::Form)
{
	this->setWindowTitle("Loading screen");
	ui->setupUi(this);
	int delay = 1000;
	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, QOverload<>::of(&VisualSort_Qt_loading::update_pixelmap));
	timer->start(delay);
}

VisualSort_Qt_loading::~VisualSort_Qt_loading()
{
	delete ui;
}

void VisualSort_Qt_loading::update_pixelmap()
{
	QPixmap pix;

	if (pix.load(QString::fromStdString(qrc_prefix + path_to_folder + file_paths[current_path]))) {
		pix = pix.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		ui->label->setPixmap(pix);
		if (current_path == file_paths_num - 1) {
			current_path = 0;
		}
		else {
			++current_path;
		}
	}
}

void VisualSort_Qt_loading::closeEvent(QCloseEvent* event)
{
	exit(0);
}