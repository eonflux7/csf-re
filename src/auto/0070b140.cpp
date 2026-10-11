#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpPTankAtomicCreate, 0x00582400, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RpAtomicSetFrame, 0x0058F300, (int a1, int a2));
CSF_FUNC(uint32_t*, __cdecl, RpMaterialSetTexture, 0x00591560, (uint32_t* a1, int a2));
CSF_FUNC(void*, __cdecl, RwFrameCreate, 0x006235D0, ());
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));

CSF_GLOBAL(int32_t, g_PTankPluginOffset, 0x008B09EC);

// Returns the PTank plugin data block of the atomic stored in field_0008.
static uint8_t* GetPTankPluginData(C_008636ec* self)
{
    return reinterpret_cast<uint8_t*>(
        CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0008), g_PTankPluginOffset));
}

static C_008636ec* __fastcall WaterPTank_Construct(C_008636ec* self, void* /*edx*/, int a2, int a3, int a4)
{
    C_00860178_Construct(self, nullptr);
    self->field_0004 = a2;
    self->vtbl = CSF_VTBL(0x008636EC);
    self->field_0010 = a4;
    self->field_0008 = 0;
    self->field_000c = a3;
    self->field_0014 = 0;
    self->field_0018 = 0;

    uint32_t atomic = (uint32_t)RpPTankAtomicCreate(a4, 0x10000088, 0);
    self->field_0008 = atomic;
    if (atomic != 0)
    {
        uint8_t* frame = reinterpret_cast<uint8_t*>(RwFrameCreate());
        uint32_t flags = CSF_AT(uint32_t, frame, 0x1C) | 0x20003;
        CSF_AT(uint32_t, frame, 0x38) = 0x3F800000;
        CSF_AT(uint32_t, frame, 0x24) = 0x3F800000;
        CSF_AT(uint32_t, frame, 0x10) = 0x3F800000;
        CSF_AT(uint32_t, frame, 0x20) = 0;
        CSF_AT(uint32_t, frame, 0x18) = 0;
        CSF_AT(uint32_t, frame, 0x14) = 0;
        CSF_AT(uint32_t, frame, 0x34) = 0;
        CSF_AT(uint32_t, frame, 0x30) = 0;
        CSF_AT(uint32_t, frame, 0x28) = 0;
        CSF_AT(uint32_t, frame, 0x48) = 0;
        CSF_AT(uint32_t, frame, 0x44) = 0;
        CSF_AT(uint32_t, frame, 0x40) = 0;
        CSF_AT(uint32_t, frame, 0x1C) = flags;

        RpAtomicSetFrame((int)self->field_0008, (int)frame);

        CSF_AT(uint32_t, GetPTankPluginData(self), 0xAC) = 1;
        CSF_AT(uint32_t, GetPTankPluginData(self), 0xA4) = 5;
        CSF_AT(uint32_t, GetPTankPluginData(self), 0xA8) = 6;
        CSF_AT(uint32_t, GetPTankPluginData(self), 0x40) |= 0x10000000u;
        CSF_AT(uint32_t, GetPTankPluginData(self), 0xC4) = 0xFFFFFFFFu;

        uint8_t* atomicPtr = reinterpret_cast<uint8_t*>(self->field_0008);
        uint8_t* geometry = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, atomicPtr, 0x18));
        uint32_t* materialList = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, geometry, 0x20));
        uint8_t* material = reinterpret_cast<uint8_t*>(materialList[0]);
        if (material != nullptr)
        {
            CSF_AT(uint32_t, material, 4) = CSF_AT(uint32_t, GetPTankPluginData(self), 0xC4);
        }

        CSF_AT(uint32_t, GetPTankPluginData(self), 0x40) |= 0x2000u;

        {
            uint8_t* a = reinterpret_cast<uint8_t*>(self->field_0008);
            uint8_t* g = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, a, 0x18));
            uint32_t* ml = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, g, 0x20));
            RpMaterialSetTexture(reinterpret_cast<uint32_t*>(ml[0]), (int)self->field_000c);
        }

        uint8_t* plugin = GetPTankPluginData(self);
        uint32_t pflags = CSF_AT(uint32_t, plugin, 0x40);
        pflags |= 0x800000;
        CSF_AT(uint32_t, plugin, 0x40) = pflags;

        CSF_AT(uint32_t, GetPTankPluginData(self), 4) = 0;
    }
    return self;
}

HOOK(0x0070B140, WaterPTank_Construct, "auto");
