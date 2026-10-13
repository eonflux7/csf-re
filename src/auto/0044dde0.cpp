#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Constructor of C_00831230. Initializes the object's own fields, allocates a
// 36-byte helper record from the game heap, and links it at +0x1C (or nullptr on failure).
static uint32_t* __fastcall C_00831230_Construct(C_00831230* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00831230);
    CSF_AT(uint32_t, self, 0x18) = 10;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    self->field_0004 = 0;
    self->field_0008 = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;

    uint8_t* helper = static_cast<uint8_t*>(Mem_Alloc(36));
    if (helper != nullptr)
    {
        CSF_AT(uint32_t, helper, 0x04) = 0;
        CSF_AT(uint32_t, helper, 0x08) = 0;
        CSF_AT(uint32_t, helper, 0x0C) = 0;
        CSF_AT(uint32_t, helper, 0x10) = 10;
        CSF_AT(uint32_t, helper, 0x14) = 0;
        CSF_AT(uint32_t, helper, 0x18) = 0;
        CSF_AT(uint32_t, helper, 0x1C) = 0;
        CSF_AT(uint32_t, helper, 0x20) = 10;
        *reinterpret_cast<void***>(helper) = CSF_VTBL(0x00831210);
        CSF_AT(void*, self, 0x1C) = helper;
    }
    else
    {
        CSF_AT(void*, self, 0x1C) = nullptr;
    }

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0044DDE0, C_00831230_Construct, "auto");
