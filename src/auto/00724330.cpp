#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(std::uint32_t*, __fastcall, InitializeTextStyle, 0x00530AB0,
         (std::uint32_t* self, void* edx));
CSF_FUNC(int, __fastcall, UpdateRowTextRange, 0x00723C90,
         (void* self, void* edx, char* text, std::uint32_t count,
          std::uint32_t* style));
CSF_FUNC(int, __cdecl, FreeDebugMemory, 0x00690E60, (void* block));

static void __fastcall CGuiListBox_UpdateRowText(
    CGuiListBox* self, void* /*edx*/, int row, std::uint32_t count)
{
    if (count == 0)
        return;

    std::uint32_t style[15];
    InitializeTextStyle(style, nullptr);

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

    const std::uint32_t rowOffset = static_cast<std::uint32_t>(row) << 4;
    void* rowRecord =
        reinterpret_cast<void*>(self->field_0220 + rowOffset);
    char* text = reinterpret_cast<char*>(
        CSF_AT(std::uint32_t, rowRecord, 8));
    UpdateRowTextRange(rowRecord, nullptr, text, count, style);

    if (style[12] >= 8) {
        void* block = reinterpret_cast<void*>(style[7]);
        if (block)
            FreeDebugMemory(block);
    }
}

HOOK(0x00724330, CGuiListBox_UpdateRowText, "auto");
