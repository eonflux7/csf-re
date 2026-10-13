#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, Tree_InsertNode_6B7AC0, 0x006B7AC0, (uint32_t* a1, int a2, uint32_t* a3, char a4, uint32_t* a5, uint32_t* a6));
CSF_FUNC(int, __fastcall, TreeMap_MoveIteratorToPrevious_660630, 0x00660630, (void* self, void* edx));

static int __fastcall TreeMap_InsertUniqueKey(void* self, void* /*edx*/, int resultAddress, uint32_t* value)
{
    // Header sentinel node lives at self->+4; its root is header[1] (+4).
    auto* header = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x04));
    auto* parent = header;
    auto* node = CSF_AT(uint32_t*, header, 0x04);
    bool insertLeft = true;

    if (!CSF_AT(uint8_t, node, 0x15))
    {
        const uint32_t key = *value;
        do
        {
            insertLeft = key < CSF_AT(uint32_t, node, 0x0C);
            parent = node;
            node = insertLeft
                ? CSF_AT(uint32_t*, node, 0x00)
                : CSF_AT(uint32_t*, node, 0x08);
        }
        while (!CSF_AT(uint8_t, node, 0x15));
    }

    uint32_t iterator = reinterpret_cast<uint32_t>(parent);
    auto* candidate = parent;
    auto* result = reinterpret_cast<void*>(resultAddress);

    if (insertLeft)
    {
        if (parent == CSF_AT(uint32_t*, header, 0x00))
        {
            const uint32_t insertedNode = *Tree_InsertNode_6B7AC0(
                reinterpret_cast<uint32_t*>(self), 0,
                &iterator, 1, parent, value);
            CSF_AT(uint8_t, result, 0x04) = 1;
            CSF_AT(uint32_t, result, 0x00) = insertedNode;
            return resultAddress;
        }

        TreeMap_MoveIteratorToPrevious_660630(&iterator, nullptr);
        candidate = reinterpret_cast<uint32_t*>(iterator);
    }

    if (CSF_AT(uint32_t, candidate, 0x0C) >= *value)
    {
        CSF_AT(uint8_t, result, 0x04) = 0;
        CSF_AT(uint32_t, result, 0x00) = reinterpret_cast<uint32_t>(candidate);
        return resultAddress;
    }

    const uint32_t insertedNode = *Tree_InsertNode_6B7AC0(
        reinterpret_cast<uint32_t*>(self), 0,
        &iterator, static_cast<char>(insertLeft), parent, value);
    CSF_AT(uint32_t, result, 0x00) = insertedNode;
    CSF_AT(uint8_t, result, 0x04) = 1;
    return resultAddress;
}

HOOK(0x006B7C60, TreeMap_InsertUniqueKey, "auto");
