#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxScene_SetClumpState, 0x006BC970,
         (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(char, __fastcall, C_008656d0_CaptureHandleValueIfFlagged, 0x006C5960, (C_008656d0* self, void* , C_00862490* a2));
CSF_FUNC(char, __fastcall, C_008656d0_ApplyEntryParameters, 0x006C63D0, (C_008656d0* self, void* , int entryAddress));
CSF_FUNC(void**, __fastcall, S_006cc4f0_RefreshState, 0x006CC4F0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_006cc4f0_ProcessEntryVector, 0x006CC830,
         (void* self, void* edx, uint32_t* a2));

[[maybe_unused]] static const auto kSceneStateVtable = CSF_VTBL(0x006CC4F0);

static char __fastcall S_006cc4f0_ApplySceneState(
    S_006cc4f0* self, void* /*edx*/, char state)
{
    const uint32_t nextRevision = self->field_00f4 + 1;
    self->field_0094 = state;
    self->field_00f4 = nextRevision;
    S_006cc4f0_RefreshState(self, nullptr);

    const uint32_t handle = self->field_0064;
    if (handle &&
        !CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00bc), 0x34))
    {
        auto* manager =
            reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760());
        C_008656d0_CaptureHandleValueIfFlagged(
            manager, nullptr, reinterpret_cast<C_00862490*>(handle));
    }

    const int entryAddress = static_cast<int>(self->field_0070);
    auto* manager =
        reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760());
    C_008656d0_ApplyEntryParameters(manager, nullptr, entryAddress);

    self->field_00d0 = 0;
    self->field_00e8 = 0;
    S_006cc4f0_ProcessEntryVector(
        self, nullptr,
        reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x14));

    auto* vectorBytes = reinterpret_cast<uint8_t*>(self) + 0x24;
    int remaining = 3;
    do
    {
        S_006cc4f0_ProcessEntryVector(
            self, nullptr, reinterpret_cast<uint32_t*>(vectorBytes));
        vectorBytes += 0x10;
        --remaining;
    } while (remaining);

    if (!self->field_006b)
    {
        void* clump = reinterpret_cast<void*>(self->field_0008);
        if (clump)
            GfxScene_SetClumpState(clump, nullptr, 1);

        uint32_t index = 0;
        if (self->field_00a8)
        {
            do
            {
                void* child = reinterpret_cast<void*>(
                    CSF_AT(uint32_t,
                           reinterpret_cast<void*>(self->field_00a0),
                           4 * index));
                if (child)
                {
                    void* childClump =
                        reinterpret_cast<void*>(CSF_AT(uint32_t, child, 0x38));
                    if (childClump)
                        GfxScene_SetClumpState(childClump, nullptr, 0);
                }

                const uint32_t count = self->field_00a8;
                ++index;
                if (index >= count)
                    break;
            } while (true);
        }
    }

    void* parentEntry = reinterpret_cast<void*>(self->field_00bc);
    if (parentEntry)
    {
        const int parentParameters = CSF_AT(int, parentEntry, 0x4C);
        auto* parentManager =
            reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760());
        C_008656d0_ApplyEntryParameters(
            parentManager, nullptr, parentParameters);
    }

    return 1;
}

HOOK(0x006CCFF0, S_006cc4f0_ApplySceneState, "auto");
