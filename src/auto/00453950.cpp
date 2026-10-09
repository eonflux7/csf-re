#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_RemoveObjectReference, 0x00453560, (void* self, void* edx, int a2));

static int __fastcall CScene_ReleaseAllObjectReferences(C_008312b0* self, void* /*edx*/)
{
    uint32_t objectCount = CSF_AT(uint32_t, self, 0x38);
    if (objectCount)
    {
        for (int i = (int)objectCount - 1; i >= 0; --i)
        {
            uint32_t objectArray = CSF_AT(uint32_t, self, 0x30);
            uint32_t object = reinterpret_cast<uint32_t*>(objectArray)[i];
            SceneLoader_RemoveObjectReference(self, nullptr, (int)(object + 0x1c));
        }
        CSF_AT(uint32_t, self, 0x38) = 0;
    }

    int result = (int)CSF_AT(uint32_t, self, 0x48);
    if (result)
    {
        for (int j = result - 1; j >= 0; --j)
        {
            uint32_t refArray = CSF_AT(uint32_t, self, 0x40);
            uint32_t object = reinterpret_cast<uint32_t*>(refArray)[j];
            result = SceneLoader_RemoveObjectReference(self, nullptr, (int)(object + 0x1c));
        }
        CSF_AT(uint32_t, self, 0x48) = 0;
    }
    return result;
}

HOOK(0x00453950, CScene_ReleaseAllObjectReferences, "auto");
