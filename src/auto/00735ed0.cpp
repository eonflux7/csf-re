#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, Tree_DecrementIterator, 0x00733D20,
         (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, Tree_InsertNode, 0x00735760,
         (uint32_t* a1, int a2, uint32_t* a3, char a4,
          uint32_t* a5, uint32_t* a6));

static uint32_t* __fastcall Tree_InsertUniqueInt(
    uint32_t* self, void* /*edx*/, uint32_t* result, uint32_t* value)
{
    uint32_t* parent = CSF_AT(uint32_t*, self, 0x4);
    uint32_t* node = CSF_AT(uint32_t*, parent, 0x4);
    bool insertLeft = true;

    if (!CSF_AT(uint8_t, node, 0x11))
    {
        const int32_t key = CSF_AT(int32_t, value, 0x0);
        do
        {
            insertLeft = key < CSF_AT(int32_t, node, 0xC);
            parent = node;
            node = insertLeft
                ? CSF_AT(uint32_t*, node, 0x0)
                : CSF_AT(uint32_t*, node, 0x8);
        } while (!CSF_AT(uint8_t, node, 0x11));
    }

    uint32_t iteratorNode = reinterpret_cast<uint32_t>(parent);
    uint32_t* candidate = parent;

    if (insertLeft)
    {
        uint32_t* header = CSF_AT(uint32_t*, self, 0x4);
        if (parent == CSF_AT(uint32_t*, header, 0x0))
        {
            uint32_t* inserted = Tree_InsertNode(
                self, 0, &iteratorNode, 1, parent, value);
            const uint32_t insertedNode = *inserted;
            CSF_AT(uint8_t, result, 0x4) = 1;
            CSF_AT(uint32_t, result, 0x0) = insertedNode;
            return result;
        }

        Tree_DecrementIterator(&iteratorNode, nullptr);
        candidate = reinterpret_cast<uint32_t*>(iteratorNode);
    }

    if (CSF_AT(int32_t, candidate, 0xC) >= CSF_AT(int32_t, value, 0x0))
    {
        CSF_AT(uint8_t, result, 0x4) = 0;
        CSF_AT(uint32_t, result, 0x0) = reinterpret_cast<uint32_t>(candidate);
        return result;
    }

    uint32_t* inserted = Tree_InsertNode(
        self, 0, &iteratorNode, static_cast<char>(insertLeft), parent, value);
    const uint32_t insertedNode = *inserted;
    CSF_AT(uint32_t, result, 0x0) = insertedNode;
    CSF_AT(uint8_t, result, 0x4) = 1;
    return result;
}

HOOK(0x00735ED0, Tree_InsertUniqueInt, "auto");
