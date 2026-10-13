#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, GfxWorld_DecrementTreeIterator, 0x00660630,
         (void* self, void* edx));
CSF_FUNC(int*, __fastcall, GfxWorld_IncrementTreeIterator, 0x00665E40,
         (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GfxWorld_InsertTreeNode, 0x006B7AC0,
         (uint32_t* a1, int a2, uint32_t* a3, char a4, uint32_t* a5, uint32_t* a6));
CSF_FUNC(int, __fastcall, GfxWorld_InsertUniqueTreeValue, 0x006B7C60,
         (void* self, void* edx, int a2, uint32_t* a3));

static uint32_t* __fastcall GfxWorld_InsertWithHint(
    uint32_t* self, void* /*edx*/, uint32_t* result,
    uint32_t* hint, uint32_t* value)
{
    if (CSF_AT(uint32_t, self, 0x8) == 0)
    {
        GfxWorld_InsertTreeNode(
            self, 0, result, 1, CSF_AT(uint32_t*, self, 0x4), value);
        return result;
    }

    uint32_t* header = CSF_AT(uint32_t*, self, 0x4);
    if (hint == CSF_AT(uint32_t*, header, 0x0))
    {
        if (*value < CSF_AT(uint32_t, hint, 0xC))
        {
            GfxWorld_InsertTreeNode(self, 0, result, 1, hint, value);
            return result;
        }
    }
    else if (hint == header)
    {
        uint32_t* last = CSF_AT(uint32_t*, header, 0x8);
        if (CSF_AT(uint32_t, last, 0xC) < *value)
        {
            GfxWorld_InsertTreeNode(self, 0, result, 0, last, value);
            return result;
        }
    }
    else
    {
        uint32_t* neighbor;
        if (*value < CSF_AT(uint32_t, hint, 0xC))
        {
            neighbor = hint;
            GfxWorld_DecrementTreeIterator(&neighbor, nullptr);
            if (CSF_AT(uint32_t, neighbor, 0xC) < *value)
            {
                if (CSF_AT(uint8_t, CSF_AT(uint32_t*, neighbor, 0x8), 0x15))
                    GfxWorld_InsertTreeNode(self, 0, result, 0, neighbor, value);
                else
                    GfxWorld_InsertTreeNode(self, 0, result, 1, hint, value);
                return result;
            }
        }

        if (CSF_AT(uint32_t, hint, 0xC) < *value)
        {
            neighbor = hint;
            GfxWorld_IncrementTreeIterator(&neighbor, nullptr);
            if (neighbor == CSF_AT(uint32_t*, self, 0x4) ||
                *value < CSF_AT(uint32_t, neighbor, 0xC))
            {
                if (CSF_AT(uint8_t, CSF_AT(uint32_t*, hint, 0x8), 0x15))
                    GfxWorld_InsertTreeNode(self, 0, result, 0, hint, value);
                else
                    GfxWorld_InsertTreeNode(self, 0, result, 1, neighbor, value);
                return result;
            }
        }
    }

    alignas(8) uint8_t insertionResultBytes[0x8];
    int insertionResult = GfxWorld_InsertUniqueTreeValue(
        self, nullptr, reinterpret_cast<int>(insertionResultBytes), value);
    *result = CSF_AT(uint32_t, reinterpret_cast<void*>(insertionResult), 0x0);
    return result;
}

HOOK(0x006B7D20, GfxWorld_InsertWithHint, "auto");
