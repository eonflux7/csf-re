#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, SceneObject_ResetAttachedColorOverride, 0x004CF850, (void* self, void* edx));

static int __fastcall SceneObject_ResetColorOverride(void* self, void* /*edx*/)
{
    void* colorOverride = CSF_AT(void*, self, 0x16C);
    if (!colorOverride)
        return 0;

    int result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, colorOverride, 0)[0x58 / 4])(colorOverride, nullptr);

    if (CSF_AT(int, self, 0x50) == 32)
        return SceneObject_ResetAttachedColorOverride(
            CSF_AT(void*, self, 0x34), nullptr);

    return result;
}

HOOK(0x0048E830, SceneObject_ResetColorOverride, "auto");
