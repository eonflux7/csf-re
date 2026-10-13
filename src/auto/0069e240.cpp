#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// The import pops one stack argument. Register arguments preserve the
// original ECX value at the call without changing that stack layout.
using GlobalMemoryStatusImportFn =
    void(__fastcall*)(C_0085eac0*, void*, void*);

CSF_GLOBAL(GlobalMemoryStatusImportFn, g_GlobalMemoryStatusImport, 0x82D240);

static uint32_t __fastcall C_0085eac0_GetPhysMemoryDelta(
    C_0085eac0* self, void* /*edx*/)
{
    alignas(8) uint8_t memoryStatusBytes[0x20];
    g_GlobalMemoryStatusImport(self, nullptr, memoryStatusBytes);
    return self->field_0004 -
           CSF_AT(uint32_t, memoryStatusBytes, 0x0C);
}

HOOK(0x69E240, C_0085eac0_GetPhysMemoryDelta, "auto");
