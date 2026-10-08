#include <cstdint>
#include "globals.h"
#include "hook.h"

// Mem_FreeDebug: game debug free, cdecl, pops 0 bytes.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x690E60, (void* block));

// Refreshes the enable/visibility state of the CMenuEula child objects from
// the counter at +0x38 and the limit at +0x50, then returns the result of the
// last child call. Returns 0 (ret 0): __thiscall with no stack arguments.
static void* __fastcall CMenuEula_RefreshState(void* self, void* /*edx*/)
{
    // Locals mirroring the original frame: v7 (scratch out-buffer), v10 (cleanup
    // counter, always 7 here), Block (never assigned a non-zero value).
    uint32_t scratch = 0;
    uint32_t cleanupCounter = 7;
    void* block = nullptr;

    // Step 1: child at +0x2C, method slot 0x68 (index 26), args (flag, 1, 1).
    // The flag is 'counter > 0' in the low byte; the upper 24 bits of the
    // counter itself are passed through, as the original pushes the full edx.
    {
        const int32_t counter = CSF_AT(int32_t, self, 0x38);
        void* firstChild = CSF_AT(void*, self, 0x2C);
        void** firstVtbl = CSF_AT(void**, firstChild, 0);
        const uint32_t firstArg =
            (static_cast<uint32_t>(counter) & 0xFFFFFF00u) | (counter > 0 ? 1u : 0u);
        reinterpret_cast<void* (__fastcall*)(void*, void*, uint32_t, int, int)>(
            firstVtbl[0x68 / 4])(firstChild, nullptr, firstArg, 1, 1);
    }

    // Step 2: child at +0x34, method slot 0x24C (index 147), args (&scratch, 0).
    // Returns a record pointer whose int at +0x14 is the record count.
    void* secondChild = CSF_AT(void*, self, 0x34);
    void** secondVtbl = CSF_AT(void**, secondChild, 0);
    void* countRecord = reinterpret_cast<void* (__fastcall*)(void*, void*, void*, int)>(
        secondVtbl[0x24C / 4])(secondChild, nullptr, &scratch, 0);
    const int32_t recordCount = CSF_AT(int32_t, countRecord, 0x14);

    // Step 3: flag = recordCount > 0 && counter != limit - recordCount.
    // Both fields are re-read here, after the calls above, as the original does.
    bool flag = false;
    if (recordCount > 0)
    {
        const uint32_t remaining = static_cast<uint32_t>(CSF_AT(int32_t, self, 0x50)) -
                                   static_cast<uint32_t>(recordCount);
        flag = CSF_AT(int32_t, self, 0x38) != static_cast<int32_t>(remaining);
    }

    // Step 4: child at +0x30, method slot 0x68 (index 26), args (arg, 1, 1).
    // arg keeps the upper 24 bits of recordCount and carries the flag in the low byte.
    void* thirdChild = CSF_AT(void*, self, 0x30);
    void** thirdVtbl = CSF_AT(void**, thirdChild, 0);
    const uint32_t thirdArg =
        (static_cast<uint32_t>(recordCount) & 0xFFFFFF00u) | (flag ? 1u : 0u);
    void* result = reinterpret_cast<void* (__fastcall*)(void*, void*, uint32_t, int, int)>(
        thirdVtbl[0x68 / 4])(thirdChild, nullptr, thirdArg, 1, 1);

    CSF_AT(int32_t, self, 0x18) = -1;

    // Cleanup path: only when the cleanup counter is >= 8 (unsigned) and block is non-null.
    if (cleanupCounter >= 8 && block != nullptr)
    {
        return reinterpret_cast<void*>(static_cast<intptr_t>(Mem_FreeDebug(block)));
    }
    return result;
}

HOOK(0x00532E40, CMenuEula_RefreshState, "auto");
