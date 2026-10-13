#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpPrtStdAtomicIsEmitter, 0x0057C220, (int a1));
CSF_FUNC(int, __cdecl, RpPrtStdAtomicGetEmitter, 0x0057C290, (int a1));
CSF_FUNC(int, __cdecl, RpPrtStdPropTabGetPropOffset, 0x0057C8B0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpAtomicDestroy, 0x0058F060, (uint32_t* a1));
CSF_FUNC(int, __cdecl, RpAtomicSetFrame, 0x0058F300, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwFrameDestroy, 0x00623B10, (int frame));
CSF_FUNC(void*, __fastcall, RecordArray_Grow_6D9EB0, 0x006D9EB0, (void* self, void* edx, int a2));

CSF_GLOBAL(std::int32_t, g_PTankPluginOffset, 0x008B09EC);

static int __fastcall GfxParticleAtomic_AddEmitter(
    int* self, void* /*edx*/, int arrayIndex, int atomic)
{
    RpPrtStdAtomicIsEmitter(atomic);
    auto* emitter = reinterpret_cast<uint8_t*>(RpPrtStdAtomicGetEmitter(atomic));
    const int propertyOffset = RpPrtStdPropTabGetPropOffset(
        CSF_AT(int, CSF_AT(void*, emitter, 0x0C), 0x10), 6);
    auto* properties = emitter + propertyOffset;

    auto* tank = CSF_AT(
        uint8_t*, CSF_AT(uint8_t*, properties, 0x18) + g_PTankPluginOffset, 0);
    const uint32_t tankKey = CSF_AT(uint32_t, tank, 0);

    const uint32_t arrayOffset = 16 * static_cast<uint32_t>(arrayIndex);
    auto* records = CSF_AT(uint8_t*, self, 0) + arrayOffset;
    uint32_t remaining = CSF_AT(uint32_t, records, 8);
    uint32_t matchingCount = 0;

    if (remaining != 0)
    {
        auto* entry = CSF_AT(uint8_t*, records, 0);
        do
        {
            if (CSF_AT(uint32_t, entry, 4) == tankKey)
                ++matchingCount;
            entry += 12;
            --remaining;
        } while (remaining != 0);
    }

    if (matchingCount >= 0xA)
    {
        const int frame = CSF_AT(int, reinterpret_cast<void*>(atomic), 4);
        if (frame != 0)
        {
            RpAtomicSetFrame(atomic, 0);
            RwFrameDestroy(frame);
        }
        return RpAtomicDestroy(reinterpret_cast<uint32_t*>(atomic));
    }

    tank = CSF_AT(
        uint8_t*, CSF_AT(uint8_t*, properties, 0x18) + g_PTankPluginOffset, 0);
    CSF_AT(uint32_t, tank, 0x40) |= 0x800000;

    tank = CSF_AT(
        uint8_t*, CSF_AT(uint8_t*, properties, 0x18) + g_PTankPluginOffset, 0);
    CSF_AT(uint32_t, tank, 4) = 0;

    auto* particleList = CSF_AT(uint8_t*, emitter, 0x20);
    if (particleList != nullptr)
    {
        auto* node = CSF_AT(uint8_t*, particleList, 0);
        CSF_AT(uint32_t, particleList, 8) = 0;
        CSF_AT(uint32_t, particleList, 0x10) = 0;

        while (node != particleList)
        {
            CSF_AT(uint32_t, node, 8) = 0;
            CSF_AT(uint32_t, node, 0x10) = 0;
            node = CSF_AT(uint8_t*, node, 0);
        }
    }

    CSF_AT(uint32_t, emitter, 0x10) = 0;
    records = CSF_AT(uint8_t*, self, 0) + arrayOffset;
    const int initialLifetime = 20;

    if (CSF_AT(uint32_t, records, 8) >= CSF_AT(uint32_t, records, 4))
        RecordArray_Grow_6D9EB0(records, nullptr, CSF_AT(int, records, 0x0C));

    const uint32_t count = CSF_AT(uint32_t, records, 8);
    auto* entry = CSF_AT(uint8_t*, records, 0) + 12 * count;
    CSF_AT(uint32_t, records, 8) = count + 1;
    CSF_AT(int, entry, 0) = atomic;
    CSF_AT(uint32_t, entry, 4) = tankKey;
    CSF_AT(int, entry, 8) = initialLifetime;
    return initialLifetime;
}

HOOK(0x007034C0, GfxParticleAtomic_AddEmitter, "auto");
