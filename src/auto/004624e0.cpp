#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, InitializeTransientState, 0x004619C0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, CollectLinkedObjectPointers, 0x00468A00, (void* self, void* edx, int a2, void* a3));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resolves the handle stored in the node reached through *ownerRef, then collects the
// scene objects linked to that resolved object. Every collected object other than
// self gets its transient state reset and its +128 flag byte cleared. The collected
// list is freed afterwards. Returns the handle address when the node count is negative,
// otherwise 0 (or the result of Mem_Free when a list was allocated).
static int __fastcall ResetLinkedSceneObjectsExceptSelf(void* self, void* /*edx*/,
                                                        int /*a2*/, int /*a3*/, void* ownerRef)
{
    void* recordPtr = reinterpret_cast<void*>(CSF_AT(uint32_t, ownerRef, 0));
    uint32_t ownerNodeAddr = CSF_AT(uint32_t, recordPtr, 4);
    void* ownerNode = reinterpret_cast<void*>(ownerNodeAddr);
    int32_t nodeCount = CSF_AT(int32_t, ownerNode, 12);
    uint32_t handleAddr = ownerNodeAddr + 4;

    if (nodeCount < 0)
        return static_cast<int>(handleAddr);

    int resolvedObject = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleAddr));
    if (resolvedObject == 0)
        return resolvedObject;

    // Output list header: [0] item array, [1] unused, [2] item count, [3] capacity (10).
    uint32_t neighborList[4];
    neighborList[0] = 0;
    neighborList[1] = 0;
    neighborList[2] = 0;
    neighborList[3] = 10;

    if (CollectLinkedObjectPointers(CSF_AT(void*, g_pCurrentScene, 0xBC), nullptr,
                                    resolvedObject, neighborList))
    {
        uint32_t neighborCount = neighborList[2];
        for (uint32_t index = 0; index < neighborCount; ++index)
        {
            uint32_t* neighborArray = reinterpret_cast<uint32_t*>(neighborList[0]);
            void* neighbor = reinterpret_cast<void*>(neighborArray[index]);
            if (neighbor != nullptr && neighbor != self)
            {
                InitializeTransientState(neighbor, nullptr);
                CSF_AT(uint8_t, neighbor, 128) = 0;
            }
        }
    }

    if (neighborList[0] != 0)
        return Mem_Free(reinterpret_cast<void*>(neighborList[0]));

    return 0;
}

HOOK(0x4624E0, ResetLinkedSceneObjectsExceptSelf, "auto");
