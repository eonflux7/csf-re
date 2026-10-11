#include <cstdint>
#include "globals.h"
#include "hook.h"

namespace Concurrency {
struct IThreadProxy;
}

// Returns the thread proxy stored at +0x9C (dword index 39) of the scene loader.
static Concurrency::IThreadProxy* __fastcall SceneLoader_GetVisibilityProxy(void* self, void* /*edx*/) {
    return CSF_AT(Concurrency::IThreadProxy*, self, 0x9C);
}

HOOK(0x006CABD0, SceneLoader_GetVisibilityProxy, "auto");
