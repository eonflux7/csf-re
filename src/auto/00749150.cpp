#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Empties a circular intrusive list: unlinks every node, frees each node's
// attached buffer (when its type tag is >= 8) and then the node itself.
// The list head sentinel is kept and returned.
static void** __fastcall ClearIntrusiveListAndFreeNodes(void*** self, void* /*edx*/)
{
    // head sentinel stored at self+4; self+8 is the node counter, reset to 0
    void** head = CSF_AT(void**, self, 0x4);
    void** node = CSF_AT(void**, head, 0x0);

    // make the sentinel point to itself (empty list)
    CSF_AT(void**, head, 0x0) = head;
    head = CSF_AT(void**, self, 0x4);
    CSF_AT(void**, head, 0x4) = head;
    CSF_AT(void**, self, 0x8) = nullptr;

    if (node != head)
    {
        void** next = nullptr;
        do
        {
            next = CSF_AT(void**, node, 0x0);

            if (CSF_AT(uint32_t, node, 0x28) >= 8)
            {
                void* buffer = CSF_AT(void*, node, 0x14);
                if (buffer)
                    Mem_FreeDebug(buffer);
            }

            CSF_AT(uint32_t, node, 0x28) = 7;
            CSF_AT(uint32_t, node, 0x24) = 0;
            CSF_AT(uint16_t, node, 0x14) = 0;
            Mem_FreeDebug(node);

            head = CSF_AT(void**, self, 0x4);
            node = next;
        } while (next != head);
    }

    return head;
}

HOOK(0x749150, ClearIntrusiveListAndFreeNodes, "auto");
