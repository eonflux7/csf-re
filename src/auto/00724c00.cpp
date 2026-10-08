#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void*, __fastcall, ClearListBoxBase, 0x007176E0,
         (CGuiListBox* self, void* edx));
CSF_FUNC(int, __fastcall, AssignRowStorage, 0x007248C0,
         (void* self, void* edx, void* source));
CSF_FUNC(int*, __cdecl, ResetWideStringRecordRange, 0x007241E0,
         (int* begin, int* end));
CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(int, __cdecl, FreeDebugMemory, 0x00690E60, (void* block));

static int __fastcall CGuiListBox_ClearRowStorage(
    CGuiListBox* self, void* /*edx*/)
{
    ClearListBoxBase(self, nullptr);

    void* rowText = reinterpret_cast<void*>(self->m_pRowText);
    if (rowText) {
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, rowText, 0)[0x20 / 4])(rowText, nullptr);
    }

    void* storage = reinterpret_cast<std::uint8_t*>(self) + 0x21C;
    std::uint32_t temporary[4];
    temporary[1] = 0;
    temporary[2] = 0;
    temporary[3] = 0;
    AssignRowStorage(temporary, nullptr, storage);

    int* begin = reinterpret_cast<int*>(
        CSF_AT(std::uint32_t, storage, 4));
    if (begin) {
        int* end = reinterpret_cast<int*>(
            CSF_AT(std::uint32_t, storage, 8));
        ResetWideStringRecordRange(begin, end);

        void* block = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t, storage, 4));
        if (block)
            FreeDebugMemory(block);
    }

    CSF_AT(std::uint32_t, storage, 4) = 0;
    CSF_AT(std::uint32_t, storage, 8) = 0;
    CSF_AT(std::uint32_t, storage, 12) = 0;
    SetAllocationMode(2);

    AssignRowStorage(storage, nullptr, temporary);
    int result = SetAllocationMode(0);

    void* block = reinterpret_cast<void*>(temporary[1]);
    if (block) {
        ResetWideStringRecordRange(
            reinterpret_cast<int*>(block),
            reinterpret_cast<int*>(temporary[2]));
        result = FreeDebugMemory(block);
    }
    return result;
}

HOOK(0x00724C00, CGuiListBox_ClearRowStorage, "auto");
