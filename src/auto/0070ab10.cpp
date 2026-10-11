#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, Water_CopyPosition, 0x004A9ED0,
         (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, Water_UpdateBase, 0x006FF9F0,
         (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, Water_GrowEntries, 0x007003B0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, RpTeamPlayerSetActive, 0x007FEE10,
         (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, _rpTeamPlayerSelect, 0x007FEE60,
         (uint32_t* a1, int a2));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_ValueScale, 0x0082D4EC);
CSF_GLOBAL(float, g_ContextualDistanceScale, 0x0082DD74);
CSF_GLOBAL(float, g_WaterSplitMagnitudeSquared, 0x00863674);

static bool Water_IsPlayerSelected(void* player)
{
    if (!CSF_AT(uint32_t, player, 0x24))
        return false;

    const uint16_t index = CSF_AT(uint16_t, player, 0x20);
    void* team = reinterpret_cast<void*>(CSF_AT(uint32_t, player, 0x24));
    void* selection = reinterpret_cast<void*>(CSF_AT(uint32_t, team, 0x60));
    return (CSF_AT(uint32_t, selection, 4 * (index >> 5)) &
            (uint32_t(1) << (index & 0x1F))) != 0;
}

static uint32_t* Water_GetNodePosition(C_00863600* self, uint32_t nodeOffset)
{
    void* player = reinterpret_cast<void*>(self->field_0004);
    void* nodes = reinterpret_cast<void*>(CSF_AT(uint32_t, player, 0x10));
    void* node = reinterpret_cast<void*>(CSF_AT(uint32_t, nodes, nodeOffset));
    void* owner = reinterpret_cast<void*>(CSF_AT(uint32_t, node, 0));
    void* matrices = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x10));
    const uint32_t base = CSF_AT(uint32_t, matrices, 0);
    const uint32_t index = CSF_AT(uint32_t, node, 4);
    return reinterpret_cast<uint32_t*>(base + (index << 6) + 48);
}

static void Water_AppendEntry(C_00863600* self, const uint8_t* entry)
{
    if (self->field_001c >= self->field_0018)
    {
        Water_GrowEntries(reinterpret_cast<uint8_t*>(self) + 0x14,
                          nullptr, static_cast<int>(self->field_0020));
    }

    uint8_t* destination = reinterpret_cast<uint8_t*>(self->field_0014) +
                           32 * self->field_001c;
    ++self->field_001c;
    std::memcpy(destination, entry, 0x20);
}

