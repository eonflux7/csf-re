#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
static CSF_FUNC(char, __fastcall, sub_6EB9E0, 0x006EB9E0, (void* self, void* edx, char a2));
static CSF_FUNC(uint32_t*, __fastcall, sub_6ED0C0, 0x006ED0C0, (void* self, void* edx));

static C_00862028* __cdecl GfxCollision_CreateIntersectionQuery()
{
    auto* query = static_cast<C_00862028*>(Mem_Alloc(104));
    if (query)
    {
        sub_6ED0C0(query, nullptr);
        query->vtbl = CSF_VTBL(0x00861F38);
    }

    sub_6EB9E0(query, nullptr, 1);
    return query;
}

HOOK(0x006EE4C0, GfxCollision_CreateIntersectionQuery, "auto");
