#include <QtWidgets/QApplication>
#include ".h/MainUi.h"
#include "static.h"

InputHookProc()
{
	if (Global::ir.rec)
	{
		static clock_t tprev;
		if (Global::ir.clock.now == 0) tprev = 0;
		Global::ir.clock.update();
		if (Global::ir.set.recKey && (Input::Convert(vk) == Global::ir.set.recKey)) return 0;
		if (vk)
		{
			Action action;
			action.vk = vk, action.vkState = state; action.x = msPt.x, action.y = msPt.y; action.clock = Global::ir.clock.now;
			Global::ir.rec->actions.Add(action);

			tprev = action.clock;
		}
		else
		{
			if (Global::ir.set.fps)
			{
				if ((tprev + (1000 / Global::ir.set.fps)) < Global::ir.clock.now)
				{
					Action action;
					action.x = msPt.x, action.y = msPt.y; action.clock = Global::ir.clock.now;
					Global::ir.rec->actions.Add(action);

					tprev = action.clock;
				}
			}
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	std::locale::global(std::locale(".UTF8"));
	Process::RunPath();
	Process::RunOnce(L"InputRecorder");
	timeBeginPeriod(1);

	InitUi(1);
	LoadData();
	Thread::Start(TipsWindow::TipsWindowThread);

    QApplication app(argc, argv);
    app.setFont(QFont("Microsoft YeHei"));

    MainUi w;
	Global::ir.main = &w;
    w.show();

    return app.exec();
	timeBeginPeriod(0);
}
