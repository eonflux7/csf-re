#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiTextureListBox_GetColumnCount, 0x00714830,
         (CGuiTextureListBox* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, ResizeDwordVector, 0x00731000,
         (unsigned int* self, void* edx, unsigned int count, int value));
CSF_FUNC(void*, __fastcall, InsertTextureRows, 0x00731440,
         (void* self, void* edx, int position,
          unsigned int count, unsigned int* row));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static void* __fastcall CGuiTextureListBox_AppendEmptyTextureRows(
    CGuiTextureListBox* self, void* /*edx*/, unsigned int count)
{
    const unsigned int columnCount =
        static_cast<unsigned int>(
            CGuiTextureListBox_GetColumnCount(self, nullptr));

    unsigned int row[4];
    row[1] = 0;
    row[2] = 0;
    row[3] = 0;
    ResizeDwordVector(row, nullptr, columnCount, 0);

    InsertTextureRows(
        reinterpret_cast<std::uint8_t*>(self) + 0x1F8,
        nullptr, static_cast<int>(self->field_0200), count, row);

    void* block = reinterpret_cast<void*>(row[1]);
    if (block != nullptr) {
        return reinterpret_cast<void*>(
            static_cast<std::uintptr_t>(Mem_FreeDebug(block)));
    }
    return block;
}

HOOK(0x00731780, CGuiTextureListBox_AppendEmptyTextureRows, "auto");
