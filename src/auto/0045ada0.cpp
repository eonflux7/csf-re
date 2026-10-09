#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

static int __stdcall ResolveAndCacheObjectField(int outputHolder, int /*unusedA2*/, int keyRef)
{
    // Clear the cached field of the holder's target before resolving.
    uintptr_t holderTarget = CSF_AT(uint32_t, outputHolder, 4);
    CSF_AT(uint32_t, holderTarget, 4) = 0;

    // Service object lives at gameRoot + 0x1FC8.
    uintptr_t gameRoot = (uintptr_t)GetValue8B0554();
    void* service = reinterpret_cast<void*>(CSF_AT(uint32_t, gameRoot, 0x1FC8));

    // Key is *(*(*keyRef) + 4) + 4 from the chained source objects.
    uintptr_t keyObj = CSF_AT(uint32_t, keyRef, 0);
    uintptr_t keyNode = CSF_AT(uint32_t, keyObj, 4);
    int key = (int)CSF_AT(uint32_t, keyNode, 4);

    // Slot 4 (offset 0x10): lookup by key, returns handle or -1.
    void** vtbl = CSF_AT(void**, service, 0);
    int result = ((int(__fastcall*)(void*, void*, int))vtbl[4])(service, nullptr, key);

    if (result != -1)
    {
        // Slot 2 (offset 0x08): resolve handle to object.
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, service, 0)[2])(service, nullptr, result);

        // Reload the holder's target after the call and store the object's field at +0xBC.
        uintptr_t resolvedObj = (uintptr_t)result;
        uint32_t fieldValue = CSF_AT(uint32_t, resolvedObj, 0xBC);
        CSF_AT(uint32_t, CSF_AT(uint32_t, outputHolder, 4), 4) = fieldValue;
    }

    return result;
}

HOOK(0x0045ADA0, ResolveAndCacheObjectField, "auto");
