#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));

// Destroys the text controller owned by a HUD object: releases the two
// virtual-owned sub-objects (slot 0 with arg 1, i.e. deleting destructor),
// drops one reference on the ref-counted object at +0x44, and frees the
// buffer at +0x34. Returns the result of the last Mem_Free call, or 0.
static int __fastcall Hud_DestroyTextController(int* self, void* /*edx*/)
{
    uint8_t* selfBytes = reinterpret_cast<uint8_t*>(self);

    // First owned object at +0x30: delete via vtable slot 0.
    uint8_t* firstObject = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, selfBytes, 0x30));
    if (firstObject != nullptr)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, firstObject, 0)[0])(firstObject, nullptr, 1);
        CSF_AT(uint32_t, selfBytes, 0x30) = 0;
    }

    // Second owned object at +0xD0: same pattern.
    uint8_t* secondObject = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, selfBytes, 0xD0));
    if (secondObject != nullptr)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, secondObject, 0)[0])(secondObject, nullptr, 1);
        CSF_AT(uint32_t, selfBytes, 0xD0) = 0;
    }

    // Ref-counted object at +0x44: release through its embedded sub-object at +4.
    uint32_t refCountedObject = CSF_AT(uint32_t, selfBytes, 0x44);
    if (refCountedObject != 0)
    {
        ReleaseObjectReference(reinterpret_cast<void*>(refCountedObject + 4), nullptr);
        CSF_AT(uint32_t, selfBytes, 0x44) = 0;
    }

    // Heap buffer at +0x34: free with the game heap.
    int result = 0;
    uint32_t buffer = CSF_AT(uint32_t, selfBytes, 0x34);
    if (buffer != 0)
    {
        result = Mem_Free(reinterpret_cast<void*>(buffer));
        CSF_AT(uint32_t, selfBytes, 0x34) = 0;
    }

    return result;
}

HOOK(0x439AC0, Hud_DestroyTextController, "auto");
