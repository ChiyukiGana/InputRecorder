#pragma once
#include <fstream>
#include "InputRecorderDef.h"
#define IrFolder L"record\\"
#define IrFile L".inrec"

struct InrecFileStruct
{
	union
	{
		byte size[sizeof(Action)];
		uint32 length = 0;
	};
	Action* actions = 0;
};

static void SaveRecord()
{
	for (uint32 u = 0; u < Global::ir.irs.size(); u++)
	{
		if (Global::ir.irs[u].actions.size())
		{
			InrecFileStruct ifs;
			memset(&ifs, 0, sizeof(InrecFileStruct));
			ifs.length = Global::ir.irs[u].actions.size();
			ifs.actions = &Global::ir.irs[u].actions[0];
			File::FolderCreate(IrFolder);
			std::wstring path(IrFolder);
			path += Global::ir.irs[u].name + IrFile;
			std::ofstream fs(path, std::ios::out | std::ios::binary);
			if (fs.good())
			{
				fs.write((char*)(&ifs.length), sizeof(ifs.size)); // write length info
				fs.write((char*)ifs.actions, sizeof(ifs.size) * ifs.length); // write data
				fs.close();
			}
			else MsgBox::Error(Global::ir.irs[u].name.c_str(), L"±£¥Ê¬º÷∆ ß∞‹");
		}
	}
}
static void LoadRecord()
{
	File::FileList files = File::FindFile(((std::wstring)IrFolder + (std::wstring)L"*" + IrFile).c_str());
	std::wstring error;
	for (uint32 u = 0; u < files.size(); u++) {
		InputRecord& ir = Global::ir.irs.AddNull();
		ir.name = (std::wstring(files[u].name)).substr(0, wcslen(files[u].name) - 6);
	}
}
static void LoadRecordData(InputRecord* ir)
{
	if (!ir->actions.size())
	{
		std::wstring path(IrFolder);
		path += ir->name + IrFile;
		std::ifstream fs(path, std::ios::in | std::ios::binary);
		if (fs.good())
		{
			InrecFileStruct ifs;
			memset(&ifs, 0, sizeof(InrecFileStruct));
			fs.read((char*)(&ifs.length), sizeof(ifs.size)); // read header
			ifs.actions = new Action[sizeof(ifs.size) * ifs.length]; // buffer
			fs.read((char*)ifs.actions, sizeof(ifs.size) * ifs.length); // read data
			ir->actions.AddArray(ifs.actions, ifs.length); // copy data
			delete[] ifs.actions;
		}
		else MsgBox::Warning(L"º”‘ÿ¬º÷∆ ß∞‹");
	}
}


static void SaveJson()
{
	neb::CJsonObject cfg;
	std::string str;
	cfg.Add("document_charset", std::string("UTF8"));
	cfg.Add("key", Global::ir.set.key);
	cfg.Add("recKey", Global::ir.set.recKey);
	cfg.Add("fps", Global::ir.set.fps);
	cfg.Add("recDelay", Global::ir.set.recDelay);
	cfg.Add("playDelay", Global::ir.set.playDelay);
	File::TextSave("InputRecorder.json", cfg.ToString());
}
static void LoadJson()
{
	Global::ir.irs.clear();
	if (File::FileState(L"InputRecorder.json"))
	{
		neb::CJsonObject cfg(File::TextLoad("InputRecorder.json"));
		std::string str;
		cfg.Get("key", Global::ir.set.key);
		cfg.Get("recKey", Global::ir.set.recKey);
		cfg.Get("fps", Global::ir.set.fps);
		cfg.Get("recDelay", Global::ir.set.recDelay);
		cfg.Get("playDelay", Global::ir.set.playDelay);
	}
	else
	{
		Global::ir.set.key = VK_F8;
		Global::ir.set.recKey = VK_F9;
		Global::ir.set.fps = 100;
		Global::ir.set.recDelay = 3;
		Global::ir.set.playDelay = 0;
	}
}
static void LoadData()
{
	LoadJson();
	LoadRecord();
}