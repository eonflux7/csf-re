#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));

static void __fastcall S_004686a0_RegisterScriptInIndexLists(
    S_004686a0* self, void* /*edx*/, int scriptAddress)
{
    // The packet additionally requires this address as a vtable binding.
    (void)CSF_VTBL(0x0041E5F0);

    if (!scriptAddress)
        return;

    auto* script = reinterpret_cast<void*>(
        static_cast<uint32_t>(scriptAddress));
    auto* indices = reinterpret_cast<void*>(
        CSF_AT(uint32_t, script, 0x10));

    const uint32_t begin = CSF_AT(uint32_t, indices, 0x0C);
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(
                  CSF_AT(uint32_t, indices, 0x10) - begin) >> 3)
        : 0;

    for (uint32_t index = 0; index < count; ++index)
    {
        const auto* entries = reinterpret_cast<const uint8_t*>(
            CSF_AT(uint32_t, indices, 0x0C));
        const int32_t key = CSF_AT(int32_t, entries + index * 8, 0);

        if (key == -1)
            continue;

        auto* lists = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, self, 0x68));

        S_0041e5f0* list;
        if ((key & 0x8000000) != 0)
            list = reinterpret_cast<S_0041e5f0*>(lists + 0x10);
        else
            list = reinterpret_cast<S_0041e5f0*>(
                lists + (static_cast<uint32_t>(key) << 4));

        if (list->field_0008 >= list->field_0004)
            GrowButtonArray(list, nullptr, list->field_000c);

        const uint32_t size = list->field_0008;
        auto* destination = reinterpret_cast<int*>(
            list->field_0000 + 4 * size);
        list->field_0008 = size + 1;
        *destination = scriptAddress;
    }
}

HOOK(0x00468BB0, S_004686a0_RegisterScriptInIndexLists, "auto");
