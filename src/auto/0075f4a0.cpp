#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, DestroyMapTreeBranch, 0x0075EDC0,
         (void* self, void* edx, void** Block));
CSF_FUNC(int*, __fastcall, EraseMapNodeFromRange, 0x0075F1E0,
         (uint32_t* a1, int a2, int* a3, int a4));

static int** __fastcall MapEraseRangeNodes(
    void* self, void* /*edx*/, int** result, int* first, int* last)
{
    void* current = first;
    void* lastNode = last;

    void* sentinel = CSF_AT(void*, self, 0x04);

    if (first == CSF_AT(void*, sentinel, 0x00) && last == sentinel)
    {
        DestroyMapTreeBranch(self, nullptr, CSF_AT(void**, sentinel, 0x04));

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

            if (CSF_AT(std::uint8_t, current, 0x31) == 0)
            {
                void* right = CSF_AT(void*, current, 0x08);
                if (CSF_AT(std::uint8_t, right, 0x31) == 0)
                {
                    current = right;
                    void* left = CSF_AT(void*, current, 0x00);
                    while (CSF_AT(std::uint8_t, left, 0x31) == 0)
                    {
                        current = left;
                        left = CSF_AT(void*, current, 0x00);
                    }
                }
                else
                {
                    void* parent = CSF_AT(void*, current, 0x04);
                    while (CSF_AT(std::uint8_t, parent, 0x31) == 0)
                    {
                        if (current != CSF_AT(void*, parent, 0x08))
                            break;

                        current = parent;
                        parent = CSF_AT(void*, parent, 0x04);
                    }
                    current = parent;
                }
            }

            EraseMapNodeFromRange(reinterpret_cast<uint32_t*>(self), 0,
                                  reinterpret_cast<int*>(&rangeCursor),
                                  static_cast<int>(reinterpret_cast<std::uintptr_t>(nodeToErase)));
        } while (current != lastNode);
    }

    *result = reinterpret_cast<int*>(current);
    return result;
}

HOOK(0x0075F4A0, MapEraseRangeNodes, "auto");
