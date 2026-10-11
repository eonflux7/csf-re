#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall S_0041e5f0_GrowArrayCapacity(
    S_0041e5f0* self, void* /*edx*/, int additionalCapacity)
{
    void* newBuffer = Mem_Alloc(
        4u * (self->field_0004 + static_cast<std::uint32_t>(additionalCapacity)));

    if (self->field_0008 != 0)
    {
        std::memcpy(
            newBuffer,
            reinterpret_cast<const void*>(self->field_0000),
            4u * self->field_0008);
    }

    void* oldBuffer = reinterpret_cast<void*>(self->field_0000);
    self->field_0004 += static_cast<std::uint32_t>(additionalCapacity);

    void* result = newBuffer;
    if (oldBuffer != nullptr)
    {
        result = reinterpret_cast<void*>(
            static_cast<std::uintptr_t>(Mem_Free(oldBuffer)));
        self->field_0000 = 0;
    }

    self->field_0000 = reinterpret_cast<std::uintptr_t>(newBuffer);
    return result;
}

HOOK(0x0041E5F0, S_0041e5f0_GrowArrayCapacity, "auto");
