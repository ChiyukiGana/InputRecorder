#pragma once
#include <QtWidgets/QWidget>
#include "../cg/cg.h"

struct Clock
{
	clock_t begin = 0;
	clock_t now = 0;
	void reset() { begin = clock(), now = 0; }
	void update() { now = clock() - begin; }
};

struct UI
{
	static float zoomRote;
	static QString syOn;
	static QString syOff;
	static QString syOk;
	static QString syYes;
	static QString syNot;
	static QString syStop;
	static QString rcStart;
	static QString rcStop;
	static QString rcClose;
};

struct Action
{
	int32 clock = 0;
	byte vk = 0;
	byte vkState = 0;
	int16 x = 0;
	int16 y = 0;
};
typedef List<Action> Actions;

struct InputRecord
{
	uint32 key = 0;
	uint32 count = 0;
	std::wstring name;
	Actions actions;
};
typedef List<InputRecord> InputRecords;

struct SettingsData
{
	uint32 key = 0;
	uint32 recKey = 0;
	uint32 fps = 0;
	uint32 recDelay = 0;
	uint32 playDelay = 0;
};

struct InputRecorderStruct
{
	SettingsData set;

	InputRecords irs;
	InputRecord* rec = 0;

	Clock clock;

	QWidget* main = 0;
	QWidget* mcui = 0;
	QWidget* stui = 0;
	QWidget* abui = 0;
	QWidget* rcui = 0;
};

struct Global
{
	static InputRecorderStruct ir;
};

static void LoadRecordData(InputRecord*);
static void PlayInput(InputRecord& ir)
{
	LoadRecordData(&ir);
	Thread::Sleep(Global::ir.set.playDelay * 1000);
	int32 tbegin = clock();
	int32 now = tbegin;
	int32 delay = 0;
	uint32 count = 0;
	while (count < ir.count)
	{
		for (uint32 u = 0; u < ir.actions.size(); u++)
		{
			if (!ir.actions[u].vk || Input::Type(ir.actions[u].vk)) Input::MoveTo(ir.actions[u].x, ir.actions[u].y);
			Input::State(ir.actions[u].vk, ir.actions[u].vkState);

			if (u < ir.actions.size() - 1)
			{
				delay = ir.actions[u + 1].clock - ir.actions[u].clock;
				if (delay > 0) Thread::Sleep(delay);

				now = clock() - tbegin;
				while (now < ir.actions[u + 1].clock) now = clock() - tbegin;
			}
		}
		count++;
	}
}

static void HookState(bool state)
{
	if (state) InputHook::Start(InputHook::all);
	else InputHook::Close();
}

static std::wstring NameFilter(std::wstring name)
{
	for (uint32 n = 0;; n++)
	{
		for (uint32 nx = 0; nx < Global::ir.irs.size(); nx++)
		{
			std::wstring find = name + L" " + String::toWString(n + 1);
			if (Global::ir.irs[nx].name == find)
			{
				break;
			}
			if (nx >= Global::ir.irs.size() - 1)
			{
				return find;
			}
		}
	}
	return L"";
}