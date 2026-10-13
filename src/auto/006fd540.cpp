#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(char, __fastcall, GfxFrame_InitRotationDegreesFromMatrix, 0x006BF380, (void* self, void* , int matrixBase));
CSF_FUNC(int, __fastcall, GfxObject_ConstructEmbeddedState, 0x006BFD00, (C_00860700* self, void*));
CSF_FUNC(int, __fastcall, LightObject_ApplyStartupParam, 0x006FD220, (void* self, void* edx, int a2));

static CLightObject* __fastcall CLightObject_Initialize(CLightObject* self, void* /*edx*/, int a2, int a3)
{
    C_00860178_Construct(self, nullptr);

    CSF_AT(uint32_t, self, 0x04) = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0082D428));
    self->vtbl = CSF_VTBL(0x00862F70);
    CSF_AT(uint32_t, self, 0x04) = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00862F54));

    uint8_t* embeddedBase = reinterpret_cast<uint8_t*>(self) + 0x10;
    GfxObject_ConstructEmbeddedState(reinterpret_cast<C_00860700*>(embeddedBase), nullptr);

    CSF_AT(uint32_t, self, 0x40) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;
    CSF_AT(uint32_t, self, 0x48) = 0;
    CSF_AT(float, self, 0x4C) = 1.0f;
    CSF_AT(uint32_t, self, 0x50) = 0;
    CSF_AT(uint32_t, self, 0x54) = 0;
    CSF_AT(uint32_t, self, 0x58) = 0;
    CSF_AT(float, self, 0x5C) = 1.0f;
    CSF_AT(uint32_t, self, 0x60) = 0;
    CSF_AT(uint32_t, self, 0x64) = 0;
    CSF_AT(uint32_t, self, 0x68) = 0;
    CSF_AT(float, self, 0x6C) = 1.0f;
    CSF_AT(uint32_t, self, 0x70) = 0;
    CSF_AT(uint32_t, self, 0x74) = 0;
    CSF_AT(uint32_t, self, 0x78) = 0;
    CSF_AT(float, self, 0x7C) = 1.0f;

    CSF_AT(int, self, 0x0C) = a2;

    void* lightSourceObject = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(a2)));
    int matrixBase = static_cast<int>(CSF_AT(uint32_t, lightSourceObject, 0x04));
    GfxFrame_InitRotationDegreesFromMatrix(embeddedBase, nullptr, matrixBase);

    LightObject_ApplyStartupParam(self, nullptr, a3);

    return self;
}

HOOK(0x006FD540, CLightObject_Initialize, "auto");
