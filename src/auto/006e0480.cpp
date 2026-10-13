#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

static int CreatePageRuntimeObject() {
    int result = CoreMemory_AllocUntagged(24);
    if (!result)
        return 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0x08) = 0;
    *reinterpret_cast<void***>(result) = CSF_VTBL(0x00861A00);
    return result;
}

HOOK(0x006E0480, CreatePageRuntimeObject, "auto");
