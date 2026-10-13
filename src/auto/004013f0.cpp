#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __cdecl, MpSpawnPoints_RegisterObject, 0x00475850, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));

static C_0082d46c* __fastcall C_0082d46c_Construct(C_0082d46c* self, void* /*edx*/) {
    // Pad slot at +0x4 is written as a data address before and after the vtable store.
    uint8_t* padPtr = reinterpret_cast<uint8_t*>(self) + 0x4;

    CSF_AT(uint32_t, self, 0x4) = CSF_ADDR_DATA(0x0082D420);
    self->vtbl = CSF_VTBL(0x0082D46C);
    CSF_AT(uint32_t, self, 0x4) = CSF_ADDR_DATA(0x0082D464);

    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 10;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 10;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 10;

    MpSpawnPoints_RegisterObject(static_cast<int>(reinterpret_cast<uintptr_t>(self)));

    int globalInt = GetGlobalInt();
    int renderHandle = RenderHandle_FromObject(reinterpret_cast<void*>(globalInt), nullptr);

    // Virtual slot at +0x34 -> index 13; this = renderHandle object, arg = self + 4.
    void* handleObj = reinterpret_cast<void*>(renderHandle);
    using SlotFn = void(__fastcall*)(void*, void*, uint8_t*);
    SlotFn slot = reinterpret_cast<SlotFn>(CSF_AT(void**, handleObj, 0)[13]);
    slot(handleObj, nullptr, padPtr);

    return self;
}

HOOK(0x004013F0, C_0082d46c_Construct, "auto");
