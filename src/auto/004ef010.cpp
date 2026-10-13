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

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_0085e7d8_Construct, 0x0069C2A0, (void* self, void* edx));

CSF_GLOBAL(uint32_t, g_Value_8baf94, 0x008BAF94);

static C_0085e7d8* TextDatabase_GetOrCreateSingleton() {
    if (g_Value_8baf94 != 0) {
        return reinterpret_cast<C_0085e7d8*>(g_Value_8baf94);
    }

    void* allocated = Mem_Alloc(48);
    if (allocated == nullptr) {
        g_Value_8baf94 = 0;
        return nullptr;
    }

    C_0085e7d8* database = reinterpret_cast<C_0085e7d8*>(allocated);
    uint32_t* constructResult = C_0085e7d8_Construct(database, nullptr);
    database->vtbl = CSF_VTBL(0x83EE68);
    g_Value_8baf94 = reinterpret_cast<uint32_t>(database);
    return reinterpret_cast<C_0085e7d8*>(constructResult);
}

HOOK(0x4EF010, TextDatabase_GetOrCreateSingleton, "auto");
