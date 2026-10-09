#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Mem_Free (game heap free), bound by address.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destroys the object referenced from a node's +4 slot (original sub_4714E0).
CSF_FUNC(int, __cdecl, MissionScript_DestroyNode, 0x004714E0, (void* Block));

// Frees the node block at a1->[+4] and its owned object at node->[+4], then clears both links.
static int __stdcall MissionScript_FreeNodeLink(int a1)
{
    int result = 0;
    uint8_t* node = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, a1, 4));
    if (node)
    {
        void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, node, 4));
        if (obj)
        {
            // Virtual slot 0 (destructor-style), called with argument 0.
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[0])(obj, nullptr, 0);
            MissionScript_DestroyNode(obj);
        }
        CSF_AT(uint32_t, node, 4) = 0;
        result = Mem_Free(node);
        CSF_AT(uint32_t, a1, 4) = 0;
    }
    return result;
}

HOOK(0x0046C460, MissionScript_FreeNodeLink, "auto");
