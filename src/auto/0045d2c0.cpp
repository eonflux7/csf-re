#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee at 0x468750: __fastcall, self in ecx, one stack arg (pushed key), returns int.
CSF_FUNC(int, __fastcall, FindSceneNodeByKey, 0x00468750, (void* self, void* edx, int a2));

// Current scene global (void*).
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Vtable of the scene node type returned by FindSceneNodeByKey.
[[maybe_unused]] static auto kSceneNodeVtbl = CSF_VTBL(0x00468750);

// __stdcall, 3 stack args, ret 12.
static int __stdcall LookupSceneEntryAndCopyFlag(int ownerRef, int unusedArg, int keyRef)
{
    (void)unusedArg;

    // Clear the flag byte in the owner's state block before the lookup.
    CSF_AT(uint8_t, CSF_AT(uint8_t*, ownerRef, 0x4), 0x1) = 0;

    // keyRef -> header -> record -> key
    uint32_t header = CSF_AT(uint32_t, keyRef, 0x0);
    uint32_t record = CSF_AT(uint32_t, header, 0x4);
    uint32_t key = CSF_AT(uint32_t, record, 0x4);

    // Scene's node table lives at +0xBC of the current scene.
    void* scene = g_pCurrentScene;
    void* sceneNodes = CSF_AT(void*, scene, 0xBC);

    int found = FindSceneNodeByKey(sceneNodes, nullptr, static_cast<int>(key));

    if (found != 0)
    {
        uint8_t* foundNode = reinterpret_cast<uint8_t*>(static_cast<uint32_t>(found));
        uint8_t flagValue = CSF_AT(uint8_t, foundNode, 0x80);
        // Reload owner's state pointer after the call.
        CSF_AT(uint8_t, CSF_AT(uint8_t*, ownerRef, 0x4), 0x1) = flagValue;
    }

    return found;
}

HOOK(0x0045D2C0, LookupSceneEntryAndCopyFlag, "auto");
