#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, NetSession_GetNodeAllocator, 0x00646FF0, ());
CSF_FUNC(void, __cdecl, NetSession_FreeListNode, 0x006470E0, (int a1, int a2, int a3));

// Unlinks every node of the intrusive list whose sentinel head is stored at self[1].
// Each node is retyped to the vtable at 0x85FE1C, its temporary string is destroyed,
// and the node is returned to the NetSession node free list (size class 7).
static int __fastcall NetGatheringDDL_ClearNodeList(int** self, void* /*edx*/)
{
    int* head = self[1];
    int* node = *reinterpret_cast<int**>(head);

    *reinterpret_cast<int**>(head) = head;   // head->next = head
    CSF_AT(int*, head, 4) = head;             // head->prev = head
    self[2] = nullptr;

    if (node != head) {
        int* nextNode;
        do {
            nextNode = *reinterpret_cast<int**>(node);
            CSF_AT(void*, node, 8) = CSF_VTBL(0x0085FE1C);
            DestroyTemporaryString(reinterpret_cast<uint8_t*>(node) + 16, nullptr);
            auto allocator = NetSession_GetNodeAllocator();
            NetSession_FreeListNode(static_cast<int>(reinterpret_cast<intptr_t>(allocator)),
                                    static_cast<int>(reinterpret_cast<intptr_t>(node)),
                                    7);
            head = self[1];
            node = nextNode;
        } while (nextNode != head);
    }

    return static_cast<int>(reinterpret_cast<intptr_t>(head));
}

HOOK(0x6A6FB0, NetGatheringDDL_ClearNodeList, "auto");
