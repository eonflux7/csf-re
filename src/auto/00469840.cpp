#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound callees (addresses from the packet).
CSF_FUNC(int, __fastcall, SceneLoader_ResetPendingState, 0x00469370, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ReleaseSceneGroup, 0x0046F560, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_InitResourceEntry, 0x0046FA20, (void* self, void* edx));

// Original: __thiscall, no stack args (ret 0).
static int __fastcall SceneLoader_PrepareSceneResources(S_0045d610* self, void* /*edx*/)
{
    int result = SceneLoader_ResetPendingState(reinterpret_cast<void*>(self), nullptr);

    uint32_t group = self->field_0038;
    if (group)
        result = SceneLoader_ReleaseSceneGroup(reinterpret_cast<void*>(group), nullptr);

    uint32_t count = self->field_0020;
    for (uint32_t i = 0; i < count; ++i)
    {
        // The array base is reloaded every iteration: callees may change the object.
        result = static_cast<int>(self->field_0018);
        uint32_t entryValue = *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(result) + 4u * i);
        void* entry = reinterpret_cast<void*>(entryValue);
        if (entry)
            result = static_cast<int>(reinterpret_cast<uintptr_t>(
                SceneLoader_InitResourceEntry(entry, nullptr)));
    }
    return result;
}

HOOK(0x00469840, SceneLoader_PrepareSceneResources, "auto");
