#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Steps a tree iterator back to the previous in-order node. Node layout:
// +0x00 left link, +0x04 right link, +0x08 next link, +0x2D sentinel/end flag.
static int __fastcall MoveBriefingTreeIteratorToPreviousNode(void* self, void* /*edx*/)
{
    void* node = CSF_AT(void*, self, 0x00);

    if (CSF_AT(std::uint8_t, node, 0x2D) != 0)
    {
        node = CSF_AT(void*, node, 0x08);
        CSF_AT(void*, self, 0x00) = node;
    }
    else
    {
        void* predecessor = CSF_AT(void*, node, 0x00);

        if (CSF_AT(std::uint8_t, predecessor, 0x2D) == 0)
        {
            node = CSF_AT(void*, predecessor, 0x08);
            while (CSF_AT(std::uint8_t, node, 0x2D) == 0)
            {
                predecessor = node;
                node = CSF_AT(void*, predecessor, 0x08);
            }
            CSF_AT(void*, self, 0x00) = predecessor;
        }
        else
        {
            node = CSF_AT(void*, node, 0x04);
            if (CSF_AT(std::uint8_t, node, 0x2D) == 0)
            {
                do
                {
                    if (CSF_AT(void*, self, 0x00) != CSF_AT(void*, node, 0x00))
                        break;

                    CSF_AT(void*, self, 0x00) = node;
                    node = CSF_AT(void*, node, 0x04);
                }
                while (CSF_AT(std::uint8_t, node, 0x2D) == 0);

                if (CSF_AT(std::uint8_t, node, 0x2D) == 0)
                    CSF_AT(void*, self, 0x00) = node;
            }
        }
    }

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(node));
}

HOOK(0x0043E130, MoveBriefingTreeIteratorToPreviousNode, "auto");
