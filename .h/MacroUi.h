#pragma once
#include <qabstractnativeeventfilter.h>
#include "ui_MacroUi.h"
#include "RecordUi.h"
#include "../static.h"

class MacroUi : public QWidget, public QAbstractNativeEventFilter
{
	Q_OBJECT;
	Ui::MacroUiClass ui;

	const int32 countMax = 999999;

public:
	MacroUi(QWidget* parent) : QWidget(parent)
	{
		setWindowFlags(Qt::FramelessWindowHint);
		ui.setupUi(this);
		WidInit();
	}

private:

	void WidInit()
	{
		if (Global::ir.set.key)
		{
			if (!RegisterHotKey((HWND)winId(), 0, Global::ir.set.key >> 16, Global::ir.set.key & 0xFFFF)) MsgBox::Warning(L"该按键已被占用或不可用");
		}

		ui.etCount->setValidator(new QIntValidator(0, countMax, this));

		// Table
		{
			ui.tbItem->setColumnCount(1);
			QTableWidgetItem* tbi = new QTableWidgetItem(QString::fromUtf8(u8"名称"));
			ui.tbItem->setHorizontalHeaderItem(0, tbi);
			ui.tbItem->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
		}
		TbUpdate();
		WidEvent();
	}

	void WidEvent()
	{
		qApp->installNativeEventFilter(this);

		connect(ui.tbItem, SIGNAL(cellClicked(int, int)), SLOT(ObTbActive(int, int)));
		connect(ui.bnRec, SIGNAL(clicked()), this, SLOT(OnBnRec()));
		connect(ui.bnPlay, SIGNAL(clicked()), this, SLOT(OnBnPlay()));
		connect(ui.bnDel, SIGNAL(clicked()), this, SLOT(OnBnDel()));
	}

	void TbUpdate()
	{
		ui.tbItem->clearMask();
		ui.tbItem->setRowCount(Global::ir.irs.size());
		ui.tbItem->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Fixed);
		ui.tbItem->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
		ui.tbItem->verticalHeader()->setDefaultSectionSize(0);

		for (uint32 u = 0; u < Global::ir.irs.size(); u++) {
			ui.tbItem->setItem(u, 0, new QTableWidgetItem(QString::fromWCharArray(Global::ir.irs[u].name.c_str())));
			ui.tbItem->item(u, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}

		ui.bnPlay->setDisabled(1);
		ui.bnDel->setDisabled(1);
	}

	bool nativeEventFilter(const QByteArray& type, LPVOID msg, LPLONG)
	{
		LPMSG m = (LPMSG)msg;
		if (m->message == WM_HOTKEY)
		{
			OnBnPlay();
			return 1;
		}
		return 0;
	}

private slots:
	void ObTbActive(int row, int column)
	{
		if (row < 0) return;
		ui.bnPlay->setDisabled(0);
		ui.bnDel->setDisabled(0);
	}

	void OnBnRec()
	{
		Global::ir.main->hide();
		RecordUi rec;
		rec.Start();
		TbUpdate();
		Global::ir.main->show();
	}
	void OnBnPlay()
	{
		int row = ui.tbItem->currentRow();
		if (row < 0) return;
		if (ui.etCount->text().size()) Global::ir.irs[row].count = ui.etCount->text().toInt();
		else Global::ir.irs[row].count = 1;
		Global::ir.main->hide();
		PlayInput(Global::ir.irs[row]);
		Global::ir.main->show();
	}
	void OnBnDel()
	{
		int row = ui.tbItem->currentRow();
		if (row < 0) return;

		std::wstring path(IrFolder);
		path += Global::ir.irs[row].name + IrFile;
		File::FileDelete(path.c_str());

		LoadData();
		TbUpdate();
	}
};