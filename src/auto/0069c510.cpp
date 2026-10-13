#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __cdecl, Campaign_CheckScenarioFile, 0x00697720, (char* a1));

CSF_GLOBAL(int, g_Value_85E8D0, 0x0085E8D0);
CSF_GLOBAL(int, g_Value_85E8D4, 0x0085E8D4);
CSF_GLOBAL(uint16_t, g_Value_85E8D8, 0x0085E8D8);
CSF_GLOBAL(uint8_t, g_Value_85E8DA, 0x0085E8DA);
CSF_GLOBAL(int, g_Value_8BAF98, 0x008BAF98);
CSF_GLOBAL(int, g_Value_8BAFA4, 0x008BAFA4);
CSF_GLOBAL(int, g_Value_8BAFA8, 0x008BAFA8);
CSF_GLOBAL(uint16_t, g_Value_8BAFAC, 0x008BAFAC);
CSF_GLOBAL(uint8_t, g_Value_8BAFAE, 0x008BAFAE);

static int CoreWin32Debug_CopyScenarioPath(int a1)
{
    char* source = reinterpret_cast<char*>(a1);

    if (Campaign_CheckScenarioFile(source))
    {
        char* cursor = source;
        char ch;
        char* destination = reinterpret_cast<char*>(&g_Value_8BAFA4);
        do
        {
            ch = *cursor;
            destination[cursor - source] = ch;
            ++cursor;
        } while (ch);
        return static_cast<int>(reinterpret_cast<intptr_t>(cursor));
    }

    g_Value_8BAFA4 = g_Value_85E8D0;
    g_Value_8BAFA8 = g_Value_85E8D4;
    g_Value_8BAFAC = g_Value_85E8D8;
    g_Value_8BAFAE = g_Value_85E8DA;
    g_Value_8BAF98 = 0;
    return 1651469312;
}

HOOK(0x0069C510, CoreWin32Debug_CopyScenarioPath, "auto");
