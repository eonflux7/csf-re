#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static const auto InsertTreeNode =
    reinterpret_cast<std::uint32_t* (__fastcall*)(
        void*, void*, void**, char, void*, const void*)>(0x0073A360);

static const auto MoveTreeIteratorToPreviousNode =
    reinterpret_cast<int (__fastcall*)(void*, void*)>(0x00739B20);

static int __fastcall InsertUniqueUnsignedShortTreeValue(
    void* self, void* /*edx*/, int resultAddress, std::uint16_t* value)
{
    void* parent = CSF_AT(void*, self, 0x04);
    void* node = CSF_AT(void*, parent, 0x04);
    char insertLeft = 1;

    if (CSF_AT(std::uint8_t, node, 0x49) == 0)
    {
        const std::uint16_t key = *value;
        do
        {
            parent = node;
            insertLeft = key < CSF_AT(std::uint16_t, node, 0x0C);
            if (insertLeft)
                node = CSF_AT(void*, node, 0x00);
            else
                node = CSF_AT(void*, node, 0x08);
        }
        while (CSF_AT(std::uint8_t, node, 0x49) == 0);
    }

    void* iterator = parent;
    void* candidate = parent;
    void* result = reinterpret_cast<void*>(resultAddress);

    if (insertLeft)
    {
        void* head = CSF_AT(void*, self, 0x04);
        if (parent == CSF_AT(void*, head, 0x00))
        {
            const std::uint32_t inserted =
                *InsertTreeNode(self, parent, &iterator, 1, parent, value);
            CSF_AT(std::uint8_t, result, 0x04) = 1;
            CSF_AT(std::uint32_t, result, 0x00) = inserted;
            return resultAddress;
        }

        MoveTreeIteratorToPreviousNode(&iterator, nullptr);
        candidate = iterator;
    }

    if (CSF_AT(std::uint16_t, candidate, 0x0C) >= *value)
    {
        CSF_AT(std::uint8_t, result, 0x04) = 0;
        CSF_AT(void*, result, 0x00) = candidate;
        return resultAddress;
    }

    const std::uint32_t inserted =
        *InsertTreeNode(self, &iterator, &iterator, insertLeft, parent, value);
    CSF_AT(std::uint32_t, result, 0x00) = inserted;
    CSF_AT(std::uint8_t, result, 0x04) = 1;
    return resultAddress;
}

HOOK(0x0073A5C0, InsertUniqueUnsignedShortTreeValue, "auto");
