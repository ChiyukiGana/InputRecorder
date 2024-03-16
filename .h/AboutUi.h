#pragma once
#include "UpdateUi.h"
#include "ui_AboutUi.h"
#include "../static.h"

class AboutUi : public QWidget
{
	Q_OBJECT;
	Ui::AboutUiClass ui;

public:
	AboutUi(QWidget* parent) : QWidget(parent)
	{
		setWindowFlags(Qt::FramelessWindowHint);
		ui.setupUi(this);
		ui.lbText->installEventFilter(this);
	}

private slots:
	bool eventFilter(QObject* obj, QEvent* et)
	{
		if (obj == ui.lbText && et->type() == QEvent::MouseButtonRelease)
		{
			UpdateUi update;
			update.exec();
		}
		return 0;
	}
};