static char __fastcall C_00863600_SplitAndUpdateWater(
    C_00863600* self, void* /*edx*/, float deltaTime)
{
    const char ready = self->vSlot28();
    if (!ready)
        return ready;

    void* player = reinterpret_cast<void*>(self->field_0004);
    void* nodes = reinterpret_cast<void*>(CSF_AT(uint32_t, player, 0x10));
    void* matrix = reinterpret_cast<uint8_t*>(
        CSF_AT(uint32_t, nodes, 8)) + 48;
    void* transform = reinterpret_cast<void*>(self->field_0030);
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, transform, 0)[0x14 / 4])(transform, nullptr, matrix);

    if (!self->field_001c)
        return Water_UpdateBase(self, nullptr, deltaTime);

    if (!Water_IsPlayerSelected(reinterpret_cast<void*>(self->field_0004)))
    {
        player = reinterpret_cast<void*>(self->field_0004);
        _rpTeamPlayerSelect(
            reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, player, 0x24)),
            static_cast<int>(self->field_0004));

        if (!Water_IsPlayerSelected(reinterpret_cast<void*>(self->field_0004)))
            return 0;

        player = reinterpret_cast<void*>(self->field_0004);
        RpTeamPlayerSetActive(
            static_cast<int>(CSF_AT(uint32_t, player, 0x24)),
            static_cast<int>(self->field_0004), 1);
    }

    // Reserve the original frame's room for the position-copy callee.
    alignas(8) uint8_t positionBytes[0xB0];
    alignas(8) uint8_t entryBytes[0x20] = {};

    uint32_t offset = 0;
    for (uint32_t index = 0; index < self->field_001c; ++index, offset += 32)
    {
        uint8_t* entry = reinterpret_cast<uint8_t*>(self->field_0014) + offset;
        const float x = CSF_AT(float, entry, 0);
        const float y = CSF_AT(float, entry, 4);
        const float z = CSF_AT(float, entry, 8);
        if (!(x * x + y * y + z * z > g_WaterSplitMagnitudeSquared))
            continue;

        CSF_AT(uint32_t, entryBytes, 0x1C) = 0;
        CSF_AT(float, entryBytes, 0) =
            CSF_AT(float, entry, 0) * g_ValueScale;
        CSF_AT(float, entryBytes, 4) =
            CSF_AT(float, entry, 4) * g_ValueScale;
        CSF_AT(float, entryBytes, 8) =
            CSF_AT(float, entry, 8) * g_ValueScale;

        CSF_AT(float, entry, 0) *= g_HalfDurationScale;
        CSF_AT(float, entry, 4) *= g_HalfDurationScale;
        CSF_AT(float, entry, 8) *= g_HalfDurationScale;

        entry = reinterpret_cast<uint8_t*>(self->field_0014) + offset;
        std::memcpy(entryBytes + 0xC, entry + 0xC, 0xC);
        entry = reinterpret_cast<uint8_t*>(self->field_0014) + offset;
        CSF_AT(uint8_t, entryBytes, 0x18) = CSF_AT(uint8_t, entry, 0x18);
        const uint32_t kind = CSF_AT(uint32_t, entry, 0x1C);

        uint32_t nodeOffset;
        uint32_t newKind;
        switch (kind)
        {
        case 0:
        {
            CSF_AT(float, entryBytes, 0) *= g_ContextualDistanceScale;
            CSF_AT(float, entryBytes, 4) *= g_ContextualDistanceScale;
            CSF_AT(float, entryBytes, 8) *= g_ContextualDistanceScale;

            uint32_t* source = Water_GetNodePosition(self, 0x10);
            CSF_AT(uint32_t, entryBytes, 0x1C) = 1;
            uint32_t* position =
                Water_CopyPosition(positionBytes, nullptr, source);
            std::memcpy(entryBytes + 0xC, position, 0xC);
            Water_AppendEntry(self, entryBytes);

            source = Water_GetNodePosition(self, 0x40);
            CSF_AT(uint32_t, entryBytes, 0x1C) = 5;
            position = Water_CopyPosition(positionBytes, nullptr, source);
            std::memcpy(entryBytes + 0xC, position, 0xC);
            Water_AppendEntry(self, entryBytes);

            nodeOffset = 0x4C;
            newKind = 6;
            break;
        }
        case 1:
        case 2:
        case 3:
        case 4:
            nodeOffset = 0x10;
            newKind = 1;
            break;
        case 5:
        case 6:
            nodeOffset = 4;
            newKind = 0;
            break;
        case 7:
            nodeOffset = 0x28;
            newKind = 3;
            break;
        case 8:
            nodeOffset = 0x34;
            newKind = 4;
            break;
        case 9:
            nodeOffset = 0x40;
            newKind = 5;
            break;
        case 10:
            nodeOffset = 0x4C;
            newKind = 6;
            break;
        default:
            continue;
        }

        uint32_t* source = Water_GetNodePosition(self, nodeOffset);
        CSF_AT(uint32_t, entryBytes, 0x1C) = newKind;
        uint32_t* position = Water_CopyPosition(positionBytes, nullptr, source);
        std::memcpy(entryBytes + 0xC, position, 0xC);
        Water_AppendEntry(self, entryBytes);
    }

    return Water_UpdateBase(self, nullptr, deltaTime);
}

HOOK(0x0070AB10, C_00863600_SplitAndUpdateWater, "auto");
