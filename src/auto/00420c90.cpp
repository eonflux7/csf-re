#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee bound by address (bind-as line from the packet).
CSF_FUNC(int, __fastcall, sub_4DB440, 0x004DB440, (void* self, void* edx, int a2));

// Original: int __thiscall sub_420C90(C_00834fa8* this, int* a2); ret 4
static int __fastcall C_00834fa8_HandleEventCode8(C_00834fa8* self, void* /*edx*/, int32_t* a2)
{
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(a2));
    if (*a2 == 8)
    {
        // Object held at this->field_0008 + 0x14 (a stored pointer).
        uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
        void* target = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x14));

        // Virtual slot 0x8C / 4 = 35, called with (0, 0, -1, 0).
        void** vtbl = CSF_AT(void**, target, 0);
        ((void(__fastcall*)(void*, void*, int, int, int, int))vtbl[0x8C / 4])(
            target, nullptr, 0, 0, -1, 0);

        int32_t rowAddress = static_cast<int32_t>(CSF_AT(uint32_t, self, 0x0C) + 0x20);
        return sub_4DB440(self, nullptr, rowAddress);
    }
    return result;
}

HOOK(0x00420C90, C_00834fa8_HandleEventCode8, "auto");
