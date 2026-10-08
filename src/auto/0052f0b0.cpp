#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Default handler reached for every message that is not consumed here (cdecl, 1 arg).
CSF_FUNC(int, __cdecl, MenuBase_DefaultHandler, 0x00428480, (int message));

// Returns a linked sub-object of a menu (thiscall, no stack args).
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));

// Message code that triggers the linked sub-object refresh.
static const int kRefreshMessage = 50;
// Message code handled by forwarding to the object preceding this one in memory.
static const int kForwardMessage = 1;

// Vtable slots (byte offset / 4).
static const int kForwardedObjectSlot = 0x4C / 4;   // 19
static const int kLinkedObjectNotifySlot = 0x1C / 4; // 7

static int __fastcall MenuBase_OnCommand(void* self, void* /*edx*/, int message, int param)
{
    if (message == kForwardMessage)
    {
        // Dispatch to the enclosing (preceding-in-memory) object at self-4.
        void* forwarded = reinterpret_cast<uint8_t*>(self) - 4;
        void** vtbl = CSF_AT(void**, forwarded, 0);
        ((void(__fastcall*)(void*, void*))vtbl[kForwardedObjectSlot])(forwarded, nullptr);
    }
    else if (message == kRefreshMessage)
    {
        void* paramObj = reinterpret_cast<void*>(param);
        uint8_t flag = CSF_AT(uint8_t, paramObj, 0x38);
        if (flag != 0)
        {
            void* linked = GetReferencedObject(self, nullptr);
            void** linkedVtbl = CSF_AT(void**, linked, 0);
            ((void(__fastcall*)(void*, void*))linkedVtbl[kLinkedObjectNotifySlot])(linked, nullptr);
            return MenuBase_DefaultHandler(message);
        }
    }
    return MenuBase_DefaultHandler(message);
}

HOOK(0x0052F0B0, MenuBase_OnCommand, "auto");
