#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static void** const g_GuiControlVtable =
    CSF_VTBL(0x00864288);
static void** const g_ControlBaseVtable =
    CSF_VTBL(0x00830438);

static int __fastcall CGuiControl_Destroy(CGuiControl* self, void* /*edx*/)
{
    void* parent = reinterpret_cast<void*>(self->m_pParent);
    CSF_AT(void**, self, 0x00) = g_GuiControlVtable;

    if (parent)
    {
        ((void(__fastcall*)(void*, void*, CGuiControl*))
            CSF_AT(void**, parent, 0x00)[64])(parent, nullptr, self);
    }

    void* ownedObject = reinterpret_cast<void*>(self->m_pColorAnim);
    if (ownedObject)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, ownedObject, 0x00)[0])(ownedObject, nullptr, 1);
        self->m_pColorAnim = 0;
    }

    ownedObject = reinterpret_cast<void*>(self->m_pPosAnim);
    if (ownedObject)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, ownedObject, 0x00)[0])(ownedObject, nullptr, 1);
        self->m_pPosAnim = 0;
    }

    ownedObject = reinterpret_cast<void*>(self->m_pSizeAnim);
    if (ownedObject)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, ownedObject, 0x00)[0])(ownedObject, nullptr, 1);
        self->m_pSizeAnim = 0;
    }

    ownedObject = reinterpret_cast<void*>(self->m_pDebugRect);
    if (ownedObject)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, ownedObject, 0x00)[0])(ownedObject, nullptr, 1);
        self->m_pDebugRect = 0;
    }

    ownedObject = reinterpret_cast<void*>(self->m_pEffects);
    if (ownedObject)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, ownedObject, 0x00)[0])(ownedObject, nullptr, 1);
        self->m_pEffects = 0;
    }

    int result = static_cast<int>(self->m_listenersBegin);
    if (result != 0)
    {
        result = Mem_FreeDebug(reinterpret_cast<void*>(
            static_cast<std::uint32_t>(result)));
    }

    self->m_listenersBegin = 0;
    self->m_listenersEnd = 0;
    self->m_listenersCap = 0;

    if (CSF_AT(std::uint32_t, self, 0xA0) >= 0x10u)
    {
        result = static_cast<int>(CSF_AT(std::uint32_t, self, 0x8C));
        if (result != 0)
        {
            result = Mem_FreeDebug(reinterpret_cast<void*>(
                static_cast<std::uint32_t>(result)));
        }
    }
    CSF_AT(std::uint32_t, self, 0xA0) = 15;
    CSF_AT(std::uint32_t, self, 0x9C) = 0;
    CSF_AT(std::uint8_t, self, 0x8C) = 0;

    if (CSF_AT(std::uint32_t, self, 0x84) >= 0x10u)
    {
        result = static_cast<int>(CSF_AT(std::uint32_t, self, 0x70));
        if (result != 0)
        {
            result = Mem_FreeDebug(reinterpret_cast<void*>(
                static_cast<std::uint32_t>(result)));
        }
    }
    CSF_AT(std::uint32_t, self, 0x84) = 15;
    CSF_AT(std::uint32_t, self, 0x80) = 0;
    CSF_AT(std::uint8_t, self, 0x70) = 0;

    if (CSF_AT(std::uint32_t, self, 0x68) >= 0x10u)
    {
        result = static_cast<int>(CSF_AT(std::uint32_t, self, 0x54));
        if (result != 0)
        {
            result = Mem_FreeDebug(reinterpret_cast<void*>(
                static_cast<std::uint32_t>(result)));
        }
    }
    CSF_AT(std::uint32_t, self, 0x68) = 15;
    CSF_AT(std::uint32_t, self, 0x64) = 0;
    CSF_AT(std::uint8_t, self, 0x54) = 0;

    if (CSF_AT(std::uint32_t, self, 0x4C) >= 0x10u)
    {
        result = static_cast<int>(CSF_AT(std::uint32_t, self, 0x38));
        if (result != 0)
        {
            result = Mem_FreeDebug(reinterpret_cast<void*>(
                static_cast<std::uint32_t>(result)));
        }
    }
    CSF_AT(std::uint32_t, self, 0x4C) = 15;
    CSF_AT(std::uint32_t, self, 0x48) = 0;
    CSF_AT(std::uint8_t, self, 0x38) = 0;

    if (CSF_AT(std::uint32_t, self, 0x30) >= 0x10u)
    {
        result = static_cast<int>(CSF_AT(std::uint32_t, self, 0x1C));
        if (result != 0)
        {
            result = Mem_FreeDebug(reinterpret_cast<void*>(
                static_cast<std::uint32_t>(result)));
        }
    }
    CSF_AT(std::uint32_t, self, 0x30) = 15;
    CSF_AT(std::uint32_t, self, 0x2C) = 0;
    CSF_AT(std::uint8_t, self, 0x1C) = 0;

    CSF_AT(void**, self, 0x00) = g_ControlBaseVtable;
    return result;
}

HOOK(0x00719B10, CGuiControl_Destroy, "auto");
