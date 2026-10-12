#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpPrtStdPropTabGetPropOffset, 0x0057C8B0, (int a1, int a2));
CSF_FUNC(uint32_t*, __cdecl, RpPrtStdEmitterNewParticleBatch, 0x00580230, (int* a1));

CSF_GLOBAL(uint32_t, g_DefaultParamA_8BB42C, 0x008BB42C);
CSF_GLOBAL(std::uint32_t, g_BlackClearColor, 0x008BB430);

static uint32_t* __cdecl GfxParticleRender_InitializeEmitterProperties(
    uint32_t* emitter, const void* parameters)
{
    void* emitterClass =
        reinterpret_cast<void*>(CSF_AT(uint32_t, emitter, 0x0C));
    int propertyOffset = RpPrtStdPropTabGetPropOffset(
        CSF_AT(int, emitterClass, 0x10), 9);
    auto* properties = reinterpret_cast<uint8_t*>(emitter) + propertyOffset;

    CSF_AT(uint32_t, properties, 0x00) = 0;
    CSF_AT(uint8_t, properties, 0x05) = 0;
    CSF_AT(uint8_t, properties, 0x04) = 0;
    CSF_AT(uint32_t, properties, 0x06) = g_BlackClearColor;
    CSF_AT(uint32_t, properties, 0x0A) = g_DefaultParamA_8BB42C;
    CSF_AT(uint8_t, properties, 0x0E) = 0xFF;
    CSF_AT(uint8_t, properties, 0x0F) = 0;
    CSF_AT(uint8_t, properties, 0x10) = 0;
    CSF_AT(uint32_t, properties, 0x14) = 0;
    CSF_AT(uint32_t, properties, 0x18) = 0;
    CSF_AT(uint32_t, properties, 0x1C) = 0;
    CSF_AT(uint32_t, properties, 0x20) = 0;
    CSF_AT(uint32_t, properties, 0x24) = 0;
    CSF_AT(uint8_t, properties, 0x28) = 0;
    CSF_AT(uint32_t, properties, 0x2C) = 0;
    CSF_AT(uint8_t, properties, 0x29) = 1;
    CSF_AT(uint8_t, properties, 0x2A) = 1;

    CSF_AT(uint32_t, properties, 0x00) = CSF_AT(uint32_t, parameters, 0x00);
    CSF_AT(uint8_t, properties, 0x04) = CSF_AT(uint8_t, parameters, 0x04);
    CSF_AT(uint8_t, properties, 0x05) = CSF_AT(uint8_t, parameters, 0x05);
    CSF_AT(uint32_t, properties, 0x06) = CSF_AT(uint32_t, parameters, 0x06);
    CSF_AT(uint32_t, properties, 0x0A) = CSF_AT(uint32_t, parameters, 0x0A);
    CSF_AT(uint8_t, properties, 0x0E) = CSF_AT(uint8_t, parameters, 0x0E);
    CSF_AT(uint8_t, properties, 0x0F) = CSF_AT(uint8_t, parameters, 0x0F);
    CSF_AT(uint8_t, properties, 0x10) = CSF_AT(uint8_t, parameters, 0x10);

    void* particleClass =
        reinterpret_cast<void*>(CSF_AT(uint32_t, parameters, 0x00));
    int batchCapacity = CSF_AT(int, particleClass, 0x10);
    if (batchCapacity > 0 && CSF_AT(uint32_t, emitter, 0x20) == 0)
    {
        uint32_t* batch =
            RpPrtStdEmitterNewParticleBatch(reinterpret_cast<int*>(emitter));
        CSF_AT(uint32_t, batch, 0x10) = 0;
        CSF_AT(int, batch, 0x08) = batchCapacity;
    }

    CSF_AT(uint32_t, emitter, 0x04) = 119;
    return emitter;
}

HOOK(0x007037F0, GfxParticleRender_InitializeEmitterProperties, "auto");
