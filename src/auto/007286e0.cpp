#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static std::uint32_t __fastcall C_00865870_SynchronizeLinkedObject(
    C_00865870* self, void* /*edx*/)
{
    void* source =
        ((void* (__fastcall*)(void*, void*))self->vtbl[6])(self, nullptr);

    if (!source)
        return 0;

    // The original tests only al here, but on the early return below it leaves the whole eax as the callee
    // returned it (`mov al, [esi+48]` then `je`), so the call keeps all 32 bits.
    const std::uint32_t enabledWord =
        ((std::uint32_t (__fastcall*)(void*, void*))
            CSF_AT(void**, source, 0)[47])(source, nullptr);
    const std::uint8_t enabled = static_cast<std::uint8_t>(enabledWord);

    if (!enabled && !self->field_0048)
        return enabledWord;

    void* linkedObject = reinterpret_cast<void*>(self->field_0030);
    if (!linkedObject)
        return 0;

    alignas(4) std::uint8_t scratch[8];

    void** sourceVtable = CSF_AT(void**, source, 0);
    void** linkedVtable = CSF_AT(void**, linkedObject, 0);

    const int firstValue =
        ((int (__fastcall*)(void*, void*, std::uint8_t*))
            sourceVtable[88])(source, nullptr, scratch);

    ((void (__fastcall*)(void*, void*, int))linkedVtable[7])(
        reinterpret_cast<void*>(self->field_0030), nullptr, firstValue);

    linkedObject = reinterpret_cast<void*>(self->field_0030);
    linkedVtable = CSF_AT(void**, linkedObject, 0);

    const int secondValue =
        ((int (__fastcall*)(void*, void*))
            CSF_AT(void**, source, 0)[41])(source, nullptr);

    ((void (__fastcall*)(void*, void*, int))linkedVtable[9])(
        reinterpret_cast<void*>(self->field_0030), nullptr, secondValue);

    linkedObject = reinterpret_cast<void*>(self->field_0030);
    return static_cast<std::uint32_t>(
        ((int (__fastcall*)(void*, void*))
            CSF_AT(void**, linkedObject, 0)[2])(linkedObject, nullptr));
}

HOOK(0x007286E0, C_00865870_SynchronizeLinkedObject, "auto");
