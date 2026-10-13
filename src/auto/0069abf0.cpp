#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char, __fastcall, DestroyEntryTreeBranch, 0x0069A860,
         (void* self, void* edx, void** Block));
CSF_FUNC(int*, __fastcall, EraseEntryNodeFromRange, 0x006B7670,
         (uint32_t* a1, int a2, int* a3, int a4));

static int** __fastcall EntryTree_EraseRange(
    void* self, void* /*edx*/, int** result, int* first, int* last)
{
    void* current = first;
    void* lastNode = last;

    void* sentinel = CSF_AT(void*, self, 0x04);

    if (first == CSF_AT(void*, sentinel, 0x00) && last == sentinel)
    {
        DestroyEntryTreeBranch(self, nullptr, CSF_AT(void**, sentinel, 0x04));

        sentinel = CSF_AT(void*, self, 0x04);
        CSF_AT(void*, sentinel, 0x04) = sentinel;

        CSF_AT(std::uint32_t, self, 0x08) = 0;

        sentinel = CSF_AT(void*, self, 0x04);
        CSF_AT(void*, sentinel, 0x00) = sentinel;

        sentinel = CSF_AT(void*, self, 0x04);
        CSF_AT(void*, sentinel, 0x08) = sentinel;

        sentinel = CSF_AT(void*, self, 0x04);
        *result = reinterpret_cast<int*>(CSF_AT(void*, sentinel, 0x00));
        return result;
    }

    if (first != last)
    {
        int* rangeCursor = first;

        do
        {
            void* nodeToErase = current;

            if (CSF_AT(std::uint8_t, current, 0x15) == 0)
            {
                void* right = CSF_AT(void*, current, 0x08);
                if (CSF_AT(std::uint8_t, right, 0x15) == 0)
                {
                    current = right;
                    void* left = CSF_AT(void*, current, 0x00);
                    while (CSF_AT(std::uint8_t, left, 0x15) == 0)
                    {
                        current = left;
                        left = CSF_AT(void*, current, 0x00);
                    }
                }
                else
                {
                    void* parent = CSF_AT(void*, current, 0x04);
                    while (CSF_AT(std::uint8_t, parent, 0x15) == 0)
                    {
                        if (current != CSF_AT(void*, parent, 0x08))
                            break;

                        current = parent;
                        parent = CSF_AT(void*, parent, 0x04);
                    }
                    current = parent;
                }
            }

            EraseEntryNodeFromRange(reinterpret_cast<uint32_t*>(self), 0,
                                    reinterpret_cast<int*>(&rangeCursor),
                                    static_cast<int>(reinterpret_cast<std::uintptr_t>(nodeToErase)));
        } while (current != lastNode);
    }

    *result = reinterpret_cast<int*>(current);
    return result;
}

HOOK(0x0069ABF0, EntryTree_EraseRange, "auto");
