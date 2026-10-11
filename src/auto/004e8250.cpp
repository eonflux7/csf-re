#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Perception_CheckFirst, 0x004E6E80, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Perception_CheckThird, 0x004E6F30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Perception_CheckFourth, 0x004E6FD0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Perception_CheckFifth, 0x004E7030, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, Perception_IsIgnored, 0x004E73F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Perception_CheckSecond, 0x004E7ED0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, Perception_FinalCheck, 0x004E7F10, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));

static int __fastcall C_0083eb70_EvaluateTarget(C_0083eb70* self, void* /*edx*/, int a2, int a3)
{
    uint8_t* out = reinterpret_cast<uint8_t*>(a3);
    uint8_t* in = reinterpret_cast<uint8_t*>(a2);

    CSF_AT(int, out, 4) = -1;
    if (Perception_IsIgnored(self, nullptr, a2))
        return CSF_AT(int, out, 4);

    uint8_t* obj = reinterpret_cast<uint8_t*>(CHandleTable_Resolve(g_pCurrentScene, nullptr, a2));
    if (!obj)
        return CSF_AT(int, out, 4);

    CSF_AT(uint32_t, out, 0x18) = CSF_AT(uint32_t, in, 4);
    CSF_AT(uint32_t, out, 0x1C) = CSF_AT(uint32_t, in, 8);

    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);
    if (CSF_AT(int, owner, 0x50) != 0) {
        // The original tests only AL of each check's result.
        if (!(uint8_t)Perception_CheckFirst(self, nullptr, a3)
            && !(uint8_t)Perception_CheckSecond(self, nullptr, a3)
            && !(uint8_t)Perception_CheckThird(self, nullptr, a3)
            && !(uint8_t)Perception_CheckFourth(self, nullptr, a3)) {
            Perception_CheckFifth(self, nullptr, a3);
        }
    } else {
        bool flagged = (CSF_AT(uint32_t, obj, 0x170) & 0x2000) != 0;
        if (!flagged)
            flagged = (uint8_t)GameConfig_IsResolvedEntryActive(CSF_AT(int, obj, 0x50)) != 0;
        if (flagged) {
            CSF_AT(int, out, 4) = 3;
            CSF_AT(int, out, 8) = 14;
        }
    }

    if (CSF_AT(int, out, 4) != -1) {
        float* pos = GetObjectPositionVector(obj, nullptr);
        CSF_AT(uint32_t, out, 0x20) = reinterpret_cast<uint32_t*>(pos)[0];
        CSF_AT(uint32_t, out, 0x24) = reinterpret_cast<uint32_t*>(pos)[1];
        CSF_AT(uint32_t, out, 0x28) = reinterpret_cast<uint32_t*>(pos)[2];

        float* objPos = GetObjectPositionVector(obj, nullptr);
        float* ownerPos = GetObjectPositionVector(reinterpret_cast<void*>(self->field_0008), nullptr);
        float dx = ownerPos[0] - objPos[0];
        float dy = ownerPos[1] - objPos[1];
        float dz = ownerPos[2] - objPos[2];
        CSF_AT(float, out, 0x2C) = dz * dz + dx * dx + dy * dy;

        if (!Perception_FinalCheck(self, nullptr, a3))
            CSF_AT(int, out, 4) = -1;
    }
    return CSF_AT(int, out, 4);
}

HOOK(0x004E8250, C_0083eb70_EvaluateTarget, "auto");
