#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee at 0x43F310: __fastcall, ecx = list head node, one stack arg (ret 4).
CSF_FUNC(char, __fastcall, ResourceList_InitFromBlock, 0x0043F310, (void* self, void* edx, void** Block));

// __thiscall method: `this` is the Scene loader object, whose +0x08 holds a
// pointer to a record whose +0x04 is the block pointer passed to the callee.
static uint32_t* __fastcall SceneLoader_PrepareResourceManager(uint32_t* self, void* /*edx*/)
{
    // v2 = this + 1 (sentinel list node embedded at this+4)
    uint32_t* listHead = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 4);

    // sub_43F310(*(void ***)(*(this + 2) + 4))
    void** block = CSF_AT(void**, reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x08))), 4);
    ResourceList_InitFromBlock(listHead, nullptr, block);

    // Re-read the list node after the call; the callee may have changed it.
    uint32_t nodeAddr = CSF_AT(uint32_t, self, 0x08);
    uint8_t* node = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(nodeAddr));

    CSF_AT(uint32_t, node, 4) = nodeAddr;       // node->next = node
    CSF_AT(uint32_t, listHead, 4) = 0;          // v2[2] = 0
    CSF_AT(uint32_t, node, 0) = nodeAddr;       // node->prev = node
    CSF_AT(uint32_t, node, 8) = nodeAddr;       // node->owner = node
    CSF_AT(uint32_t, self, 0x1C) = 0xFFFFFFFFu; // this->field_001C = -1

    return reinterpret_cast<uint32_t*>(node);
}

HOOK(0x0043FBA0, SceneLoader_PrepareResourceManager, "auto");

