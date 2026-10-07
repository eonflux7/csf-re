#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiPanel_ProcessBackgroundAndChildren(
    CGuiPanel* self, void* /*edx*/)
{
    int result =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xAC / 4])(self, nullptr);

    if (static_cast<std::uint8_t>(result) != 0)
        return result;

    if (self->m_pBackgroundQuad != 0)
    {
        void* background = reinterpret_cast<void*>(self->m_pBackgroundQuad);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, background, 0)[0x7C / 4])(background, nullptr);
    }

    const std::uint32_t childrenBegin = self->field_0138;
    if (childrenBegin != 0)
    {
        result = static_cast<std::int32_t>(
            self->field_013c - childrenBegin) >> 2;
    }
    else
    {
        result = 0;
    }

    for (int childIndex = result - 1; childIndex >= 0; --childIndex)
    {
        void* child =
            reinterpret_cast<void**>(self->field_0138)[childIndex];
        result =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, child, 0)[0x130 / 4])(child, nullptr);
    }

    return result;
}

HOOK(0x0072ADA0, CGuiPanel_ProcessBackgroundAndChildren, "auto");
