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

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(void*, __fastcall, ResetPairAndCopyEndpoints, 0x00462AE0, (void* self, void* , const void* source));
CSF_FUNC(int, __fastcall, PairLink_PrepareForRebind, 0x004E55A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, DecrementByteCounter, 0x004E55B0, (void* self, void*));
CSF_FUNC(char, __fastcall, PairLink_TestEndpoints, 0x004E5B60, (void* self, void* edx, int a2));

// Converts an integer-held address back to a pointer (32-bit target).
static void* AsPointer(int32_t value)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(value)));
}

static char __stdcall PairLink_ResolveTargetAndRebind(int a1, int /*a2*/, uint32_t* a3)
{
    // Clear the flag byte on the owner (ctx->owner + 1).
    uint8_t* owner = CSF_AT(uint8_t*, AsPointer(a1), 4);
    owner[1] = 0;

    // First argument chain: a3[0] -> inner -> handle-like field.
    void* firstArgument = reinterpret_cast<void*>(static_cast<uintptr_t>(a3[0]));
    int32_t v3 = CSF_AT(int32_t, firstArgument, 4);
    int32_t v4 = CSF_AT(int32_t, AsPointer(v3), 12);
    int32_t handle = v3 + 4;

    // Second argument chain: a3[1] -> inner -> endpoint block.
    void* secondArgument = reinterpret_cast<void*>(static_cast<uintptr_t>(a3[1]));
    int32_t secondInner = CSF_AT(int32_t, secondArgument, 4);
    int32_t v6 = secondInner + 4;

    // Default result is the handle value itself (low byte is returned).
    int resolvedOrHandle = handle;

    if (v4 >= 0 && CSF_AT(int32_t, AsPointer(v6), 8) >= 0)
    {
        resolvedOrHandle = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
        if (resolvedOrHandle != 0)
        {
            C_0083e9d0* target = CSF_AT(C_0083e9d0*, AsPointer(resolvedOrHandle), 0x3C);
            if (target != nullptr)
            {
                PairLink_PrepareForRebind(target, nullptr);

                // The owner pointer is reloaded after the callee above.
                uint8_t* ownerAfterPrepare = CSF_AT(uint8_t*, AsPointer(a1), 4);

                alignas(4) uint8_t pairScratch[0x14];
                int32_t endpoints = static_cast<int32_t>(
                    reinterpret_cast<uintptr_t>(
                        ResetPairAndCopyEndpoints(static_cast<void*>(pairScratch), nullptr, AsPointer(v6))));

                char testResult = PairLink_TestEndpoints(target, nullptr, endpoints);
                ownerAfterPrepare[1] = static_cast<uint8_t>(testResult);

                char decrementResult = DecrementByteCounter(target, nullptr);
                resolvedOrHandle = (resolvedOrHandle & ~0xFF) | static_cast<uint8_t>(decrementResult);
            }
        }
    }

    return static_cast<char>(resolvedOrHandle);
}

HOOK(0x00463500, PairLink_ResolveTargetAndRebind, "auto");
