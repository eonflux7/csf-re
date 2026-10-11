#include <cstddef>
#include "globals.h"
#include <cstdint>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, GfxSceneNode_Construct, 0x006F5630, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxSceneNode_LinkChild, 0x006F5A40, (void* self, void* edx, int a2));

// Builds the streaming scene node for this entry: queries two child objects, allocates and
// constructs an 80-byte scene node, links both children into it, then calls its slot 3 with 128.
static int __fastcall C_00860bb8_InitStreamingNode(C_00860bb8* self, void* /*edx*/) {
    // First child: getter at vtable slot 0x30 (index 12).
    void* firstObject = reinterpret_cast<void*>(self->field_0008);
    int firstValue = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, firstObject, 0)[0x30 / 4])(firstObject, nullptr);

    // Second child: getter at vtable slot 0x10 (index 4).
    void* secondObject = reinterpret_cast<void*>(self->field_000c);
    int secondValue = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, secondObject, 0)[0x10 / 4])(secondObject, nullptr);

    // Allocate and construct the scene node (null if allocation fails).
    void* allocated = Mem_Alloc(0x50);
    uint32_t* sceneNode = nullptr;
    if (allocated) {
        sceneNode = GfxSceneNode_Construct(allocated, nullptr);
    }
    self->field_0010 = reinterpret_cast<uint32_t>(sceneNode);

    GfxSceneNode_LinkChild(sceneNode, nullptr, firstValue);
    GfxSceneNode_LinkChild(reinterpret_cast<void*>(self->field_0010), nullptr, secondValue);

    // Final virtual call on the scene node, slot 0xC (index 3), with a flag value of 128.
    void* node = reinterpret_cast<void*>(self->field_0010);
    int flags = 0x80;
    return reinterpret_cast<int(__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, node, 0)[0x0C / 4])(node, nullptr, &flags);
}

HOOK(0x006CA970, C_00860bb8_InitStreamingNode, "auto");
