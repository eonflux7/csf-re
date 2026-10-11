#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0082dd78_ConfigureChildAndPosition, 0x0041D180, (C_0082dd78* self, void* edx));
CSF_FUNC(bool, __fastcall, TaskHelper_CanUseSlot, 0x00489C30, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, TaskHelper_StartAction, 0x0048A4D0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint8_t, __fastcall, TaskHelper_Fallback, 0x004DAC20, (void* self, void* edx, int a2));

static uint8_t __fastcall C_0083dd58_StartOrFallback(C_0083dd58* self, void* /*edx*/)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0014);
    void* target = CSF_AT(void*, owner, 0x44);

    void** targetVtbl = CSF_AT(void**, target, 0);
    int slot = ((int(__fastcall*)(void*, void*, int))targetVtbl[0xBC / 4])(target, nullptr, 12);

    if (slot == -1 || !TaskHelper_CanUseSlot(target, nullptr, slot)) {
        return TaskHelper_Fallback(self, nullptr, 5);
    }

    uint8_t* owner2 = reinterpret_cast<uint8_t*>(self->field_0014);
    C_0082dd78_ConfigureChildAndPosition(CSF_AT(C_0082dd78*, owner2, 0x4C), nullptr);

    alignas(8) uint8_t paramBytes[0x24];
    CSF_AT(uint8_t, paramBytes, 0x20) = 1;
    CSF_AT(uint8_t, paramBytes, 0x22) = 1;
    CSF_AT(int, paramBytes, 0x0C) = -1;
    CSF_AT(int, paramBytes, 0x10) = -1;
    CSF_AT(void*, paramBytes, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(int, paramBytes, 0x14) = 0;
    CSF_AT(int, paramBytes, 0x18) = 0;
    CSF_AT(int, paramBytes, 0x1C) = 0;
    CSF_AT(uint8_t, paramBytes, 0x21) = 0;
    CSF_AT(int, paramBytes, 0x00) = 0;
    CSF_AT(int, paramBytes, 0x04) = 0;

    return static_cast<uint8_t>(reinterpret_cast<uintptr_t>(
        TaskHelper_StartAction(target, nullptr, slot, reinterpret_cast<int>(paramBytes))));
}

HOOK(0x004DAAE0, C_0083dd58_StartOrFallback, "auto");
