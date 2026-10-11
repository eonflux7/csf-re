#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Resolves requestedKey to an id through the resolver object obtained from field_0008,
// then fetches the value for that id from the same helper. Returns 0 when the id is not found.
static int __fastcall C_0082d5e0_ResolveIdThenGetValue(C_0082d5e0* self, void* /*edx*/, int requestedKey)
{
    // Get the resolver object: helper->vtbl[0x28 / 4]()
    void* helper = reinterpret_cast<void*>(self->field_0008);
    void* resolver = reinterpret_cast<void*>(
        ((uint32_t(__fastcall*)(void*, void*))CSF_AT(void**, helper, 0)[0x28 / 4])(helper, nullptr));

    // Resolve the key to an id: resolver->vtbl[0x98 / 4](requestedKey)
    int resolvedId = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, resolver, 0)[0x98 / 4])(
        resolver, nullptr, requestedKey);

    if (resolvedId == -1)
        return 0;

    // Re-read the helper (the original reloads field_0008 here) and fetch its resolver again.
    helper = reinterpret_cast<void*>(self->field_0008);
    void* valueSource = reinterpret_cast<void*>(
        ((uint32_t(__fastcall*)(void*, void*))CSF_AT(void**, helper, 0)[0x28 / 4])(helper, nullptr));

    // Return valueSource->vtbl[0x8C / 4](resolvedId)
    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, valueSource, 0)[0x8C / 4])(
        valueSource, nullptr, resolvedId);
}

HOOK(0x407EE0, C_0082d5e0_ResolveIdThenGetValue, "auto");
