#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(std::uint32_t*, __fastcall, C_00830438_Construct,
         0x00712CC0, (std::uint32_t* self, void* edx));

static CMenuSoundOptions* __fastcall CMenuSoundOptions_Construct(
    CMenuSoundOptions* self, void* /*edx*/, int parameter)
{
    CSF_AT(void**, self, 0) = CSF_VTBL(0x0083042C);

    C_00830438_Construct(
        reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uint8_t*>(self) + 4),
        nullptr);

    CSF_AT(void**, self, 0) = CSF_VTBL(0x00842AD0);
    self->field_0004 = CSF_ADDR_VT(0x00842A80);

    self->field_0030 = 1.0f;
    self->field_0034 = 1.0f;
    self->field_0038 = 1;
    self->field_003c = 0;
    self->field_0050 = static_cast<std::uint32_t>(parameter);
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;

    void* globalContext = reinterpret_cast<void*>(GetGlobalValue_8B056C());
    std::uint32_t context = CSF_AT(std::uint32_t, globalContext, 0x18);
    self->field_0028 = context;

    std::uint32_t contextMember = context
        ? CSF_AT(std::uint32_t, reinterpret_cast<void*>(context), 0x08)
        : 0;

    void** initialVtable = CSF_AT(void**, self, 0);
    self->field_002c = contextMember;
    self->field_0014 = 0;
    self->field_0054 = 0;
    self->field_0018 = 0;
    self->field_0048 = 0;
    self->field_0058 = 0;

    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        initialVtable[0x40 / 4])(self, nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x78 / 4])(self, nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x7C / 4])(self, nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x80 / 4])(self, nullptr);

    return self;
}

HOOK(0x0053CFC0, CMenuSoundOptions_Construct, "auto");
