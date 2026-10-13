#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpLightCreate, 0x0059D040, (char a1));
CSF_FUNC(void*, __cdecl, RwFrameCreate, 0x006235D0, ());
CSF_FUNC(void*, __cdecl, RwFrameSetIdentity, 0x006238C0, (void* frame));
CSF_FUNC(int, __cdecl, _rwObjectHasFrameSetFrame, 0x0063CE30, (int object, int frame));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(char, __fastcall, GfxFrame_InitRotationDegreesFromMatrix, 0x006BF380, (void* self, void* , int matrixBase));
CSF_FUNC(int, __fastcall, GfxObject_ConstructEmbeddedState, 0x006BFD00, (C_00860700* self, void*));
CSF_FUNC(int, __fastcall, CLightObject_ConfigureLight, 0x006FD220, (void* self, void* edx, int a2));

static int* __fastcall CLightObject_Construct(
    CLightObject* self, void* /*edx*/, int lightType, int configuration)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);

    CSF_AT(uint32_t, self, 0x04) =
        static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0082D428)));
    CSF_AT(void**, self, 0x00) = CSF_VTBL(0x00862F70);
    CSF_AT(uint32_t, self, 0x04) =
        static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x00862F54)));

    auto* embeddedState = reinterpret_cast<C_00860700*>(
        reinterpret_cast<uint8_t*>(self) + 0x10);
    GfxObject_ConstructEmbeddedState(embeddedState, nullptr);

    CSF_AT(float, self, 0x40) = 0.0f;
    CSF_AT(float, self, 0x44) = 0.0f;
    CSF_AT(float, self, 0x48) = 0.0f;
    CSF_AT(float, self, 0x4C) = 1.0f;
    CSF_AT(float, self, 0x50) = 0.0f;
    CSF_AT(float, self, 0x54) = 0.0f;
    CSF_AT(float, self, 0x58) = 0.0f;
    CSF_AT(float, self, 0x5C) = 1.0f;
    CSF_AT(float, self, 0x60) = 0.0f;
    CSF_AT(float, self, 0x64) = 0.0f;
    CSF_AT(float, self, 0x68) = 0.0f;
    CSF_AT(float, self, 0x6C) = 1.0f;
    CSF_AT(float, self, 0x7C) = 1.0f;
    CSF_AT(float, self, 0x70) = 0.0f;
    CSF_AT(float, self, 0x74) = 0.0f;
    CSF_AT(float, self, 0x78) = 0.0f;

    char renderWareType = static_cast<char>(0x80);
    switch (lightType)
    {
    case 0:
        renderWareType = 2;
        break;
    case 1:
        renderWareType = 1;
        break;
    case 2:
        renderWareType = static_cast<char>(0x80);
        break;
    case 3:
        renderWareType = -127;
        break;
    default:
        break;
    }

    CSF_AT(int, self, 0x0C) = RpLightCreate(renderWareType);
    void* frame = RwFrameCreate();
    RwFrameSetIdentity(frame);
    GfxFrame_InitRotationDegreesFromMatrix(
        embeddedState, nullptr,
        static_cast<int>(reinterpret_cast<intptr_t>(frame)));
    _rwObjectHasFrameSetFrame(
        CSF_AT(int, self, 0x0C),
        static_cast<int>(reinterpret_cast<intptr_t>(frame)));
    CLightObject_ConfigureLight(self, nullptr, configuration);

    return reinterpret_cast<int*>(self);
}

HOOK(0x006FD400, CLightObject_Construct, "auto");
