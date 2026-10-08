#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiTextureListBox_GetColumnCount, 0x00714830,
         (CGuiListBox* self, void* edx));
CSF_FUNC(std::uint32_t*, __fastcall, MpServerInfo_InitDefaultStyle, 0x00530AB0,
         (std::uint32_t* self, void* edx));
CSF_FUNC(std::uint32_t*, __fastcall, CopyConstructStringRecord, 0x007234F0,
         (std::uint32_t* self, void* edx, std::uint32_t* source));

// One column-count argument followed by a 60-byte by-value record:
// sixteen DWORD stack arguments, matching the original ret 64.
CSF_FUNC(std::uint32_t, __fastcall, ConstructRowCells, 0x00724120,
         (void* self, void* edx, std::uint32_t columnCount,
          std::uint32_t word0, std::uint32_t word1,
          std::uint32_t word2, std::uint32_t word3,
          std::uint32_t word4, std::uint32_t word5,
          std::uint32_t word6, std::uint32_t word7,
          std::uint32_t word8, std::uint32_t word9,
          std::uint32_t word10, std::uint32_t word11,
          std::uint32_t word12, std::uint32_t word13,
          std::uint32_t word14));
CSF_FUNC(std::uint32_t, __fastcall, InsertRowCells, 0x00724470,
         (void* self, void* edx, std::uint32_t position,
          std::uint32_t count, void* cells));
CSF_FUNC(int, __cdecl, ResetWideStringsInRecordRange, 0x007233D0,
         (std::uint32_t begin, std::uint32_t end));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall CGuiListBox_InsertRows(
    CGuiListBox* self, void* /*edx*/, std::uint32_t count)
{
    const std::uint32_t columnCount =
        static_cast<std::uint32_t>(CGuiTextureListBox_GetColumnCount(self, nullptr));

    std::uint32_t style[15];
    MpServerInfo_InitDefaultStyle(style, nullptr);

    void* rowText = reinterpret_cast<void*>(self->m_pRowText);
    if (rowText) {
        const int first =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, rowText, 0)[0x70 / 4])(rowText, nullptr);

        rowText = reinterpret_cast<void*>(self->m_pRowText);
        void** vtable = CSF_AT(void**, rowText, 0);
        style[0] = static_cast<std::uint32_t>(first);
        style[1] = static_cast<std::uint32_t>(
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                vtable[0x74 / 4])(rowText, nullptr));
    }

    std::uint32_t cells[4];
    cells[1] = 0;
    cells[2] = 0;
    cells[3] = 0;

    std::uint32_t copiedStyle[15];
    CopyConstructStringRecord(copiedStyle, nullptr, style);
    ConstructRowCells(
        cells, nullptr, columnCount,
        copiedStyle[0], copiedStyle[1], copiedStyle[2],
        copiedStyle[3], copiedStyle[4], copiedStyle[5],
        copiedStyle[6], copiedStyle[7], copiedStyle[8],
        copiedStyle[9], copiedStyle[10], copiedStyle[11],
        copiedStyle[12], copiedStyle[13], copiedStyle[14]);

    int result = static_cast<int>(InsertRowCells(
        reinterpret_cast<std::uint8_t*>(self) + 0x21C,
        nullptr, self->field_0224, count, cells));

    void* block = reinterpret_cast<void*>(cells[1]);
    if (block) {
        ResetWideStringsInRecordRange(
            reinterpret_cast<std::uint32_t>(block), cells[2]);
        result = Mem_FreeDebug(block);
    }

    if (style[12] >= 8) {
        const std::uint32_t styleStorage = style[7];
        result = static_cast<int>(styleStorage);
        if (styleStorage)
            result = Mem_FreeDebug(reinterpret_cast<void*>(styleStorage));
    }
    return result;
}

HOOK(0x00724B30, CGuiListBox_InsertRows, "auto");
