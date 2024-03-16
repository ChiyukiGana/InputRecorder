#pragma once
#include ".h/FileDef.h"
#include ".h/TipsWindow.h"

static void InitUi(bool zoom)
{
	UI::zoomRote = 1.0f;
	if (zoom && System::screenSize().cy > 1200)
	{
		UI::zoomRote = System::screenZoomRote();
		qputenv("QT_SCALE_FACTOR", QByteArray::number(UI::zoomRote, 10, 1));
	}
	{
		if (System::Version().dwMajorVersion >= 10)
		{
			UI::syOn = u8"✅";
			UI::syOff = u8"⛔";
			UI::syOk = u8"⭕";
			UI::syYes = u8"✔️";
			UI::syNot = u8"❌";
			UI::syStop = u8"🛑";
		}
		else
		{
			UI::syOn = u8"✔";
			UI::syOff = u8"✘";
			UI::syOk = u8"✔";
			UI::syYes = u8"✔";
			UI::syNot = u8"✘";
			UI::syStop = u8"Ⓢ";
		}
		{
			UI::rcStart = (QString::fromUtf8(u8"开始") + UI::syOk);
			UI::rcStop = (QString::fromUtf8(u8"停止") + UI::syOk);
			UI::rcClose = (QString::fromUtf8(u8"取消") + UI::syNot);
		}
	}
}