#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __stdcall ResolveHandleIntoOwnerSlot(int ownerRef, int unusedArg, int entryRef) {
    // Mark the owner's slot as empty before resolving.
    uint32_t* owner = reinterpret_cast<uint32_t*>(ownerRef);
    reinterpret_cast<uint32_t*>(owner[1])[1] = 0xFFFFFFFFu;

    // entryRef -> pointer to a node; node->+4 is the entry whose +12 holds the handle index.
    uint32_t* node = reinterpret_cast<uint32_t*>(*reinterpret_cast<uint32_t*>(entryRef));
    uint32_t* entry = reinterpret_cast<uint32_t*>(node[1]);
    int handleIndex = static_cast<int>(entry[3]);

    int result = static_cast<int>(reinterpret_cast<uintptr_t>(entry) + 4);
    if (handleIndex >= 0) {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result != 0) {
            // Owner slot is reloaded from the owner object after the call.
            uint32_t* ownerAfter = reinterpret_cast<uint32_t*>(ownerRef);
            reinterpret_cast<uint32_t*>(ownerAfter[1])[1] =
                CSF_AT(uint32_t, result, 0x78);
        }
    }
    return result;
}

HOOK(0x45C970, ResolveHandleIntoOwnerSlot, "auto");
