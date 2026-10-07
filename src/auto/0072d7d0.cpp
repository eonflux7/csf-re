#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int (__cdecl* const CompareIdsIgnoreCase)(
    const char*, const char*) =
    reinterpret_cast<int (__cdecl*)(const char*, const char*)>(0x007B0C41);

static int __fastcall CGuiSlider_FindPartById(
    CGuiSlider* self, void* /*edx*/, char* id)
{
    void* part = reinterpret_cast<void*>(self->m_pArrowUp);
    if (part)
    {
        const char* partId =
            reinterpret_cast<const char* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, part, 0)[0x74 / 4])(part, nullptr);
        if (CompareIdsIgnoreCase(partId, id) == 0)
            return static_cast<int>(self->m_pArrowUp);
    }

    part = reinterpret_cast<void*>(self->m_pArrowDown);
    if (part)
    {
        const char* partId =
            reinterpret_cast<const char* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, part, 0)[0x74 / 4])(part, nullptr);
        if (CompareIdsIgnoreCase(partId, id) == 0)
            return static_cast<int>(self->m_pArrowDown);
    }

    part = reinterpret_cast<void*>(self->m_pThumb);
    if (part)
    {
        const char* partId =
            reinterpret_cast<const char* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, part, 0)[0x74 / 4])(part, nullptr);
        if (CompareIdsIgnoreCase(partId, id) == 0)
            return static_cast<int>(self->m_pThumb);
    }

    return 0;
}

HOOK(0x0072D7D0, CGuiSlider_FindPartById, "auto");
