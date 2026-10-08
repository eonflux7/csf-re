#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41,
         (const char* first, const char* second));

static int __fastcall CGuiPanel_FindChildById(
    CGuiPanel* self, void* /*edx*/, char* id)
{
    const uint32_t begin = self->m_childrenBegin;
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->m_childrenEnd - begin) >> 2)
        : 0;

    for (uint32_t index = 0; index < count; ++index)
    {
        void* child = reinterpret_cast<void*>(
            reinterpret_cast<uint32_t*>(self->m_childrenBegin)[index]);

        const char* childId =
            (reinterpret_cast<const char* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, child, 0)[0x74 / 4]))(child, nullptr);

        if (__stricmp(childId, id) == 0)
        {
            return static_cast<int>(
                reinterpret_cast<uint32_t*>(self->m_childrenBegin)[index]);
        }
    }

    return 0;
}

HOOK(0x0072AF10, CGuiPanel_FindChildById, "auto");
