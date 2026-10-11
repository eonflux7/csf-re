#include <cstdint>
#include "globals.h"
#include "hook.h"

// zlib-stream style reset: clears the stream state block `a1` and, when the
// caller-supplied `a2` context needs it, runs the registered callbacks.
// `a3` (optional) receives the stored result from a1+0x3C.

CSF_FUNC(int, __cdecl, sub_6A6110, 0x006A6110, (int a1, int a2));

static int __cdecl ZlibStream_ResetState(void* a1, void* a2, uint32_t* a3)
{
    if (a3)
        *a3 = CSF_AT(uint32_t, a1, 0x3C);

    if (CSF_AT(uint32_t, a1, 0x00) == 4 || CSF_AT(uint32_t, a1, 0x00) == 5)
    {
        auto notify = reinterpret_cast<void(__cdecl*)(uint32_t, uint32_t)>(
            static_cast<uintptr_t>(CSF_AT(uint32_t, a2, 0x24)));
        notify(CSF_AT(uint32_t, a2, 0x28), CSF_AT(uint32_t, a1, 0x0C));
    }

    if (CSF_AT(uint32_t, a1, 0x00) == 6)
        sub_6A6110(static_cast<int>(CSF_AT(uint32_t, a1, 0x04)), static_cast<int>(reinterpret_cast<uintptr_t>(a2)));

    uint32_t savedValue = CSF_AT(uint32_t, a1, 0x28);
    CSF_AT(uint32_t, a1, 0x34) = savedValue;
    CSF_AT(uint32_t, a1, 0x30) = savedValue;

    uint32_t callback = CSF_AT(uint32_t, a1, 0x38);
    CSF_AT(uint32_t, a1, 0x00) = 0;
    CSF_AT(uint32_t, a1, 0x1C) = 0;
    CSF_AT(uint32_t, a1, 0x20) = 0;

    int result = static_cast<int>(callback);
    if (callback)
    {
        auto run = reinterpret_cast<int(__cdecl*)(uint32_t, uint32_t, uint32_t)>(
            static_cast<uintptr_t>(callback));
        result = run(0, 0, 0);
        CSF_AT(uint32_t, a1, 0x3C) = static_cast<uint32_t>(result);
        CSF_AT(uint32_t, a2, 0x30) = static_cast<uint32_t>(result);
    }
    return result;
}

HOOK(0x6A4DF0, ZlibStream_ResetState, "auto");
