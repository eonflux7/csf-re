#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(bool, __stdcall, TypeRegistry_IsStringLess, 0x0054ABF0,
         (char* a1, char* a2));
CSF_FUNC(int, __fastcall, TypeRegistry_MoveToPreviousNode, 0x0054B230,
         (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, TypeRegistry_InsertTreeNode, 0x0054B4A0,
         (uint32_t* a1, int a2, uint32_t* a3, char a4,
          uint32_t* a5, uint32_t* a6));

static uint32_t* __fastcall TypeRegistry_InsertUnique(
    uint32_t* self, void* /*edx*/, uint32_t* result, uint32_t* value)
{
    uint32_t* header = CSF_AT(uint32_t*, self, 0x4);
    uint32_t* node = CSF_AT(uint32_t*, header, 0x4);
    uint32_t* parent = header;
    bool insertLeft = true;

    if (!CSF_AT(uint8_t, node, 0x19))
    {
        const char* key = CSF_AT(const char*, value, 0x0);
        do
        {
            parent = node;
            insertLeft =
                std::strcmp(key, CSF_AT(const char*, node, 0xC)) < 0;
            if (insertLeft)
                node = CSF_AT(uint32_t*, node, 0x0);
            else
                node = CSF_AT(uint32_t*, node, 0x8);
        }
        while (!CSF_AT(uint8_t, node, 0x19));
    }

    alignas(8) uint8_t iteratorBytes[0x8];
    CSF_AT(uint32_t*, iteratorBytes, 0x0) = parent;
    uint32_t* predecessor = parent;

    if (insertLeft)
    {
        if (parent ==
            CSF_AT(uint32_t*, CSF_AT(uint32_t*, self, 0x4), 0x0))
        {
            uint32_t* inserted = TypeRegistry_InsertTreeNode(
                self, 0, reinterpret_cast<uint32_t*>(&value),
                1, parent, value);
            CSF_AT(uint32_t, result, 0x0) =
                CSF_AT(uint32_t, inserted, 0x0);
            CSF_AT(uint8_t, result, 0x4) = 1;
            return result;
        }

        TypeRegistry_MoveToPreviousNode(iteratorBytes, nullptr);
        predecessor = CSF_AT(uint32_t*, iteratorBytes, 0x0);
    }

    if (TypeRegistry_IsStringLess(
            CSF_AT(char*, predecessor, 0xC),
            CSF_AT(char*, value, 0x0)))
    {
        uint32_t* inserted = TypeRegistry_InsertTreeNode(
            self, 0, reinterpret_cast<uint32_t*>(&value),
            static_cast<char>(insertLeft), parent, value);
        CSF_AT(uint32_t, result, 0x0) =
            CSF_AT(uint32_t, inserted, 0x0);
        CSF_AT(uint8_t, result, 0x4) = 1;
        return result;
    }

    CSF_AT(uint32_t*, result, 0x0) = predecessor;
    CSF_AT(uint8_t, result, 0x4) = 0;
    return result;
}

HOOK(0x0054B900, TypeRegistry_InsertUnique, "auto");
