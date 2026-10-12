#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0, (std::size_t size, int flags, int tag));

static int __fastcall C_008313f8_AssignValueReference(
    C_008313f8* self, void* /*edx*/, int valueReference)
{
    int result = Mem_AllocDebug(0x1Cu, 0, 0);
    if (result)
    {
        void* storage = reinterpret_cast<void*>(result);
        CSF_AT(uint8_t, storage, 0x00) = 1;
        CSF_AT(uint32_t, storage, 0x10) = 0;
        CSF_AT(uint32_t, storage, 0x14) = 0;
        CSF_AT(uint32_t, storage, 0x18) = 0;
        CSF_AT(uint32_t, storage, 0x04) = 0;
        CSF_AT(uint32_t, storage, 0x08) = 0;
    }

    self->field_0004 = static_cast<uint32_t>(result);
    if (result)
    {
        CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0x00) = 1;
        result = static_cast<int>(self->field_0004);
        CSF_AT(int, reinterpret_cast<void*>(result), 0x04) = valueReference;
    }
    return result;
}

HOOK(0x00473EB0, C_008313f8_AssignValueReference, "auto");
