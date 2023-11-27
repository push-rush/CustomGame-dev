#include "pch.h"
#include "common.h"
#include <stdarg.h>
#include <stdio.h>
#include <Windows.h>
#include "App.xaml.h"

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::System::Threading;
using namespace Windows::Foundation;
using namespace FmodExample;


std::vector<char *> gStringList;


void Common_Init(void **extraDriverData)
{
	if (extraDriverData)
	{
		*extraDriverData = nullptr;
	}
}

void Common_Close()
{
	App::Get()->Exit();
}

void Common_Exit(int code)
{
	App::Get()->Exit();
}


void _Common_Sleep(unsigned int ms);

void Common_Update()
{
	App::Get()->Update();
	App::Get()->ClearText();
}

void Common_DrawText(const char *text)
{
    wchar_t ws[256];
        
	_snwprintf_s(ws, 256, L"%S\n", text);

	App::Get()->AddText(ws);
}

bool Common_BtnPress(Common_Button btn)
{
	return App::Get()->KeyPressed(btn);
}

bool Common_BtnDown(Common_Button btn)
{
	return App::Get()->KeyDown(btn);
}

const char *Common_BtnStr(Common_Button btn)
{
    switch (btn)
    {
        case BTN_ACTION1:   return "1";
        case BTN_ACTION2:   return "2";
        case BTN_ACTION3:   return "3";
        case BTN_ACTION4:   return "4";
        case BTN_LEFT:      return "LEFT";
        case BTN_RIGHT:     return "RIGHT";
        case BTN_UP:        return "UP";
        case BTN_DOWN:      return "DOWN";
        case BTN_MORE:      return "Q";
        case BTN_QUIT:      return "ESC";
        default:            return "Unknown";
    }
}

/*void Common_LoadFileMemory(const char *name, void **buff, int *length)
{
	HANDLE file;
	wchar_t wname[256];

	
	_snwprintf_s(wname, 256, L"%S", name);
	
	file = CreateFile2(wname, GENERIC_READ, 0, OPEN_EXISTING, NULL);
	
	HANDLE mapping;
	mapping = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);

	*buff = MapViewOfFileEx(mapping, FILE_MAP_READ, 0, 0, 0, NULL);

	
	FILE_STANDARD_INFO fileinfo;
	GetFileInformationByHandleEx(file, FileStandardInfo, &fileinfo, sizeof(FILE_STANDARD_INFO));

	*length = fileinfo.EndOfFile.LowPart;

	CloseHandle(mapping);
	CloseHandle(file);
}

void Common_UnloadFileMemory(void *buff)
{
	UnmapViewOfFile(buff);
}*/

void Common_LoadFileMemory(const char *name, void **buff, int *length)
{}

void Common_UnloadFileMemory(void *buff)
{
}


const char *Common_MediaPath(const char *fileName)
{    
    char *filePath = (char *)calloc(256, sizeof(char));

    strcat(filePath, "ms-appx:///");
    strcat(filePath, fileName);
    gStringList.push_back(filePath);

    return filePath;
}

const char *Common_WritePath(const char *fileName)
{
    String^ tempPath = Windows::Storage::ApplicationData::Current->TemporaryFolder->Path;

    char buf[512];
    sprintf_s(buf, "%S\\", tempPath->Begin());

    char *filePath = (char *)calloc(512, sizeof(char));

    strcat(filePath, buf);
    strcat(filePath, fileName);
    gStringList.push_back(filePath);

    return filePath;
}


void Common_Sleep(unsigned int ms)
{
	TimeSpan delay = { ms * 10000 };
	HANDLE sleepEvent = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);

	ThreadPoolTimer::CreateTimer(
		ref new TimerElapsedHandler(
			[=](ThreadPoolTimer^ op)
			{ 
				SetEvent(sleepEvent); 
			}
		), 
		delay);

	DWORD hr = WaitForSingleObjectEx(sleepEvent, INFINITE, FALSE);
		
	CloseHandle(sleepEvent);
}
