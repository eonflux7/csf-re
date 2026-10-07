#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns a pointer to the data embedded at offset +4 of the object, or nullptr
// if the object itself is null. The address is computed (this + 4), the field is
// not dereferenced.
static uint8_t* __fastcall C_0083e500_GetEmbeddedDataPointer(C_0083e500* self, void* /*edx*/)
{
    if (self)
        return reinterpret_cast<uint8_t*>(self) + 4;
    return nullptr;
}

HOOK(0x004E12B0, C_0083e500_GetEmbeddedDataPointer, "auto");
