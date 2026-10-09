#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __cdecl SceneLoader_ProcessDefaultResources()
{
    void* manager = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FD0);
    static constexpr int resourceIds[] = {
        2, 3, 4, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100
    };

    int result = 0;
    for (int resourceId : resourceIds)
    {
        void* resourceStore = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        void** vtable = CSF_AT(void**, manager, 0);
        int resource = SceneLoader_GetResourceById(resourceStore, nullptr, resourceId);
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            vtable[28 / 4])(manager, nullptr, resource);
    }
    return result;
}

HOOK(0x004FE130, SceneLoader_ProcessDefaultResources, "auto");
