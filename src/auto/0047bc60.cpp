#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

static int __fastcall C_0082f9f8_ResetSubTaskAndStartMovement(C_0082f9f8* self, void* /*edx*/) {
    uint8_t* taskObj = reinterpret_cast<uint8_t*>(self->field_0008);
    uint8_t* subObj = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, taskObj, 0x14));
    uint8_t* innerObj = subObj + 4;

    using SlotFn = void(__fastcall*)(void*, void*);
    SlotFn slot5 = reinterpret_cast<SlotFn>(CSF_AT(void**, innerObj, 0)[5]);
    slot5(innerObj, nullptr);

    int movementArg = reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x24);
    return CharTask_StartMovement(self, nullptr, movementArg);
}

HOOK(0x0047BC60, C_0082f9f8_ResetSubTaskAndStartMovement, "auto");
