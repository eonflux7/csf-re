#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, C_00833708_SendOrder, 0x0048A4D0, (C_00833708* self, void* , int taskIndex, int orderAddress));
CSF_FUNC(bool, __fastcall, S_0048c6c0_CheckTarget, 0x0048C720, (S_0048c6c0* self, void* , int sourceHandle, int targetHandle));
CSF_FUNC(int, __fastcall, GetControlEntryByIndex, 0x0049ED10, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCore_UpdateTargetData, 0x004D1730, (S_0048c9f0* self, void* , int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_0083dd58_UpdateTargetData(
    C_0083dd58* self, void* /*edx*/, int targetAddress)
{
    bool useAlternateOrder = S_0048c6c0_CheckTarget(
        reinterpret_cast<S_0048c6c0*>(reinterpret_cast<uint8_t*>(self) + 0x40),
        nullptr,
        static_cast<int>(self->field_0014 + 28),
        targetAddress);

    if (!useAlternateOrder)
    {
        void* scene = g_pCurrentScene;
        int controlHandle = GetControlEntryByIndex(
            reinterpret_cast<void*>(self->field_0014), nullptr, 0);
        void* control = reinterpret_cast<void*>(
            CHandleTable_Resolve(scene, nullptr, controlHandle));
        void* task = CSF_AT(void*, control, 0x34);
        useAlternateOrder = task &&
            !reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, task, 0)[0xB4 / 4])(
                    task, nullptr, targetAddress);
    }

    AiCore_UpdateTargetData(
        reinterpret_cast<S_0048c9f0*>(reinterpret_cast<uint8_t*>(self) + 0x1C),
        nullptr, targetAddress);

    void* target = reinterpret_cast<void*>(targetAddress);
    uint32_t targetFirst = CSF_AT(uint32_t, target, 0x4);
    uint32_t targetSecond = CSF_AT(uint32_t, target, 0x8);
    void* owner = reinterpret_cast<void*>(self->field_0014);

    alignas(8) uint8_t orderBytes[0x24];
    CSF_AT(uint32_t, orderBytes, 0x00) = 0;
    CSF_AT(uint32_t, orderBytes, 0x04) = useAlternateOrder + 1;
    CSF_AT(void**, orderBytes, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, orderBytes, 0x0C) = targetFirst;
    CSF_AT(uint32_t, orderBytes, 0x10) = targetSecond;
    CSF_AT(uint32_t, orderBytes, 0x14) = 0;
    CSF_AT(uint32_t, orderBytes, 0x18) = 0;
    CSF_AT(uint32_t, orderBytes, 0x1C) = 0;
    CSF_AT(uint8_t, orderBytes, 0x20) = 1;
    CSF_AT(uint8_t, orderBytes, 0x21) = 0;
    CSF_AT(uint8_t, orderBytes, 0x22) = 1;

    return reinterpret_cast<int>(C_00833708_SendOrder(
        CSF_AT(C_00833708*, owner, 0x44), nullptr, 0,
        reinterpret_cast<int>(orderBytes)));
}

HOOK(0x004D5780, C_0083dd58_UpdateTargetData, "auto");
