#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_0050f230_InsertTreeNode, 0x00519D20, (uint32_t* a1, int a2, uint32_t* a3, char a4, uint32_t* a5, uint32_t* a6));
CSF_FUNC(int, __fastcall, MoveTreeIteratorToPreviousNode_733D20, 0x00733D20, (void* self, void*));

static int __fastcall S_0050f230_InsertUniqueKey(
    S_0050f230* self, void* /*edx*/, int resultAddress, uint32_t* value)
{
    auto* parent = reinterpret_cast<uint32_t*>(self->field_0004);
    auto* node = CSF_AT(uint32_t*, parent, 0x04);
    bool insertLeft = true;

    if (!CSF_AT(uint8_t, node, 0x11))
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
        while (!CSF_AT(uint8_t, node, 0x11));
    }

    uint32_t iterator = reinterpret_cast<uint32_t>(parent);
    auto* candidate = parent;
    auto* result = reinterpret_cast<void*>(resultAddress);

    if (insertLeft)
    {
        auto* header = reinterpret_cast<uint32_t*>(self->field_0004);
        if (parent == CSF_AT(uint32_t*, header, 0x00))
        {
            const uint32_t insertedNode = *S_0050f230_InsertTreeNode(
                reinterpret_cast<uint32_t*>(self), 0,
                &iterator, 1, parent, value);
            CSF_AT(uint8_t, result, 0x04) = 1;
            CSF_AT(uint32_t, result, 0x00) = insertedNode;
            return resultAddress;
        }

        MoveTreeIteratorToPreviousNode_733D20(&iterator, nullptr);
        candidate = reinterpret_cast<uint32_t*>(iterator);
    }

    if (CSF_AT(uint32_t, candidate, 0x0C) >= *value)
    {
        CSF_AT(uint8_t, result, 0x04) = 0;
        CSF_AT(uint32_t, result, 0x00) =
            reinterpret_cast<uint32_t>(candidate);
        return resultAddress;
    }

    const uint32_t insertedNode = *S_0050f230_InsertTreeNode(
        reinterpret_cast<uint32_t*>(self), 0,
        &iterator, static_cast<char>(insertLeft), parent, value);
    CSF_AT(uint32_t, result, 0x00) = insertedNode;
    CSF_AT(uint8_t, result, 0x04) = 1;
    return resultAddress;
}

HOOK(0x00519EC0, S_0050f230_InsertUniqueKey, "auto");
