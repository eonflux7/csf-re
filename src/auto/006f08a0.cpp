#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpPTankAtomicLock, 0x005821D0, (int a1, uint32_t* a2, unsigned int a3, int a4));
CSF_FUNC(int, __cdecl, RpPTankAtomicUnlock, 0x005823E0, (int a1));
CSF_FUNC(void*, __fastcall, GfxPtank_GetPlacementState, 0x006F3FE0, (void* self, void* , void* outState));
CSF_FUNC(void, __fastcall, C_00862748_UpdateParticles_6F4590, 0x006F4590, (C_00862748* self, void*));

CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);
CSF_GLOBAL(std::int32_t, g_PTankPluginOffset, 0x008B09EC);

static int __fastcall C_008622e8_UpdateTranslatedAtomic(
    C_008622e8* self, void* /*edx*/)
{
    auto* emitter = reinterpret_cast<C_00862748*>(self->field_0020);
    if (!self->field_0024)
    {
        return ((int(__fastcall*)(void*, void*))
            emitter->vtbl[0x7C / 4])(emitter, nullptr);
    }

    C_00862748_UpdateParticles_6F4590(emitter, nullptr);
    auto* origin = reinterpret_cast<float*>(
        reinterpret_cast<C_00862748*>(self->field_0020)->vGetGlobal8BB420());

    alignas(8) uint8_t placementBytes[0x34];
    float& translationX = CSF_AT(float, placementBytes, 0x28);
    float& translationY = CSF_AT(float, placementBytes, 0x2C);
    float& translationZ = CSF_AT(float, placementBytes, 0x30);
    translationX = CSF_AT(float, self, 0x28) - origin[0];
    translationY = CSF_AT(float, self, 0x2C) - origin[1];
    translationZ = CSF_AT(float, self, 0x30) - origin[2];

    GfxPtank_GetPlacementState(
        reinterpret_cast<void*>(self->field_0020), nullptr, placementBytes);
    uint32_t*& state = CSF_AT(uint32_t*, placementBytes, 0x10);

    alignas(8) uint8_t sourcePositionBytes[0xC];
    alignas(8) uint8_t destinationPositionBytes[0x8];
    alignas(8) uint8_t sourceColorBytes[0x8];
    alignas(8) uint8_t destinationColorBytes[0x8];
    alignas(8) uint8_t sourceSizeBytes[0x8];
    alignas(8) uint8_t destinationSizeBytes[0x8];
    alignas(8) uint8_t sourceUvBytes[0x8];
    alignas(8) uint8_t destinationUvBytes[0x8];

    uint8_t& alphaScale = CSF_AT(uint8_t, sourcePositionBytes, 0xB);
    bool& usesMatrices = CSF_AT(bool, sourcePositionBytes, 0xA);
    alphaScale = static_cast<uint8_t>(static_cast<int64_t>(
        CSF_AT(float, self, 0x34) * g_ColorChannelScale));
    usesMatrices = (state[0] & 8) != 0;
    const unsigned int positionFlag = usesMatrices ? 8 : 1;

    RpPTankAtomicLock(static_cast<int>(state[6]),
        reinterpret_cast<uint32_t*>(sourcePositionBytes),
        positionFlag, static_cast<int>(0x80000000u));
    RpPTankAtomicLock(static_cast<int>(self->field_0024),
        reinterpret_cast<uint32_t*>(destinationPositionBytes),
        positionFlag, 0x40000000);
    RpPTankAtomicLock(static_cast<int>(state[6]),
        reinterpret_cast<uint32_t*>(sourceColorBytes),
        2, static_cast<int>(0x80000000u));
    RpPTankAtomicLock(static_cast<int>(self->field_0024),
        reinterpret_cast<uint32_t*>(destinationColorBytes),
        2, 0x40000000);
    RpPTankAtomicLock(static_cast<int>(state[6]),
        reinterpret_cast<uint32_t*>(sourceSizeBytes),
        4, static_cast<int>(0x80000000u));
    RpPTankAtomicLock(static_cast<int>(self->field_0024),
        reinterpret_cast<uint32_t*>(destinationSizeBytes),
        4, 0x40000000);
    RpPTankAtomicLock(static_cast<int>(state[6]),
        reinterpret_cast<uint32_t*>(sourceUvBytes),
        32, static_cast<int>(0x80000000u));
    RpPTankAtomicLock(static_cast<int>(self->field_0024),
        reinterpret_cast<uint32_t*>(destinationUvBytes),
        32, 0x40000000);

    auto* plugin = CSF_AT(void*,
        reinterpret_cast<void*>(self->field_0024), g_PTankPluginOffset);
    CSF_AT(uint32_t, plugin, 0x40) |= 0x800000;
    plugin = CSF_AT(void*,
        reinterpret_cast<void*>(self->field_0024), g_PTankPluginOffset);
    CSF_AT(uint32_t, plugin, 0x4) = state[2];

    std::memcpy(CSF_AT(void*, destinationColorBytes, 0),
        CSF_AT(const void*, sourceColorBytes, 0),
        CSF_AT(uint32_t, sourceColorBytes, 4) * state[2]);
    std::memcpy(CSF_AT(void*, destinationSizeBytes, 0),
        CSF_AT(const void*, sourceSizeBytes, 0),
        CSF_AT(uint32_t, sourceSizeBytes, 4) * state[2]);
    std::memcpy(CSF_AT(void*, destinationUvBytes, 0),
        CSF_AT(const void*, sourceUvBytes, 0),
        CSF_AT(uint32_t, sourceUvBytes, 4) * state[2]);

    if (state[2])
    {
        const uint16_t scale = alphaScale;
        uint32_t index = 0;
        do
        {
            if (usesMatrices)
            {
                std::memcpy(CSF_AT(void*, destinationPositionBytes, 0),
                    CSF_AT(const void*, sourcePositionBytes, 0), 0x40);
                CSF_AT(float*, destinationPositionBytes, 0)[12] =
                    translationX + CSF_AT(float*, sourcePositionBytes, 0)[12];
                CSF_AT(float*, destinationPositionBytes, 0)[13] =
                    translationY + CSF_AT(float*, sourcePositionBytes, 0)[13];
                CSF_AT(float*, destinationPositionBytes, 0)[14] =
                    translationZ + CSF_AT(float*, sourcePositionBytes, 0)[14];
            }
            else
            {
                CSF_AT(float*, destinationPositionBytes, 0)[0] =
                    translationX + CSF_AT(float*, sourcePositionBytes, 0)[0];
                CSF_AT(float*, destinationPositionBytes, 0)[1] =
                    translationY + CSF_AT(float*, sourcePositionBytes, 0)[1];
                CSF_AT(float*, destinationPositionBytes, 0)[2] =
                    translationZ + CSF_AT(float*, sourcePositionBytes, 0)[2];
            }

            uint8_t* color = CSF_AT(uint8_t*, destinationColorBytes, 0);
            color[3] = static_cast<uint8_t>(
                static_cast<uint16_t>(scale * color[3]) >> 8);

            CSF_AT(uint8_t*, sourcePositionBytes, 0) +=
                CSF_AT(uint32_t, sourcePositionBytes, 4);
            CSF_AT(uint8_t*, destinationPositionBytes, 0) +=
                CSF_AT(uint32_t, destinationPositionBytes, 4);
            CSF_AT(uint8_t*, destinationColorBytes, 0) +=
                CSF_AT(uint32_t, destinationColorBytes, 4);
            ++index;
        } while (index < state[2]);
    }

    RpPTankAtomicUnlock(static_cast<int>(state[6]));
    RpPTankAtomicUnlock(static_cast<int>(self->field_0024));

    using AtomicCallback = int(__cdecl*)(uint32_t);
    const uint32_t atomic = self->field_0024;
    return CSF_AT(AtomicCallback,
        reinterpret_cast<void*>(atomic), 0x48)(atomic);
}

HOOK(0x006F08A0, C_008622e8_UpdateTranslatedAtomic, "auto");
