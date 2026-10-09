#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(void*, __fastcall, ResetPairAndCopyEndpoints, 0x00462AE0, (void* self, void* , const void* source));
CSF_FUNC(int, __fastcall, PairLink_PrepareForRebind, 0x004E55A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, DecrementByteCounter, 0x004E55B0, (void* self, void*));
CSF_FUNC(char, __fastcall, PairLink_TestEndpoints, 0x004E5B60, (void* self, void* edx, int a2));

// Resolves a handle from the current scene, and if the resolved object has a link target,
// rebinds it to the endpoints of pair B, stores the result flag into a1's owner, and releases
// the target via DecrementByteCounter. Returns the low byte of the last computed value.
static char __stdcall PairLink_ResolveTargetAndRebind(int a1, int a2, uint32_t* a3)
{
    uint8_t* ctx = reinterpret_cast<uint8_t*>(a1);

    // Clear the flag byte on the owner object referenced by a1+4.
    uint8_t* ownerFirst = reinterpret_cast<uint8_t*>(*CSF_AT(uint32_t*, ctx, 4));
    *CSF_AT(uint8_t*, ownerFirst, 1) = 0;

    // Pair A: handle source.
    uint8_t* pairA = reinterpret_cast<uint8_t*>(a3[0]);
    uint32_t pairAInner = *CSF_AT(uint32_t*, pairA, 4);
    int32_t pairAFlag = *CSF_AT(int32_t*, reinterpret_cast<uint8_t*>(pairAInner), 12);
    int handleValue = static_cast<int>(pairAInner + 4);

    // Pair B: endpoint source.
    uint8_t* pairB = reinterpret_cast<uint8_t*>(a3[1]);
    uint32_t pairBInner = *CSF_AT(uint32_t*, pairB, 4);
    uint32_t endpointSource = pairBInner + 4;

    int returnValue = handleValue;

    if (pairAFlag >= 0 && *CSF_AT(int32_t*, reinterpret_cast<uint8_t*>(pairBInner), 12) >= 0)
    {
        int resolvedEntry = CHandleTable_Resolve(g_pCurrentScene, nullptr, handleValue);
        returnValue = resolvedEntry;

        if (resolvedEntry)
        {
            C_0083e9d0* linkObj = reinterpret_cast<C_0083e9d0*>(
                *CSF_AT(uint32_t*, reinterpret_cast<uint8_t*>(resolvedEntry), 60));

            if (linkObj)
            {
                PairLink_PrepareForRebind(linkObj, nullptr);

                // Owner is reloaded after the call; its value is kept for the final store.
                uint8_t* ownerAfterPrepare = reinterpret_cast<uint8_t*>(*CSF_AT(uint32_t*, ctx, 4));

                uint8_t pairEndpoints[0x14];
                void* copiedEndpoints = ResetPairAndCopyEndpoints(
                    pairEndpoints, nullptr, reinterpret_cast<const void*>(endpointSource));

                char linkResult = PairLink_TestEndpoints(
                    linkObj, nullptr,
                    static_cast<int>(reinterpret_cast<uintptr_t>(copiedEndpoints)));

                *CSF_AT(uint8_t*, ownerAfterPrepare, 1) = static_cast<uint8_t>(linkResult);

                returnValue = DecrementByteCounter(linkObj, nullptr);
            }
        }
    }

    return static_cast<char>(returnValue);
}

HOOK(0x00463500, PairLink_ResolveTargetAndRebind, "auto");
