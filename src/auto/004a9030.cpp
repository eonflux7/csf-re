#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, UpdateActiveEntryProperties, 0x00489E60, (void* self, void* , int index, const void* source));

// Builds a 0x24-byte request record on the stack from the three dwords at a2,
// then forwards it to UpdateActiveEntryProperties on the entry object held in
// this->field_0014 (+0x44).
static int __fastcall C_00835228_UpdateActiveEntryFromSource(C_00835228* self, void* /*edx*/, uint32_t* a2)
{
    alignas(8) uint8_t requestFrame[0x24];

    CSF_AT(uint32_t, requestFrame, 0x00) = 1;
    CSF_AT(uint32_t, requestFrame, 0x04) = 3;
    CSF_AT(const void*, requestFrame, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(int, requestFrame, 0x0C) = -1;
    CSF_AT(int, requestFrame, 0x10) = -1;
    CSF_AT(uint32_t, requestFrame, 0x14) = a2[0];
    CSF_AT(uint32_t, requestFrame, 0x18) = a2[1];
    CSF_AT(uint32_t, requestFrame, 0x1C) = a2[2];
    CSF_AT(uint8_t, requestFrame, 0x20) = 1;
    CSF_AT(uint8_t, requestFrame, 0x21) = 0;
    CSF_AT(uint8_t, requestFrame, 0x22) = 1;

    void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0014));
    void* entryObj = CSF_AT(void*, owner, 0x44);

    return UpdateActiveEntryProperties(entryObj, nullptr, 0, requestFrame);
}

HOOK(0x004A9030, C_00835228_UpdateActiveEntryFromSource, "auto");
