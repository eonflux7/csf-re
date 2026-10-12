#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Node lookup used by the campaign entry table (original 0x43E970).
CSF_FUNC(int*, __fastcall, CampaignEntry_FindNode, 0x0043E970, (void* self, void* edx, int a2));

// String comparison of a key against a node's text (original 0x42E200).
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));

// Finds the node matching the key. Returns the node when it is not the sentinel head and
// is not ordered before the key; otherwise returns the sentinel head stored at this->field_0004.
// Writes the chosen pointer through `result` and returns `result` (ret 8).
static int** __fastcall CampaignEntry_FindMatchOrHead(S_0043e970* self, void* /*edx*/, int** result, unsigned int* key)
{
    // The original's call to 0x43E970 is a call through the struct's own vtable reference;
    // this is a non-executing reference to keep the address bound.
    (void)CSF_VTBL(0x0043E970);

    int* node = CampaignEntry_FindNode(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(key)));

    bool keepNode = false;
    if (node != reinterpret_cast<int*>(self->field_0004)) {
        // Small-string buffer lives inline at +0x10; otherwise +0x10 holds a pointer to the text.
        unsigned int capacity = CSF_AT(unsigned int, node, 0x24);
        char* nodeText = (capacity < 0x10)
            ? reinterpret_cast<char*>(node) + 0x10
            : CSF_AT(char*, node, 0x10);
        unsigned int nodeLength = CSF_AT(unsigned int, node, 0x20);

        int comparison = CompareStringSubstring(key, nullptr, 0u, key[5], nodeText, nodeLength);
        keepNode = !(comparison < 0);
    }

    if (keepNode) {
        *result = node;
    } else {
        *result = reinterpret_cast<int*>(self->field_0004);
    }
    return result;
}

HOOK(0x0043F2A0, CampaignEntry_FindMatchOrHead, "auto");
