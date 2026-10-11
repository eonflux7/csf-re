#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "core/memory.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, sub_6AEF20, 0x006AEF20, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_6AF450, 0x006AF450, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Lazily resolves the scene child at scene+0x130 (its vtable slot 7 yields the cached
// object stored at +0x14), then walks the 12-byte entries at +0x04 (count at +0x0C),
// invoking sub_6AF450 on each with the cached object's +4 sub-object as self.
static int __fastcall S_0041e750_UpdateSceneEntries(S_0041e750* self, void* /*edx*/)
{
    if (self->field_0014 == 0)
    {
        void* sceneChild = CSF_AT(void*, g_pCurrentScene, 0x130);
        int resolved = ((int(__fastcall*)(void*, void*))CSF_AT(void**, sceneChild, 0)[7])(sceneChild, nullptr);
        self->field_0014 = (uint32_t)resolved;
    }

    int result = sub_6AEF20(reinterpret_cast<void*>(self->field_0014 + 4), nullptr);

    uint32_t remaining = self->field_000c;
    if (remaining)
    {
        uint32_t offset = 0;
        do
        {
            int entryArgument = (int)(self->field_0004 + offset);
            sub_6AF450(reinterpret_cast<void*>(self->field_0014 + 4), nullptr, entryArgument);
            offset += 12;
            --remaining;
        } while (remaining);
    }

    return result;
}

HOOK(0x0041E750, S_0041e750_UpdateSceneEntries, "auto");
