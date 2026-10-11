#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Cheats_GetPrimaryMissionCode, 0x00429E10, (int a1, void* a2));
CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720, (void* returnSlot, const char* text, int count));
CSF_FUNC(int, __fastcall, SaveGame_ValidateName, 0x00696BB0, (void* name, void* edx, const uint8_t* separator));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static S_004f21d0* __cdecl Cheats_ResolveMissionCode(int missionId, S_004f21d0* output)
{
    void* primaryCode = Cheats_GetPrimaryMissionCode(missionId, output);
    if (static_cast<uint8_t>(SaveGame_ValidateName(primaryCode, nullptr, &g_byte_82E65C)))
        return output;

    const char* code;
    switch (missionId)
    {
    case 8:  code = CSF_STR(0x00830224, "c010"); break;
    case 9:  code = CSF_STR(0x0083021C, "c011"); break;
    case 15: code = CSF_STR(0x00830214, "c017"); break;
    case 17: code = CSF_STR(0x0083020C, "c019"); break;
    case 18: code = CSF_STR(0x00830204, "c020"); break;
    case 19: code = CSF_STR(0x008301FC, "c021"); break;
    case 20: code = CSF_STR(0x008301F4, "c022"); break;
    case 21: code = CSF_STR(0x008301EC, "c023"); break;
    case 23: code = CSF_STR(0x008301E4, "c025"); break;
    case 24: code = CSF_STR(0x008301DC, "c095"); break;
    case 25: code = CSF_STR(0x008301D4, "c103"); break;
    case 26: code = CSF_STR(0x008301CC, "c104"); break;
    case 27: code = CSF_STR(0x008301C4, "c108"); break;
    case 28: code = CSF_STR(0x008301BC, "c111"); break;
    case 29: code = CSF_STR(0x008301B4, "c116"); break;
    case 30: code = CSF_STR(0x008301AC, "c117"); break;
    case 31: code = CSF_STR(0x008301A4, "c118"); break;
    default: return output;
    }

    return static_cast<S_004f21d0*>(InitStringFromLiteral(output, code, 1));
}

HOOK(0x0042A1B0, Cheats_ResolveMissionCode, "auto");
