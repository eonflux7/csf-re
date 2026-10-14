#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee bindings (by address).
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x712CC0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x506BD0, ());

// __thiscall constructor: this in ecx, one stack argument (ret 4).
// Vtable pointers are stored as the original's immediates:
//   0x0083042C = CMenuScreen base vtable (temporary, before the derived vtable is installed)
//   0x00842908 = CMenuProfiles vtable
//   0x008428B8 = embedded subobject vtable
static void* __fastcall CMenuProfiles_Construct(void* self, void* /*edx*/, int menuHandle)
{
    void* embeddedSubobject = static_cast<uint8_t*>(self) + 4;

    CSF_AT(uint32_t, self, 0) = CSF_ADDR_VT(0x0083042C);
    C_00830438_Construct(embeddedSubobject, nullptr);

    CSF_AT(uint32_t, self, 0) = CSF_ADDR_VT(0x00842908);
    CSF_AT(uint32_t, embeddedSubobject, 0) = CSF_ADDR_VT(0x008428B8);
    CSF_AT(uint32_t, self, 0x3C) = static_cast<uint32_t>(menuHandle);
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint8_t, self, 0x40) = 0;

    int globalRecordAddress = GetGlobalValue_8B056C();
    uint32_t globalRecordWord = *reinterpret_cast<uint32_t*>(globalRecordAddress + 8);

    CSF_AT(uint32_t, self, 0x28) = globalRecordWord;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(int32_t, self, 0x38) = -1;
    CSF_AT(int32_t, self, 0x30) = -1;

    // Virtual call through vtable slot 0x40 (slot 16) of the object's current vtable.
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[16])(self, nullptr);

    return self;
}

HOOK(0x0053C100, CMenuProfiles_Construct, "auto");
