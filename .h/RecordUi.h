#pragma once
#include <qabstractnativeeventfilter.h>
#include <qevent.h>
#include "ui_RecordUi.h"
#include "../static.h"

class RecordUi : public QDialog, public QAbstractNativeEventFilter
{
	Q_OBJECT;
	Ui::RecordUiClass ui;

public:
	RecordUi() : QDialog()
	{
		ui.setupUi(this);
		setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
		setMouseTracking(true);

		if (Global::ir.set.recKey)
		{
			WORD key = 0;
			switch (Global::ir.set.recKey)
			{
			case VK_CONTROL: key = MOD_CONTROL; break;
			case VK_SHIFT: key = MOD_SHIFT; break;
			case VK_MENU: key = MOD_ALT; break;
			}
			if (key)
			{
				if (!RegisterHotKey((HWND)winId(), 0, key, 0)) MsgBox::Warning(L"该按键已被占用或不可用");
			}
			else
			{
				if (!RegisterHotKey((HWND)winId(), 0, 0, Global::ir.set.recKey)) MsgBox::Warning(L"该按键已被占用或不可用");
			}
		}
		ui.bnStart->setText(UI::rcStart);
		ui.bnClose->setText(UI::rcClose);

		WidEvent();
	}

	void Start()
	{
		if (Global::ir.set.recKey)
		{
			std::wstring text(L"按下");
			text += Input::Name(Global::ir.set.recKey);
			text += L"开始录制";
			TipsWindow::Show(text, RGB(0x20, 0xFF, 0x20));
		}
		exec();
	}

private:
	void WidEvent()
	{
		qApp->installNativeEventFilter(this);
		connect(ui.bnStart, SIGNAL(clicked()), this, SLOT(OnBnStart()));
		connect(ui.bnClose, SIGNAL(clicked()), this, SLOT(OnBnClose()));
	}

	bool nativeEventFilter(const QByteArray& type, LPVOID msg, LPLONG)
	{
		LPMSG m = (LPMSG)msg;
		if (m->message == WM_HOTKEY)
		{
			OnBnStart();
			return 1;
		}
		return 0;
	}

	void mouseMoveEvent(QMouseEvent* et) { if (et->buttons() & Qt::LeftButton) move(et->pos() + pos() - QPoint(5, 15)); }

public slots:
	void OnBnStart()
	{
		if (Global::ir.rec)
		{
			if (Global::ir.rec)
			{
				Global::ir.rec->name = NameFilter(L"录制");
				Global::ir.rec = 0;
				SaveRecord();
			}
			HookState(0);
			TipsWindow::Hide();
			close();
		}
		else
		{
			for (uint32 t = Global::ir.set.recDelay; t > 0; t--)
			{
				TipsWindow::Show(std::to_wstring(t));
				sleep(1000);
			}
			if (Global::ir.set.recKey)
			{
				std::wstring text(L"按下");
				text += Input::Name(Global::ir.set.recKey);
				text += L"停止录制";
				TipsWindow::Show(text, RGB(0x20, 0xFF, 0x20));
			}
			ui.bnStart->setText(UI::rcStop);
			Global::ir.rec = &Global::ir.irs.AddNull();
			Global::ir.clock.reset();
			HookState(1);
		}
	}
	void OnBnClose()
	{
		if (Global::ir.rec)
		{
			Global::ir.irs.DelBack();
			Global::ir.rec = 0;
		}
		HookState(0);
		TipsWindow::Hide();
		close();
	}
};