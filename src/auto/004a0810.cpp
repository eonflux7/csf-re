#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

// Vtable of the list/array helper object embedded at +0x10 (referenced by the original).
[[maybe_unused]] static void* const kListArrayVtable = CSF_VTBL(0x0041E5F0);

// Allocates a 28-byte alert icon entry (vtable 0x82D448), copies two words from the
// source descriptor, and appends the entry to the pointer list embedded at self+0x10.
static int __fastcall HudAlertIcons_AppendIconEntry(char* self, void* /*edx*/, int sourceArg)
{
    char* entry = nullptr;
    void* allocated = Mem_Alloc(28);
    if (allocated) {
        entry = static_cast<char*>(allocated);
        CSF_AT(uint32_t, entry, 0x0C) = 0;
        CSF_AT(uint32_t, entry, 0x14) = 0;
        CSF_AT(void*, entry, 0x00) = CSF_VTBL(0x0082D448);
        CSF_AT(int, entry, 0x04) = -1;
        CSF_AT(int, entry, 0x08) = -1;
        CSF_AT(int, entry, 0x10) = -1;
        CSF_AT(uint8_t, entry, 0x18) = 1;
    }

    char* source = reinterpret_cast<char*>(sourceArg);
    CSF_AT(uint32_t, entry, 0x04) = CSF_AT(uint32_t, source, 0x04);
    CSF_AT(uint32_t, entry, 0x08) = CSF_AT(uint32_t, source, 0x08);

    S_0041e5f0* list = reinterpret_cast<S_0041e5f0*>(self + 0x10);
    if (list->field_0008 >= list->field_0004)
        S_0041e5f0_GrowArrayCapacity(list, nullptr, list->field_000c);

    uint32_t index = list->field_0008;
    uint32_t* slot = reinterpret_cast<uint32_t*>(list->field_0000 + 4 * index);
    list->field_0008 = index + 1;
    *slot = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(entry));

    return static_cast<int>(reinterpret_cast<uintptr_t>(entry));
}

HOOK(0x004A0810, HudAlertIcons_AppendIconEntry, "auto");
