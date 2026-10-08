#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Menu event codes stored as 4-byte ASCII tags ("GE17" / "GE20", little-endian).
static constexpr uint32_t kMenuCodeGE17 = 0x47453137u;
static constexpr uint32_t kMenuCodeGE20 = 0x47453230u;

CSF_FUNC(void, __fastcall, MenuMpPause_InitMenuBuffer, 0x501870, (void* buffer, int a2));
CSF_FUNC(void*, __cdecl, GetGlobalDword_73C020, 0x73C020, ());
CSF_FUNC(uint8_t, __fastcall, MenuMpPause_QueryGlobalFlagIsOne, 0x73E040, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GetGlobalState, 0x50D190, ());
CSF_FUNC(void, __fastcall, MenuMpPause_PostMenuEvent, 0x50F790,
         (void* self, void* edx, uint32_t eventCode, int a3, int a4, uint32_t* buffer));
CSF_FUNC(uint32_t, __fastcall, MenuMpPause_DestroyMenuBuffer, 0x4FF3C0, (void* self, void* edx));

// Opens the multiplayer pause menu: builds the menu buffer, picks the event code
// depending on a global flag, posts the event and releases the buffer.
static unsigned int __fastcall MenuMpPause_OpenPauseDialog(void* self, int a2)
{
    CSF_AT(uint32_t, self, 0x14) = 0;

    uint32_t menuBuffer[45];
    MenuMpPause_InitMenuBuffer(menuBuffer, a2);
    menuBuffer[2] = 2;

    uint32_t eventCode = kMenuCodeGE20;
    void* globalRoot = GetGlobalDword_73C020();
    if (CSF_AT(uint8_t, globalRoot, 0xFC) != 0)
    {
        void* globalRootAgain = GetGlobalDword_73C020();
        if (MenuMpPause_QueryGlobalFlagIsOne(globalRootAgain, nullptr) == 1)
            eventCode = kMenuCodeGE17;
    }

    void* globalState = GetGlobalState();
    MenuMpPause_PostMenuEvent(globalState, nullptr, eventCode, 1, 3, menuBuffer);

    return MenuMpPause_DestroyMenuBuffer(menuBuffer, nullptr);
}

HOOK(0x005353E0, MenuMpPause_OpenPauseDialog, "auto");
