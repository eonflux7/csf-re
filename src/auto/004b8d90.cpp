#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCombat_BindCombatTarget, 0x004B2190, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, PairLink_PrepareForRebind, 0x004E55A0, (C_0083e9d0* self, void*));
CSF_FUNC(char, __fastcall, DecrementByteCounter, 0x004E55B0, (void* self, void*));

static int __fastcall S_004ad990_ApplyCombatOrder(
    S_004ad990* self, void* /*edx*/, int* order)
{
    if (order[7] < 0)
    {
        self->field_00a4 = 0;
        AiCombat_BindCombatTarget(
            reinterpret_cast<std::uint8_t*>(self) + 0x28,
            nullptr, order[2], reinterpret_cast<int>(order + 5));

        std::memcpy(&self->field_0040, &order[8], sizeof(float));
        std::memcpy(&self->field_0044, &order[9], sizeof(float));
        std::memcpy(&self->field_0048, &order[10], sizeof(float));

        self->field_0050 = -1;
        self->field_0054 = -1;
        self->field_005c = 0;

        std::memcpy(&CSF_AT(float, self, 0x70),
                    &self->field_0040, sizeof(float));
        std::memcpy(&CSF_AT(float, self, 0x74),
                    &self->field_0044, sizeof(float));
        std::memcpy(&CSF_AT(float, self, 0x78),
                    &self->field_0048, sizeof(float));

        self->field_0060 = 1065353216;
        CSF_AT(int, self, 0x64) = order[8];
        CSF_AT(int, self, 0x68) = order[9];
        const int result = order[10];
        CSF_AT(int, self, 0x6C) = result;

        std::memcpy(&self->field_0098, &order[11], sizeof(float));
        self->field_009c = 0.0f;
        self->field_00a9 = 0;
        self->field_00a8 = 0;
        return result;
    }

    const int resolved = CHandleTable_Resolve(
        g_pCurrentScene, nullptr, reinterpret_cast<int>(order + 5));
    if (!resolved)
        return resolved;

    if (self->field_0038 != static_cast<std::uint32_t>(order[6]))
        self->field_00a4 = 0;

    PairLink_PrepareForRebind(
        reinterpret_cast<C_0083e9d0*>(
            CSF_AT(std::uint32_t,
                   reinterpret_cast<void*>(self->field_0000), 0x3C)),
        nullptr);
    DecrementByteCounter(
        reinterpret_cast<void*>(
            CSF_AT(std::uint32_t,
                   reinterpret_cast<void*>(self->field_0000), 0x3C)),
        nullptr);

    AiCombat_BindCombatTarget(
        reinterpret_cast<std::uint8_t*>(self) + 0x28,
        nullptr, order[2], reinterpret_cast<int>(order + 5));

    self->field_0050 = -1;
    self->field_0054 = -1;
    int* position = GetObjectPositionVector(
        reinterpret_cast<void*>(self->field_0000), nullptr);

    self->field_005c = 0;
    std::memcpy(&CSF_AT(float, self, 0x70),
                &self->field_0040, sizeof(float));
    std::memcpy(&CSF_AT(float, self, 0x74),
                &self->field_0044, sizeof(float));
    std::memcpy(&CSF_AT(float, self, 0x78),
                &self->field_0048, sizeof(float));

    self->field_0060 = 1065353216;
    CSF_AT(int, self, 0x64) = position[0];
    CSF_AT(int, self, 0x68) = position[1];
    CSF_AT(int, self, 0x6C) = position[2];

    std::memcpy(&self->field_0098, &order[11], sizeof(float));
    self->field_009c = 0.0f;
    self->field_00a9 = 0;
    self->field_00a8 = 1;
    return 0;
}

HOOK(0x004B8D90, S_004ad990_ApplyCombatOrder, "auto");
