#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiCore_InitializeQuery, 0x0044E030, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, IsDistinctNonSentinelValue, 0x0048E0D0, (void* self, void* edx, std::int32_t candidate));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, AiQuery_IsHandleExcluded, 0x004E5B40, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_0083eb70_QueueVisibleTargets(C_0083eb70* self, void* /*edx*/)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);
    uint8_t* inner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x38));
    if (!inner)
        return static_cast<int>(reinterpret_cast<uintptr_t>(owner));

    int busy = reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, inner, 0)[0x7C / 4])(inner, nullptr);
    if (busy)
        return busy;

    owner = reinterpret_cast<uint8_t*>(self->field_0008);
    inner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x38));
    uint32_t count = CSF_AT(uint32_t, inner, 0x3C);
    uint8_t* listField = inner + 0x34;
    uint32_t remaining = count;
    if (count == 0)
        return static_cast<int>(reinterpret_cast<uintptr_t>(owner));

    alignas(8) uint8_t queryBytes[0x3C];
    AiCore_InitializeQuery(queryBytes, nullptr);
    CSF_AT(int, queryBytes, 0x4) = 3;
    CSF_AT(int, queryBytes, 0x8) = 14;

    uint32_t offset = 0;
    do {
        int handle = static_cast<int>(CSF_AT(uint32_t, listField, 0) + offset);
        uint8_t* obj = reinterpret_cast<uint8_t*>(CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
        if (obj
            && (IsDistinctNonSentinelValue(reinterpret_cast<void*>(self->field_0008), nullptr,
                                           static_cast<std::int32_t>(CSF_AT(uint32_t, obj, 0x78))) & 0xFF)
            && !AiQuery_IsHandleExcluded(self, nullptr, static_cast<int>(CSF_AT(uint32_t, listField, 0) + offset)))
        {
            uint8_t* scene = reinterpret_cast<uint8_t*>(g_pCurrentScene);
            uint8_t* sub = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, scene, 0xB0));
            CSF_AT(uint32_t, queryBytes, 0x30) = CSF_AT(uint32_t, sub, 8);
            uint8_t* entry = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, listField, 0));
            CSF_AT(uint32_t, queryBytes, 0x18) = CSF_AT(uint32_t, entry, offset + 4);
            CSF_AT(uint32_t, queryBytes, 0x1C) = CSF_AT(uint32_t, entry, offset + 8);
            CSF_AT(int, queryBytes, 0xC) = 1;

            uint32_t* pos = reinterpret_cast<uint32_t*>(GetObjectPositionVector(obj, nullptr));
            CSF_AT(uint32_t, queryBytes, 0x20) = pos[0];
            CSF_AT(uint32_t, queryBytes, 0x24) = pos[1];
            CSF_AT(uint32_t, queryBytes, 0x28) = pos[2];

            float* objPos = GetObjectPositionVector(obj, nullptr);
            float* selfPos = GetObjectPositionVector(reinterpret_cast<void*>(self->field_0008), nullptr);
            float dx = selfPos[0] - objPos[0];
            float dy = selfPos[1] - objPos[1];
            float dz = selfPos[2] - objPos[2];
            CSF_AT(float, queryBytes, 0x2C) = dz * dz + dy * dy + dx * dx;

            self->vSlotB4(static_cast<int>(reinterpret_cast<uintptr_t>(queryBytes)));
        }
        offset += 12;
        --remaining;
    } while (remaining != 0);

    return 0;
}

HOOK(0x004E7890, C_0083eb70_QueueVisibleTargets, "auto");
