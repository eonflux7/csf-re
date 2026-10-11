#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original: int __thiscall sub_4D72B0(C_0082f9f8* this, int a2, int a3), ret 8
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

static int __fastcall C_0082f9f8_HandleTaskEvent(C_0082f9f8* self, void* /*edx*/, int eventCode, int /*a3*/)
{
    if (eventCode == 0x12)
    {
        // self->field_0008 holds a pointer to the owning object; its +0x14 slot holds the target object.
        uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8));
        void* target = CSF_AT(void*, owner, 0x14);

        // Virtual call at vtable byte offset 0x8C (slot 35): (target, 52, 0, -1, 0)
        using SlotFn = void(__fastcall*)(void*, void*, int, int, int, int);
        SlotFn slotFn = reinterpret_cast<SlotFn>(CSF_AT(void**, target, 0)[0x8C / 4]);
        slotFn(target, nullptr, 52, 0, -1, 0);
    }

    return CharTask_StartMovement(self, nullptr, 0);
}

HOOK(0x4D72B0, C_0082f9f8_HandleTaskEvent, "auto");
