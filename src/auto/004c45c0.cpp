#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CoverQueryInit, 0x004B2210, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, CoverRouteExecute, 0x004C4490, (void* self, void* edx, uint32_t* a2));

// Builds a 0x34-byte cover query record with "unset" (-1) slots and a
// vtable-tagged slot pair, initializes it from the two stack arguments,
// then runs the cover route with this object's state.
static int __fastcall AiGoToCover_StartWithDefaults(S_004bd310* self, void* /*edx*/, int a2, int a3)
{
    uint32_t cover_query[13];

    cover_query[0] = 0xFFFFFFFFu;
    cover_query[1] = 0xFFFFFFFFu;
    cover_query[2] = 0xFFFFFFFFu;
    cover_query[4] = 0xFFFFFFFFu;
    cover_query[5] = 0xFFFFFFFFu;
    cover_query[10] = 0xFFFFFFFFu;
    cover_query[11] = 0xFFFFFFFFu;

    const uint32_t tag = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
    cover_query[3] = tag;
    cover_query[9] = tag;

    cover_query[6] = 0;
    cover_query[7] = 0;
    cover_query[8] = 0;
    cover_query[12] = 0;

    CoverQueryInit(cover_query, nullptr, a2, a3);
    return CoverRouteExecute(self, nullptr, cover_query);
}

HOOK(0x004C45C0, AiGoToCover_StartWithDefaults, "auto");
