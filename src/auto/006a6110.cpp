#include <cstdint>
#include "globals.h"
#include "hook.h"

// Stored callback: a function pointer at +0x24 and its user-data argument at +0x28.
typedef int(__cdecl* StoredCallbackFn)(uint32_t userData, int argument);

static int __cdecl InvokeStoredCallback(int a1, int a2) {
    void* context = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));
    StoredCallbackFn callback = reinterpret_cast<StoredCallbackFn>(CSF_AT(uint32_t, context, 0x24));
    uint32_t userData = CSF_AT(uint32_t, context, 0x28);
    return callback(userData, a1);
}

HOOK(0x6A6110, InvokeStoredCallback, "auto");
