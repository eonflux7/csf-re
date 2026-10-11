#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, NetGatheringDdl_InitializePrimaryString, 0x006A6DD0, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, NetGatheringDdl_InitializeSecondaryString, 0x006A6F10, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, NetGatheringDdl_InitializeStringStorage, 0x00656DE0, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, NetGatheringDdl_InitializeTertiaryString, 0x006A6E70, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, NetGatheringDdl_InitializeCollection, 0x00657590, (void* self, void* edx));

static uint32_t* __fastcall C_0085fe28_Initialize(C_0085fe28* self, void* /*edx*/)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);
    self->vtbl = CSF_VTBL(0x0085FE28);

    CSF_AT(uint32_t, self, 0x44) = reinterpret_cast<uint32_t>(
        NetGatheringDdl_InitializePrimaryString(bytes + 0x40, nullptr));
    CSF_AT(uint32_t, self, 0x48) = 0;

    self->field_0054 = reinterpret_cast<uint32_t>(
        NetGatheringDdl_InitializeSecondaryString(bytes + 0x50, nullptr));
    self->field_0058 = 0;

    CSF_AT(uint32_t, self, 0x64) = reinterpret_cast<uint32_t>(
        NetGatheringDdl_InitializeStringStorage(bytes + 0x60, nullptr));
    CSF_AT(uint32_t, self, 0x68) = 0;

    CSF_AT(uint32_t, self, 0x74) = reinterpret_cast<uint32_t>(
        NetGatheringDdl_InitializeTertiaryString(bytes + 0x70, nullptr));
    CSF_AT(uint32_t, self, 0x78) = 0;

    self->field_0088 = 0;
    CSF_AT(void*, self, 0x84) = CSF_VTBL(0x0085FE20);

    NetGatheringDdl_InitializeCollection(bytes + 0x94, nullptr);
    NetGatheringDdl_InitializeCollection(bytes + 0xA0, nullptr);

    self->field_02e0 = 0;
    self->field_02e4 = 15;
    *reinterpret_cast<uint8_t*>(&self->field_02d0) = 0;

    self->field_0300 = 15;
    self->field_02fc = 0;
    *reinterpret_cast<uint8_t*>(&self->field_02ec) = 0;

    self->field_031c = 15;
    self->field_0318 = 0;
    *reinterpret_cast<uint8_t*>(&self->field_0308) = 0;

    self->field_0004 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x006A7060, C_0085fe28_Initialize, "auto");
