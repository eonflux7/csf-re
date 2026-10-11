#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(bool, __fastcall, Scene_CollectSubtreeIntoBuffer, 0x006BCA60, (void* self, void* /*edx*/, void* callback, int outBuffer));
CSF_FUNC(int, __fastcall, ResolveAnimatedSceneObject, 0x006CB8C0, (void* self, void* /*edx*/, int a2));

static int __fastcall SceneMissionSetup_CollectAnimatedSceneObjects(C_00860bb8* self, void* /*edx*/)
{
    // Stack buffer handed to Scene_CollectSubtreeIntoBuffer (0x14 bytes):
    // word 0 = element array pointer, word 2 = element count,
    // words 3/4 = the original's spill slots (hint 10, this).
    alignas(8) uint8_t collectBytes[0x14];
    uint32_t* collectWords = reinterpret_cast<uint32_t*>(collectBytes);
    collectWords[0] = 0;
    collectWords[1] = 0;
    collectWords[2] = 0;
    collectWords[3] = 10;
    collectWords[4] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self));

    Scene_CollectSubtreeIntoBuffer(reinterpret_cast<void*>(self->field_0008), nullptr,
        reinterpret_cast<void*>(CSF_ADDR_FN(0x006CCD30)),
        static_cast<int>(reinterpret_cast<uintptr_t>(collectBytes)));

    if (collectWords[2] != 0)
    {
        S_0041e5f0* arrayField = reinterpret_cast<S_0041e5f0*>(&self->field_0054);
        uint32_t index = 0;
        while (true)
        {
            int sceneHandle = ResolveAnimatedSceneObject(self, nullptr,
                static_cast<int>(reinterpret_cast<uint32_t*>(collectWords[0])[index]));

            if (arrayField->field_0008 >= arrayField->field_0004)
            {
                (void)CSF_VTBL(0x0041E5F0);
                S_0041e5f0_GrowArrayCapacity(arrayField, nullptr, static_cast<int>(arrayField->field_000c));
            }

            uint32_t slot = arrayField->field_0008;
            int* outSlot = reinterpret_cast<int*>(arrayField->field_0000 + 4 * slot);
            arrayField->field_0008 = slot + 1;
            *outSlot = sceneHandle;

            if (++index >= collectWords[2])
                break;
        }
    }

    uint32_t bufferPtr = collectWords[0];
    if (bufferPtr != 0)
        return Mem_Free(reinterpret_cast<void*>(bufferPtr));
    return static_cast<int>(bufferPtr);
}

HOOK(0x006CCD70, SceneMissionSetup_CollectAnimatedSceneObjects, "auto");
