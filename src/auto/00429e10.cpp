#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));
CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720, (void* returnSlot, const char* text, int count));
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));

static S_004f21d0* __cdecl Cheats_GetEntryText(int entry, S_004f21d0* output)
{
    void* context = reinterpret_cast<void*>(GetGlobalValue_8B056C());
    void* modeProvider = CSF_AT(void*, context, 0x1C);
    int mode = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, modeProvider, 0)[8 / 4])(modeProvider, nullptr);

    const char* literal = nullptr;
    int messageCode = 0;
    int messageFlag = 0;

    if (mode == 0)
    {
        switch (entry)
        {
        case 0: literal = CSF_STR(0x0083015C, "c002"); break;
        case 1: literal = CSF_STR(0x00830154, "c003"); break;
        case 2: literal = CSF_STR(0x0083014C, "c004"); break;
        case 3: literal = CSF_STR(0x00830144, "c005"); break;
        case 4: literal = CSF_STR(0x0083013C, "c006"); break;
        case 5: literal = CSF_STR(0x00830134, "c007"); break;
        case 6: literal = CSF_STR(0x0083012C, "c008"); break;
        case 7: literal = CSF_STR(0x00830124, "c009"); break;
        case 10: literal = CSF_STR(0x0083011C, "c012"); break;
        case 11: literal = CSF_STR(0x00830184, "c013"); break;
        case 12: literal = CSF_STR(0x0083016C, "c014"); break;
        case 13: literal = CSF_STR(0x00830174, "c015"); break;
        case 14: literal = CSF_STR(0x0083017C, "c016"); break;
        case 16: literal = CSF_STR(0x00830114, "c018"); break;
        case 22: literal = CSF_STR(0x0083018C, "c024"); break;
        default: return output;
        }
    }
    else if (mode == 1)
    {
        switch (entry)
        {
        case 0:
        case 1:
        case 2:
        case 3:
            literal = CSF_STR(0x0083019C, "c100");
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            literal = CSF_STR(0x00830194, "c101");
            break;
        case 10: messageCode = 1882272051; messageFlag = 64; break;
        case 11: messageCode = 1882272052; messageFlag = 64; break;
        case 12: messageCode = 1882272055; messageFlag = 64; break;
        case 13: messageCode = 1882272054; messageFlag = 64; break;
        case 14: messageCode = 1882272053; messageFlag = 64; break;
        case 16: messageCode = 1882272056; messageFlag = 64; break;
        case 22: literal = CSF_STR(0x00830164, "c110"); break;
        default: return output;
        }
    }
    else if (mode == 2)
    {
        switch (entry)
        {
        case 0:
        case 1:
        case 2:
        case 3:
            literal = CSF_STR(0x0083019C, "c100");
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            literal = CSF_STR(0x00830194, "c101");
            break;
        case 10: messageCode = 2019703090; messageFlag = 128; break;
        case 11: literal = CSF_STR(0x00830184, "c013"); break;
        case 12: literal = CSF_STR(0x0083016C, "c014"); break;
        case 13: literal = CSF_STR(0x00830174, "c015"); break;
        case 14: literal = CSF_STR(0x0083017C, "c016"); break;
        case 16: messageCode = 2019703091; messageFlag = 128; break;
        case 22: literal = CSF_STR(0x0083018C, "c024"); break;
        default: return output;
        }
    }
    else
    {
        return output;
    }

    if (literal)
        return static_cast<S_004f21d0*>(InitStringFromLiteral(output, literal, 1));

    auto* text = reinterpret_cast<std::int16_t*>(
        MenuMessageBox_ResolveEntryByKey(messageCode, messageFlag));
    MenuProgressInput_AssignText(
        reinterpret_cast<std::uint32_t*>(output), nullptr, text);
    return output;
}

HOOK(0x00429E10, Cheats_GetEntryText, "auto");
