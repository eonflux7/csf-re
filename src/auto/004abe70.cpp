#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(bool, __fastcall, HasDifferentMatchingEntry, 0x004485A0, (void* self, void* , const void* reference, const void* key));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __stdcall AiCoverTasks_IsNotDifferentFromLinkedEntry(int linkedSource, int taskEntry)
{
    void* linkedSelf = reinterpret_cast<void*>(static_cast<intptr_t>(linkedSource));
    int linkedValue = GetLinkedObjectValue_4DB5C0(linkedSelf, nullptr);

    int ownerValue = 0;
    if (linkedValue)
        ownerValue = linkedValue - 4;
    else
        ownerValue = 0;

    uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<intptr_t>(ownerValue));
    uint32_t referenceAddr = CSF_AT(uint32_t, owner, 0x2C) + 0x1C;
    const void* reference = reinterpret_cast<const void*>(static_cast<intptr_t>(referenceAddr));

    void* sceneChild = CSF_AT(void*, g_pCurrentScene, 0xC4);
    void* sceneSelf = reinterpret_cast<uint8_t*>(sceneChild) + 4;

    const void* key = reinterpret_cast<const void*>(static_cast<intptr_t>(taskEntry) + 0x1C);

    bool hasDifferentEntry = HasDifferentMatchingEntry(sceneSelf, nullptr, reference, key);
    return hasDifferentEntry ? 0 : 1;
}

HOOK(0x004ABE70, AiCoverTasks_IsNotDifferentFromLinkedEntry, "auto");
