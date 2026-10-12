#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Message/event block passed by address to the handler stored at character+0x4C.
struct S_ModeEvent
{
    uint8_t handled;   // +0x00
    uint8_t pad_01[3];
    int32_t type;      // +0x04
    int32_t param0;    // +0x08
    int32_t param1;    // +0x0C
    int32_t param2;    // +0x10
};

CSF_FUNC(void, __fastcall, EventTarget_SendModeEvent, 0x0040FC50, (void* self, void* edx, S_ModeEvent* event));
CSF_FUNC(char, __fastcall, SetMenuContextState, 0x0049A0E0, (void* self, void* edx, int a4, char a5));
CSF_FUNC(int, __fastcall, ObjectProvider_SetUpdateMode, 0x004F7810, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __fastcall Character_DisableSpecialMode(void* self, void* /*edx*/)
{
    int flags = CSF_AT(int, self, 0x2E0);
    if ((flags & 0x1000) != 0)
    {
        void* eventTarget = CSF_AT(void*, self, 0x4C);
        CSF_AT(int, self, 0x2E0) = flags & 0xFFFFEFFF;

        S_ModeEvent event;
        event.param0 = 0;
        event.param1 = 0;
        event.param2 = 0;
        event.handled = 0;
        event.type = 1;
        EventTarget_SendModeEvent(eventTarget, nullptr, &event);
        event.type = 0;
        EventTarget_SendModeEvent(eventTarget, nullptr, &event);

        SetMenuContextState(self, nullptr, 0, 0);

        void* manager = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
        void* firstElement = reinterpret_cast<void*>(GetFirstElement(manager, nullptr));
        void* provider = GetCachedObjectProvider(firstElement, nullptr);
        ObjectProvider_SetUpdateMode(provider, nullptr, 0);

        flags = CSF_AT(int, self, 0x2E0);
        if ((flags & 0x2000) != 0)
            CSF_AT(uint8_t, self, 0x3A5) = 1;
    }
    return flags;
}

HOOK(0x0049A970, Character_DisableSpecialMode, "auto");
