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

CSF_FUNC(int, __fastcall, sub_46F4D0, 0x0046F4D0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, sub_471D00, 0x00471D00, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_768E70, 0x00768E70, ());
CSF_FUNC(int, __fastcall, sub_769360, 0x00769360, (void* self, void* edx, unsigned int a2));

// Vtable binding referenced by the original's recursive call site (per packet).
[[maybe_unused]] static const auto kMissionScriptNodeVtbl = CSF_VTBL(0x0046F900);

static uint32_t* __fastcall MissionScript_VisitNodeRange(S_0046f900* self, void* /*edx*/, int16_t* a2)
{
    // Leaf entry: a2[1] == -1 means a2 stores a direct pointer at +4.
    if (a2[1] == -1)
    {
        return sub_471D00(CSF_AT(void*, a2, 4), nullptr);
    }

    // The original keeps v7 on the stack across GetGlobalValue_768E70 and
    // hands it to sub_769360 as its stack argument.
    unsigned int v7 = static_cast<unsigned int>(static_cast<int>(a2[0]));
    int globalValue = GetGlobalValue_768E70();
    uint32_t* result = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(
        sub_769360(reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr, v7)));

    if (*result != 4)
    {
        // Child count stored at +0x18 of the resolved record, read once before the loop.
        uint32_t childCount = CSF_AT(uint32_t, result, 0x18);
        for (uint32_t i = 0; i < childCount; ++i)
        {
            void* listOwner = CSF_AT(void*, self, 0x6C);
            int childId = static_cast<int>(a2[1]) + static_cast<int>(i);
            int16_t* child = reinterpret_cast<int16_t*>(static_cast<uintptr_t>(
                sub_46F4D0(listOwner, nullptr, childId)));
            result = MissionScript_VisitNodeRange(self, nullptr, child);
        }
    }
    return result;
}

HOOK(0x0046F900, MissionScript_VisitNodeRange, "auto");
