#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

static auto const g_TargetHandleTable = CSF_VTBL(0x0048C9F0);

static void __fastcall S_0048c9f0_UpdateTargetPosition(
    S_0048c9f0* self, void* /*edx*/)
{
    if ((self->field_0039 & 1) == 0 ||
        (self->field_0010 & 0x80000000) != 0)
        return;

    alignas(8) uint8_t temporary[0xC];

    void* target = reinterpret_cast<void*>(CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 8)));

    if (target)
    {
        uint32_t* position = GetObjectPositionVector(target, nullptr);
        CSF_AT(uint32_t, self, 0x14) = position[0];
        CSF_AT(uint32_t, self, 0x18) = position[1];
        CSF_AT(uint32_t, self, 0x1C) = position[2];

        if ((CSF_AT(uint32_t, target, 0x170) & 0x2000) != 0)
        {
            position =
                reinterpret_cast<uint32_t* (__fastcall*)(
                    void*, void*, void*)>(
                    CSF_AT(void**, target, 0)[0xA4 / 4])(
                        target, nullptr, temporary);

            CSF_AT(uint32_t, self, 0x14) = position[0];
            CSF_AT(uint32_t, self, 0x18) = position[1];
            CSF_AT(uint32_t, self, 0x1C) = position[2];
        }
    }
    else
    {
        CSF_AT(void**, temporary, 0) = CSF_VTBL(0x0082D448);
        CSF_AT(int32_t, temporary, 4) = -1;
        CSF_AT(int32_t, temporary, 8) = -1;
        AiAttackTasks_CopyTargetHandle(
            self, nullptr, reinterpret_cast<int>(temporary));
    }
}

HOOK(0x0048D050, S_0048c9f0_UpdateTargetPosition, "auto");
