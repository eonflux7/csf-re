#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(int, __fastcall, C_00860894_Init, 0x006C45F0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, C_008670f0_SetField0C, 0x006C4620, (C_008670f0* self, void* , int value));
CSF_FUNC(int, __fastcall, Subobject_SetKeyedValue, 0x006C4BF0, (void* self, void* edx, int a2, unsigned int a3));
CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(void, __fastcall, S_0042cfd0_AttachSource, 0x0042CFD0, (void* self, void* edx, void* a2));

static void* GetOverlaySubobject(void* object)
{
    return ((void* (__fastcall*)(void*, void*))CSF_AT(void**, object, 0)[7])(object, nullptr);
}

static C_00860178** __fastcall S_0042cfd0_Construct(
    S_0042cfd0* self, void* /*edx*/, int a2)
{
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_0030 = static_cast<uint32_t>(-1);
    self->field_000c = static_cast<uint32_t>(-1);

    void* overlay = Mem_Alloc(28);
    if (overlay != nullptr)
    {
        C_00860178_Construct(reinterpret_cast<C_00860178*>(overlay), nullptr);
        CSF_AT(void**, overlay, 0) = CSF_VTBL(0x00830630);
        C_00860894_Init(static_cast<uint8_t*>(overlay) + 8, nullptr, -1, 0);
        CSF_AT(S_0042cfd0*, overlay, 4) = self;
    }
    self->field_0000 = reinterpret_cast<uint32_t>(overlay);

    void* globalObject = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    void* scene = CSF_AT(void*, globalObject, 4);
    void* currentOverlay = reinterpret_cast<void*>(self->field_0000);
    int sceneRegion = GetSceneRegionAt(scene, nullptr, 0);
    C_008670f0_SetField0C(
        reinterpret_cast<C_008670f0*>(GetOverlaySubobject(currentOverlay)),
        nullptr, sceneRegion);

    Subobject_SetKeyedValue(GetOverlaySubobject(reinterpret_cast<void*>(self->field_0000)), nullptr, 20, 1);
    Subobject_SetKeyedValue(GetOverlaySubobject(reinterpret_cast<void*>(self->field_0000)), nullptr, 6, 0);
    Subobject_SetKeyedValue(GetOverlaySubobject(reinterpret_cast<void*>(self->field_0000)), nullptr, 8, 0);
    Subobject_SetKeyedValue(GetOverlaySubobject(reinterpret_cast<void*>(self->field_0000)), nullptr, 29, 5);
    Subobject_SetKeyedValue(GetOverlaySubobject(reinterpret_cast<void*>(self->field_0000)), nullptr, 30, 0);
    Subobject_SetKeyedValue(GetOverlaySubobject(reinterpret_cast<void*>(self->field_0000)), nullptr, 10, 5);
    Subobject_SetKeyedValue(GetOverlaySubobject(reinterpret_cast<void*>(self->field_0000)), nullptr, 11, 6);

    S_0042cfd0_AttachSource(self, nullptr, reinterpret_cast<void*>(a2));
    return reinterpret_cast<C_00860178**>(self);
}

HOOK(0x0042D470, S_0042cfd0_Construct, "auto");
