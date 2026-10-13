#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_NetProfileInstance, 0x008B058C);

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static int32_t* __cdecl GetOrCreateNetProfileInstance(int32_t profileId) {
    int32_t* result = reinterpret_cast<int32_t*>(g_NetProfileInstance);
    if (result == nullptr) {
        result = reinterpret_cast<int32_t*>(Mem_Alloc(16));
        if (result != nullptr) {
            result[0] = profileId;
            result[1] = 0;
            result[2] = 0;
            result[3] = -1;
            g_NetProfileInstance = result;
        } else {
            g_NetProfileInstance = nullptr;
        }
    }
    return result;
}

HOOK(0x5190B0, GetOrCreateNetProfileInstance, "auto");
