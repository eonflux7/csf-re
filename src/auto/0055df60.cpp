#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, CMenuPlayerList_ConstructSecondaryBase,
         0x00712CC0, (void* self, void* edx));


static CMenuPlayerList* __fastcall CMenuPlayerList_Construct(
    CMenuPlayerList* self, void* /*edx*/, int argument)
{
    CSF_AT(uint32_t, self, 0x00) =
        static_cast<uint32_t>(CSF_ADDR_VT(0x0083042C));

    CMenuPlayerList_ConstructSecondaryBase(
        reinterpret_cast<uint8_t*>(self) + 0x04, nullptr);

    self->field_0004 =
        static_cast<uint32_t>(CSF_ADDR_VT(0x00845628));
    self->field_0020 = static_cast<uint32_t>(argument);
    CSF_AT(uint32_t, self, 0x00) =
        static_cast<uint32_t>(CSF_ADDR_VT(0x00845678));

    self->field_001c = 0;
    std::memset(reinterpret_cast<uint8_t*>(self) + 0x30, 0, 0x208);
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;

    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0x00)[16])(self, nullptr);

    return self;
}

HOOK(0x0055DF60, CMenuPlayerList_Construct, "auto");
