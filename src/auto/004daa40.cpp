#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AutoTask_SendOrder, 0x0048A4D0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(bool, __fastcall, AutoTask_CheckTarget, 0x0048C720, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, AiCore_UpdateTargetData, 0x004D1730, (void* self, void* edx, int a2));

static uint32_t* __fastcall C_0083dd58_SendTargetOrder(C_0083dd58* self, void* /*edx*/, int targetData)
{
    alignas(8) uint8_t orderBytes[0x24];

    CSF_AT(int, orderBytes, 0x00) = 0;
    CSF_AT(int, orderBytes, 0x04) = 0;
    CSF_AT(int, orderBytes, 0x14) = 0;
    CSF_AT(int, orderBytes, 0x18) = 0;
    CSF_AT(int, orderBytes, 0x1C) = 0;
    CSF_AT(uint8_t, orderBytes, 0x21) = 0;

    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0014);
    CSF_AT(void*, orderBytes, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(int, orderBytes, 0x0C) = -1;
    CSF_AT(int, orderBytes, 0x10) = -1;
    CSF_AT(uint8_t, orderBytes, 0x20) = 1;
    CSF_AT(uint8_t, orderBytes, 0x22) = 1;

    void* manager = CSF_AT(void*, owner, 0x44);
    int slot = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, manager, 0)[0xBC / 4])(manager, nullptr, 30);

    int result = AiCore_UpdateTargetData(&self->field_001c, nullptr, targetData);

    if (slot != -1) {
        owner = reinterpret_cast<uint8_t*>(self->field_0014);
        bool check = AutoTask_CheckTarget(reinterpret_cast<uint8_t*>(self) + 0x40, nullptr,
                                          (int)(uintptr_t)(owner + 0x1C), targetData);

        uint8_t* data = reinterpret_cast<uint8_t*>((uintptr_t)targetData);
        int x = CSF_AT(int, data, 4);
        int y = CSF_AT(int, data, 8);
        CSF_AT(int, orderBytes, 0x0C) = x;
        owner = reinterpret_cast<uint8_t*>(self->field_0014);
        CSF_AT(int, orderBytes, 0x10) = y;
        CSF_AT(uint8_t, orderBytes, 0x20) = 1;
        CSF_AT(int, orderBytes, 0x04) = (check ? 1 : 0) + 1;

        return AutoTask_SendOrder(CSF_AT(void*, owner, 0x44), nullptr, slot, (int)(uintptr_t)orderBytes);
    }
    return reinterpret_cast<uint32_t*>((uintptr_t)result);
}

HOOK(0x004DAA40, C_0083dd58_SendTargetOrder, "auto");
