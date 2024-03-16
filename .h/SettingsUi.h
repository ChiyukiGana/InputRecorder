#pragma once
#include <qvalidator.h>
#include "ui_SettingsUi.h"
#include "../static.h"

class SettingsUi : public QWidget
{
	Q_OBJECT;
	Ui::SettingsUiClass ui;

public:
	SettingsUi(QWidget* parent) : QWidget(parent)
	{
		setWindowFlags(Qt::FramelessWindowHint);
		ui.setupUi(this);
		WidInit();
	}

private:

	void WidInit()
	{
		ui.hkKey->Mode(1);
		ui.hkKey->VirtualKey(Global::ir.set.key & 0xFFFF, Global::ir.set.key >> 16);
		ui.hkRec->Mode(0);
		ui.hkRec->VirtualKey(Global::ir.set.recKey);
		ui.etFps->setValidator(new QIntValidator(1, 999, this));
		ui.etRecDelay->setValidator(new QIntValidator(1, 999, this));
		ui.etPlayDelay->setValidator(new QIntValidator(1, 999, this));
		ui.etFps->setText(QString::number(Global::ir.set.fps));
		ui.etRecDelay->setText(QString::number(Global::ir.set.recDelay));
		ui.etPlayDelay->setText(QString::number(Global::ir.set.playDelay));
		WidEvent();
	}

	void WidEvent()
	{
		connect(ui.hkKey, SIGNAL(changed()), this, SLOT(OnHkKey()));
		connect(ui.hkRec, SIGNAL(changed()), this, SLOT(OnHkRec()));
		connect(ui.etFps, SIGNAL(textEdited(const QString&)), this, SLOT(OnEtFps(const QString&)));
		connect(ui.etPlayDelay, SIGNAL(textEdited(const QString&)), this, SLOT(OnEtPlayDelay(const QString&)));
		connect(ui.etRecDelay, SIGNAL(textEdited(const QString&)), this, SLOT(OnEtRecDelay(const QString&)));
	}

private slots:
	void OnHkKey()
	{
		Global::ir.set.key = ui.hkKey->virtualKey();
		UnregisterHotKey((HWND)Global::ir.mcui->winId(), 0);
		if (!RegisterHotKey((HWND)Global::ir.mcui->winId(), 0, Global::ir.set.key >> 16, Global::ir.set.key & 0xFFFF)) MsgBox::Warning(L"该按键已被占用或不可用");
		SaveJson();
	}
	void OnHkRec()
	{
		Global::ir.set.recKey = ui.hkRec->virtualKey();
		SaveJson();
	}
	void OnEtFps(const QString& text)
	{
		Global::ir.set.fps = text.toInt();
		SaveJson();
	}
	void OnEtRecDelay(const QString& text)
	{
		Global::ir.set.recDelay = text.toInt();
		SaveJson();
	}
	void OnEtPlayDelay(const QString& text)
	{
		Global::ir.set.playDelay = text.toInt();
		SaveJson();
	}
};