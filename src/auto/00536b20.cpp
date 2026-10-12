#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __fastcall, SceneInstancingRefresh, 0x00536940, (void* self, void* edx, float a2, char a3));

// Stores a[0x186] flag. If it was already set, only the flag is written. Otherwise the
// instancing refresh runs first, using the float at global+0x2264, and its low byte is returned.
static char __fastcall SetFlag0186AndRefreshInstancing(S_00536940* self, void* /*edx*/, char a2)
{
    char result = static_cast<char>(self->field_0186);
    if (result) {
        self->field_0186 = a2;
    } else {
        void* globalBase = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        float refreshScale = CSF_AT(float, globalBase, 0x2264);
        result = static_cast<char>(SceneInstancingRefresh(self, nullptr, refreshScale, 1));
        self->field_0186 = a2;
    }
    return result;
}

HOOK(0x536B20, SetFlag0186AndRefreshInstancing, "auto");
