#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

// The CGuiTextureListBox vtable pointer stored into the object at construction
// time and restored here during destruction.
CSF_GLOBAL(void*, g_CGuiTextureListBoxVtable, 0x00866DB8);

// Releases every record in the range [first, last) of the dynamic array owned
// by the list box (cdecl, 4 args -> the caller cleans 16 bytes).
CSF_FUNC(int, __cdecl, DestroyRecordRange, 0x006DEC40,
         (int* first, int* last, void* owner, void* self));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60,
         (void* block));

// Base list-box destructor (thiscall, no stack args).
CSF_FUNC(int, __fastcall, CGuiListBoxBase_Destroy, 0x00716110,
         (CGuiTextureListBox* self, void* edx));

static int __fastcall CGuiTextureListBox_Destroy(
    CGuiTextureListBox* self, void* /*edx*/)
{
    // Embedded record-array bookkeeping object at +0x1F8.
    auto* range = reinterpret_cast<std::uint8_t*>(self) + 0x1F8;

    CSF_AT(void**, self, 0) = &g_CGuiTextureListBoxVtable;

    const std::uint32_t first = CSF_AT(std::uint32_t, range, 4);
    if (first)
    {
        DestroyRecordRange(
            reinterpret_cast<int*>(first),
            reinterpret_cast<int*>(CSF_AT(std::uint32_t, range, 8)),
            range,
            self);

        // The callee may have rewritten the array, so re-read + reload.
        const std::uint32_t allocation = CSF_AT(std::uint32_t, range, 4);
        if (allocation)
            Mem_FreeDebug(reinterpret_cast<void*>(allocation));
    }

    CSF_AT(std::uint32_t, range, 4) = 0;
    CSF_AT(std::uint32_t, range, 8) = 0;
    CSF_AT(std::uint32_t, range, 0xC) = 0;

    return CGuiListBoxBase_Destroy(self, nullptr);
}

HOOK(0x007312C0, CGuiTextureListBox_Destroy, "auto");
