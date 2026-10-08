#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_CameraResourcesVtable, 0x008655E8);

CSF_FUNC(int, __cdecl, InitializeQuadGeometryBuffers, 0x00739340, ());
CSF_FUNC(void*, __cdecl, CreateFrame, 0x006235D0, ());
CSF_FUNC(void*, __cdecl, SetFrameIdentity, 0x006238C0, (void* frame));
CSF_FUNC(int, __fastcall, CreateCameraSubRasters, 0x00726C70,
         (C_008655e8* self, void* edx,
          int x, int y, int width, int height));
CSF_FUNC(int, __cdecl, SetObjectFrame, 0x0063CE30,
         (void* object, void* frame));
CSF_FUNC(void*, __cdecl, AllocateGameMemory, 0x006910F0,
         (std::size_t size));
CSF_FUNC(std::uint32_t*, __fastcall, InitializeOwnerHelper, 0x006AFB80,
         (std::uint32_t* self, void* edx, std::uint32_t owner));

static C_008655e8* __fastcall C_008655e8_InitializeCameraResources(
    C_008655e8* self, void* /*edx*/,
    int x, int y, int width, int height)
{
    self->vtbl = reinterpret_cast<void**>(&g_CameraResourcesVtable);

    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 10;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_0064 = 10;
    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0074 = 0;
    self->field_0078 = 10;

    InitializeQuadGeometryBuffers();

    self->field_0004 = 0;
    self->field_0028 = 0;

    void* frame = CreateFrame();
    self->field_0008 = reinterpret_cast<std::uintptr_t>(frame);
    SetFrameIdentity(frame);

    CreateCameraSubRasters(self, nullptr, x, y, width, height);
    SetObjectFrame(
        reinterpret_cast<void*>(self->field_000c),
        reinterpret_cast<void*>(self->field_0008));

    self->field_0010 = static_cast<std::uint32_t>(width);
    self->field_0014 = static_cast<std::uint32_t>(height);
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0054 = 0;
    self->field_0068 = 0;
    self->field_007c = 0;

    void* helperStorage = AllocateGameMemory(0x34);
    if (helperStorage)
    {
        self->field_0080 = reinterpret_cast<std::uintptr_t>(
            InitializeOwnerHelper(
                static_cast<std::uint32_t*>(helperStorage), nullptr,
                reinterpret_cast<std::uintptr_t>(self)));
    }
    else
    {
        self->field_0080 = 0;
    }

    self->field_007d = 0;
    return self;
}

HOOK(0x00728000, C_008655e8_InitializeCameraResources, "auto");
