#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(std::uint32_t, g_GuiEditVtable, 0x00864E18);

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, CGuiControl_Destroy, 0x00719B10,
         (CGuiControl* self, void* edx));

static int __fastcall CGuiEdit_Destroy(CGuiEdit* self, void* /*edx*/)
{
    void* object = reinterpret_cast<void*>(self->field_0138);
    CSF_AT(void**, self, 0) =
        reinterpret_cast<void**>(&g_GuiEditVtable);

    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0138 = 0;
    }

    object = reinterpret_cast<void*>(self->m_pText);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->m_pText = 0;
    }

    if (CSF_AT(std::uint32_t, self, 0x1A0) >= 8u)
    {
        void* buffer = CSF_AT(void*, self, 0x18C);
        if (buffer)
            Mem_FreeDebug(buffer);
    }
    CSF_AT(std::uint32_t, self, 0x1A0) = 7;
    CSF_AT(std::uint32_t, self, 0x19C) = 0;
    CSF_AT(std::uint16_t, self, 0x18C) = 0;

    if (CSF_AT(std::uint32_t, self, 0x184) >= 8u)
    {
        void* buffer = CSF_AT(void*, self, 0x170);
        if (buffer)
            Mem_FreeDebug(buffer);
    }
    CSF_AT(std::uint32_t, self, 0x184) = 7;
    CSF_AT(std::uint32_t, self, 0x180) = 0;
    CSF_AT(std::uint16_t, self, 0x170) = 0;

    if (CSF_AT(std::uint32_t, self, 0x168) >= 8u)
    {
        void* buffer = CSF_AT(void*, self, 0x154);
        if (buffer)
            Mem_FreeDebug(buffer);
    }
    CSF_AT(std::uint32_t, self, 0x164) = 0;
    CSF_AT(std::uint32_t, self, 0x168) = 7;
    CSF_AT(std::uint16_t, self, 0x154) = 0;

    return CGuiControl_Destroy(static_cast<CGuiControl*>(self), nullptr);
}

HOOK(0x00721290, CGuiEdit_Destroy, "auto");
