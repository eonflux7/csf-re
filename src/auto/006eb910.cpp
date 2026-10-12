#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
static CSF_FUNC(char, __fastcall, InitializeCollisionQuery, 0x006EB9E0,
                (void* self, void* edx, char a2));
static CSF_FUNC(uint32_t*, __fastcall, ConstructCollisionQueryBase, 0x006ED0C0,
                (void* self, void* edx));

static C_00862028* __cdecl GfxCollision_CreateQuery()
{
    auto* query = static_cast<C_00862028*>(Mem_Alloc(104));
    if (query)
    {
        ConstructCollisionQueryBase(query, nullptr);
        query->vtbl = CSF_VTBL(0x00861F38);
    }
    InitializeCollisionQuery(query, nullptr, 0);
    return query;
}

HOOK(0x006EB910, GfxCollision_CreateQuery, "auto");
