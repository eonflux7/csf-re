#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_FindWindowAImportSlot, 0x82D284);
CSF_GLOBAL(void*, g_SendMessageAImportSlot, 0x82D2EC);
CSF_GLOBAL(void*, g_CreateProcessAImportSlot, 0x82D228);
CSF_GLOBAL(void*, g_GetLastErrorImportSlot, 0x82D08C);
CSF_GLOBAL(void*, g_SleepImportSlot, 0x82D05C);

CSF_GLOBAL(uint32_t, g_LogTitlePrefixFirst, 0x85EDEC);
CSF_GLOBAL(uint16_t, g_LogTitlePrefixLast, 0x85EDF0);
CSF_GLOBAL(uint32_t, g_LogCommandFirst, 0x85EDD0);
CSF_GLOBAL(uint32_t, g_LogCommandSecond, 0x85EDD4);
CSF_GLOBAL(uint8_t, g_LogCommandTerminator, 0x85EDD8);

CSF_FUNC(int32_t, __cdecl, CoreZlibLog_SendLogLine, 0x6A1E30,
         (void** window, const char* format, ...));

static int __fastcall CoreZlibLog_ConnectLogWindow(
    void** self, void* /*edx*/, char* format, char /*unused*/)
{
    alignas(4) unsigned char startupInfo[0x48];
    alignas(4) unsigned char processInformation[0x10];
    char commandLine[0x80];
    char windowName[0x80];

    std::memset(startupInfo + 4, 0, 0x40);
    CSF_AT(uint32_t, startupInfo, 0) = 68;
    CSF_AT(uint16_t, startupInfo, 0x30) = 7;
    CSF_AT(uint32_t, startupInfo, 0x2C) = 1;

    CSF_AT(uint32_t, windowName, 0) = g_LogTitlePrefixFirst;
    CSF_AT(uint16_t, windowName, 4) = g_LogTitlePrefixLast;
    std::memset(windowName + 6, 0, 0x78);
    CSF_AT(uint16_t, windowName, 0x7E) = 0;

    using FindWindowFunction = void* (__stdcall*)(const char*, const char*);
    using SendMessageFunction =
        int32_t (__stdcall*)(void*, uint32_t, uint32_t, int32_t);
    FindWindowFunction findWindow;
    SendMessageFunction sendMessage;

    if (format)
    {
        const size_t length = std::strlen(format) + 1;
        char* destination = windowName;
        while (*destination)
            ++destination;
        std::memcpy(destination, format, length);

        findWindow = reinterpret_cast<FindWindowFunction>(g_FindWindowAImportSlot);
        void* found = findWindow(nullptr, windowName);
        sendMessage = reinterpret_cast<SendMessageFunction>(g_SendMessageAImportSlot);
        *self = found;
        if (found)
            sendMessage(found, 0x8192, 0, 0);
        if (*self)
            return 1;
    }
    else
    {
        findWindow = reinterpret_cast<FindWindowFunction>(g_FindWindowAImportSlot);
        sendMessage = reinterpret_cast<SendMessageFunction>(g_SendMessageAImportSlot);
    }

    void* found = findWindow(nullptr, CSF_STR(0x85EDDC, "LOG Conectado"));
    *self = found;
    if (found)
    {
        if (format)
        {
            sendMessage(found, 0x8193, 0, 0);
            CoreZlibLog_SendLogLine(self, format);
        }
        else
        {
            sendMessage(found, 0x8191, 0, 0);
        }

        void* currentWindow = *self;
        if (currentWindow)
            sendMessage(currentWindow, 0x8192, 0, 0);
        return 1;
    }

    CSF_AT(uint32_t, commandLine, 0) = g_LogCommandFirst;
    CSF_AT(uint32_t, commandLine, 4) = g_LogCommandSecond;
    CSF_AT(uint8_t, commandLine, 8) = g_LogCommandTerminator;

    using CreateProcessFunction = int (__stdcall*)(
        const char*, char*, void*, void*, int, uint32_t,
        void*, const char*, void*, void*);

    if (!reinterpret_cast<CreateProcessFunction>(g_CreateProcessAImportSlot)(
            nullptr, commandLine, nullptr, nullptr, 0, 0,
            nullptr, nullptr, startupInfo, processInformation))
    {
        uint32_t error =
            reinterpret_cast<uint32_t (__stdcall*)()>(g_GetLastErrorImportSlot)();
        CoreZlibLog_SendLogLine(self, CSF_STR(0x85EDCC, "%d\n"), error);
        return 0;
    }

    int attempts = 0;
    for (;;)
    {
        reinterpret_cast<void (__stdcall*)(uint32_t)>(g_SleepImportSlot)(0x64);
        found = findWindow(nullptr, CSF_STR(0x85EDC4, "DlgLog"));
        ++attempts;
        *self = found;
        if (found)
            break;
        if (attempts >= 10)
            return 0;
    }

    if (format)
    {
        reinterpret_cast<SendMessageFunction>(g_SendMessageAImportSlot)(
            found, 0x8193, 0, 0);
        CoreZlibLog_SendLogLine(self, format);
    }
    else
    {
        reinterpret_cast<SendMessageFunction>(g_SendMessageAImportSlot)(
            found, 0x8191, 0, 0);
    }

    void* currentWindow = *self;
    if (currentWindow)
        reinterpret_cast<SendMessageFunction>(g_SendMessageAImportSlot)(
            currentWindow, 0x8192, 0, 0);
    return 1;
}

HOOK(0x6A1EC0, CoreZlibLog_ConnectLogWindow, "auto");
