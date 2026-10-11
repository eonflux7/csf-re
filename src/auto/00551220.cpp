#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_73F750, 0x0073F750, (void* self, void* edx));

// Resolves a scene handle (offset +8 from the argument) through the scene's handle table,
// then runs sub_73F750 on the entry's embedded sub-object and sets the state field
// of the entry's linked owner to 3 when that owner exists.
static int __stdcall MpSpawn_ResolveHandleAndSetState3(int handle)
{
    int result = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle + 8);
    if (result)
    {
        void* entry = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(result)));
        int ownerPtr = CSF_AT(int, entry, 0x18);
        void* embedded = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(result) + 0x14));
        result = sub_73F750(embedded, nullptr);
        if (ownerPtr)
        {
            void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(ownerPtr)));
            CSF_AT(int, owner, 0x10) = 3;
        }
    }
    return result;
}

HOOK(0x551220, MpSpawn_ResolveHandleAndSetState3, "auto");
