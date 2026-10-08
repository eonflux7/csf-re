#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CGuiListBoxBase_EnsureColumn, 0x00716490,
         (CGuiListBoxBase* self, void* edx, int columnIndex));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall CGuiListBoxBase_SetColumnText(
    CGuiListBoxBase* self, void* /*edx*/,
    int columnIndex, void* textDescription, int textKind)
{
    if (columnIndex < 0)
        return;

    self->m_bLayoutDirty = 1;
    CGuiListBoxBase_EnsureColumn(self, nullptr, columnIndex);

    // Retained from the original, although the entry check excludes -1.
    if (columnIndex == -1)
    {
        const std::uint32_t begin = self->m_columnsBegin;
        std::int32_t count = 0;
        if (begin != 0)
        {
            const std::int32_t byteCount =
                static_cast<std::int32_t>(self->m_columnsEnd - begin);
            count = byteCount / 12;
        }
        columnIndex = count - 1;
    }

    const std::uint32_t columnOffset =
        static_cast<std::uint32_t>(columnIndex) * 12u;

    void* oldText = CSF_AT(
        void*, reinterpret_cast<void*>(self->m_columnsBegin + columnOffset), 4);

    if (oldText != nullptr)
    {
        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, oldText, 0)[0])(oldText, nullptr, 1);

        CSF_AT(std::uint32_t,
               reinterpret_cast<void*>(self->m_columnsBegin + columnOffset),
               4) = 0;
    }

    void* renderer = reinterpret_cast<void*>(self->m_pRenderer);

    // PUSH EDI saves the nonvolatile register; slot +0x50 takes one argument.
    void* text = ((void* (__fastcall*)(void*, void*, int))
        CSF_AT(void**, renderer, 0)[0x50 / 4])(
            renderer, nullptr, textKind);

    void* stringData;
    if (CSF_AT(std::uint32_t, textDescription, 0x30) < 8u)
        stringData = static_cast<std::uint8_t*>(textDescription) + 0x1C;
    else
        stringData = CSF_AT(void*, textDescription, 0x1C);

    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, text, 0)[0x40 / 4])(text, nullptr, stringData);

    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, text, 0)[0x60 / 4])(
            text, nullptr, static_cast<std::uint8_t*>(textDescription) + 8);

    ((void (__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, text, 0)[0x68 / 4])(
            text, nullptr, CSF_AT(std::uint32_t, textDescription, 0));

    ((void (__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, text, 0)[0x6C / 4])(
            text, nullptr, CSF_AT(std::uint32_t, textDescription, 4));

    if (CSF_AT(float, textDescription, 0x34) != g_fZero ||
        CSF_AT(float, textDescription, 0x38) != g_fZero)
    {
        ((void (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, text, 0)[0x98 / 4])(
                text, nullptr,
                static_cast<std::uint8_t*>(textDescription) + 0x34);
    }

    CSF_AT(void*,
           reinterpret_cast<void*>(self->m_columnsBegin + columnOffset),
           4) = text;
}

HOOK(0x00716950, CGuiListBoxBase_SetColumnText, "auto");
