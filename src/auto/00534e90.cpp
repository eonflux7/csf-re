#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Forwards an unhandled menu message to the base handler (cdecl, one argument).
CSF_FUNC(int, __cdecl, ForwardUnhandledMenuMessage, 0x00428480, (int messageId));

// Applies a command id to the pause menu. __thiscall, one argument, callee pops 4.
CSF_FUNC(void, __fastcall, MenuMpPause_ProcessCommandId, 0x00534AE0, (void* self, void* edx, int commandId));

// Returns the pause subsystem object for the menu (lib function, __thiscall, no args).
CSF_FUNC(void*, __fastcall, GetMenuPauseSubsystem, 0x006683F0, (void* self, void* edx));

// Handles a menu message for CMenuMpPause. __thiscall, two stack args, callee pops 8.
// messageId: message code (1 / 14 = command dispatch, 50 = subsystem notification).
// payload:   message parameter block; byte at +0x38 gates the subsystem notification.
static int __fastcall MenuMpPause_HandleMessage(void* self, void* /*edx*/, int messageId, void* payload)
{
    if (messageId == 1 || messageId == 14)
    {
        MenuMpPause_ProcessCommandId(static_cast<uint8_t*>(self) - 4, nullptr, 1);
    }
    else if (messageId == 50 && CSF_AT(uint8_t, payload, 0x38) != 0)
    {
        void* pauseSubsystem = GetMenuPauseSubsystem(self, nullptr);
        // Slot 0x1c / 4 = 7 of the subsystem vtable.
        void** vtable = CSF_AT(void**, pauseSubsystem, 0);
        ((void(__fastcall*)(void*, void*))vtable[7])(pauseSubsystem, nullptr);
        return ForwardUnhandledMenuMessage(messageId);
    }

    return ForwardUnhandledMenuMessage(messageId);
}

HOOK(0x00534E90, MenuMpPause_HandleMessage, "auto");
