#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callees bound by address.
CSF_FUNC(int, __cdecl, Legacy_ForwardKeyCode, 0x00428480, (int keyCode));
CSF_FUNC(void*, __fastcall, MenuSoundOptions_GetChildObject, 0x006683F0, (void* self, void* edx));

// Original: int __thiscall sub_53CBC0(this, a2, a3); ret 8.
// Behaviour:
//   - key 1 or 14: invoke vtable slot 19 (+0x4C) on the object at this-4, then forward the key.
//   - key 50 and byte [a3+0x38] != 0: fetch child object, invoke its vtable slot 7 (+0x1C),
//     then forward the key.
//   - otherwise: forward the key only.
static int __fastcall MenuSoundOptions_HandleKey(void* self, void* /*edx*/, int keyCode, int eventData)
{
    if (keyCode == 1 || keyCode == 14)
    {
        // Parent-embedded object at this-4 (the original does `add ecx, -4`).
        void* parent = reinterpret_cast<uint8_t*>(self) - 4;
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, parent, 0)[76 / 4])(parent, nullptr);
    }
    else if (keyCode == 50 && CSF_AT(uint8_t, reinterpret_cast<void*>(eventData), 0x38) != 0)
    {
        void* child = MenuSoundOptions_GetChildObject(self, nullptr);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[28 / 4])(child, nullptr);
        return Legacy_ForwardKeyCode(keyCode);
    }
    return Legacy_ForwardKeyCode(keyCode);
}

HOOK(0x0053CBC0, MenuSoundOptions_HandleKey, "auto");
