#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_0069a8a0_InsertTreeNode, 0x0069A990, (uint32_t* a1, int a2, uint32_t* a3, char a4, uint32_t* a5, int a6));
CSF_FUNC(int, __fastcall, MoveTreeIteratorToPreviousNode_660630, 0x00660630, (void* self, void* edx));

static int __fastcall S_0069a8a0_InsertUniqueKey(
    S_0069a8a0* self, void* /*edx*/, int resultAddress, uint32_t* value)
{
    auto* parent = reinterpret_cast<uint32_t*>(self->field_0004);
    auto* node = CSF_AT(uint32_t*, parent, 0x04);
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
        auto* header = reinterpret_cast<uint32_t*>(self->field_0004);
        if (parent == CSF_AT(uint32_t*, header, 0x00))
        {
            const uint32_t insertedNode = *S_0069a8a0_InsertTreeNode(
                reinterpret_cast<uint32_t*>(self), 0,
                &iterator, 1, parent,
                static_cast<int>(reinterpret_cast<uintptr_t>(value)));
            CSF_AT(uint8_t, result, 0x04) = 1;
            CSF_AT(uint32_t, result, 0x00) = insertedNode;
            return resultAddress;
        }

        MoveTreeIteratorToPreviousNode_660630(&iterator, nullptr);
        candidate = reinterpret_cast<uint32_t*>(iterator);
    }

    if (CSF_AT(uint32_t, candidate, 0x0C) >= *value)
    {
        CSF_AT(uint8_t, result, 0x04) = 0;
        CSF_AT(uint32_t, result, 0x00) =
            reinterpret_cast<uint32_t>(candidate);
        return resultAddress;
    }

    const uint32_t insertedNode = *S_0069a8a0_InsertTreeNode(
        reinterpret_cast<uint32_t*>(self), 0,
        &iterator, static_cast<char>(insertLeft), parent,
        static_cast<int>(reinterpret_cast<uintptr_t>(value)));
    CSF_AT(uint32_t, result, 0x00) = insertedNode;
    CSF_AT(uint8_t, result, 0x04) = 1;
    return resultAddress;
}

HOOK(0x0069AB30, S_0069a8a0_InsertUniqueKey, "auto");
