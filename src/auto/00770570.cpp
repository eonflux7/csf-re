#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Binds (see packet callee list)
CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void*));
CSF_FUNC(bool, __cdecl, Geometry_IsPolygonSimple, 0x006AFE10, (std::uint32_t* polygon));

// Validates a sound-area text entry: its embedded polygon data must have more than
// two entries (unsigned compare) and the polygon must be simple.
static int __fastcall TextDatabase_ValidateEntry(C_008310b8* self, void* /*edx*/)
{
    // The sub-object lives at +0x30 inside the entry (address, not a load).
    C_008310b8* polygonOwner = reinterpret_cast<C_008310b8*>(reinterpret_cast<std::uint8_t*>(self) + 0x30);

    int result = 0;
    std::uint32_t* polygonData = reinterpret_cast<std::uint32_t*>(C_008310b8_GetSelf(polygonOwner, nullptr));
    std::uint32_t vertexCount = CSF_AT(std::uint32_t, polygonData, 8);
    if (vertexCount > 2u)
    {
        void* selfData = C_008310b8_GetSelf(polygonOwner, nullptr);
        if (Geometry_IsPolygonSimple(reinterpret_cast<std::uint32_t*>(selfData)))
            return 1;
    }
    return result;
}

HOOK(0x770570, TextDatabase_ValidateEntry, "auto");
