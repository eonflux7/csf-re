#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __cdecl, NetSession_GetNodeAllocator, 0x00646FF0, ());
CSF_FUNC(void, __cdecl, NetSession_FreeListNode, 0x006470E0,
         (int a1, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, NetSession_AssignObjectReference, 0x0066F130,
         (void* self, void* edx, char* a2));
CSF_FUNC(void, __fastcall, NetSession_ReleaseObjectReference, 0x0066F150,
         (void* self, void* edx));
CSF_FUNC(bool, __fastcall, NetSession_MatchesObjectReference, 0x0066F1A0,
         (void* self, void* edx, void** a2));
CSF_FUNC(void, __fastcall, NetSession_DestroyNodeObject, 0x00680CA0,
         (void* self, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0,
         (void* self, void* edx));

static void __fastcall NetSession_RemoveWorldObjectByReference(
    void* self, void* /*edx*/, void** objectReference)
{
    void* sentinel = CSF_AT(void*, self, 0xA0);
    void* node = CSF_AT(void*, sentinel, 0);

    if (node == sentinel)
        return;

    // The original by-value reference occupies one stack word.
    void** requestedReference = objectReference;
    for (;;)
    {
        int collection = SceneLoader_GetWorldObjectCollection(
            static_cast<uint8_t*>(node) + 8, nullptr);
        NetSession_AssignObjectReference(
            &objectReference, nullptr, reinterpret_cast<char*>(collection));

        if (NetSession_MatchesObjectReference(
                &objectReference, nullptr, requestedReference))
            break;

        NetSession_ReleaseObjectReference(&objectReference, nullptr);
        node = CSF_AT(void*, node, 0);
        if (node == CSF_AT(void*, self, 0xA0))
            return;
    }

    if (node != CSF_AT(void*, self, 0xA0))
    {
        CSF_AT(void*, CSF_AT(void*, node, 4), 0) =
            CSF_AT(void*, node, 0);
        CSF_AT(void*, CSF_AT(void*, node, 0), 4) =
            CSF_AT(void*, node, 4);

        NetSession_DestroyNodeObject(static_cast<uint8_t*>(node) + 8, 0);
        uint32_t* allocator = NetSession_GetNodeAllocator();
        NetSession_FreeListNode(
            reinterpret_cast<int>(allocator), reinterpret_cast<int>(node), 7);
        --CSF_AT(uint32_t, self, 0xA4);
    }

    NetSession_ReleaseObjectReference(&objectReference, nullptr);
}

HOOK(0x0073D410, NetSession_RemoveWorldObjectByReference, "auto");
