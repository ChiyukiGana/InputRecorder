#pragma once
#include <qevent.h>
#include "ui_UpdateUi.h"
#include "../static.h"

class UpdateUi : public QDialog
{
	Q_OBJECT;
	Ui::UpdateUiClass ui;
	QPoint msPos;

public:
	UpdateUi() : QDialog()
	{
		ui.setupUi(this);
		setWindowFlags(Qt::FramelessWindowHint);
		connect(ui.bnClose, SIGNAL(clicked()), this, SLOT(OnBnClose()));
	}

private:
	void mousePressEvent(QMouseEvent* et) { if (et->buttons() & Qt::LeftButton) msPos = et->pos(); }
	void mouseMoveEvent(QMouseEvent* et) { if (et->buttons() & Qt::LeftButton) move(et->pos() + pos() - msPos); }

private slots:
	void OnBnClose() { close(); }
};