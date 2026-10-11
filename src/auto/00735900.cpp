#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, DestroyTreeBranch, 0x00733D80,
         (void* self, void* edx, void** Block));
CSF_FUNC(int*, __fastcall, EraseTreeNodeFromRange, 0x00735130,
         (uint32_t* a1, int a2, int* a3, int a4));

static int** __fastcall TreeEraseRange(
    void* self, void* /*edx*/, int** result, int* first, int* last)
{
    void* current = first;
    void* sentinel = CSF_AT(void*, self, 0x04);

    if (first == CSF_AT(void*, sentinel, 0x00) && last == sentinel)
    {
        DestroyTreeBranch(self, nullptr, CSF_AT(void**, sentinel, 0x04));

        sentinel = CSF_AT(void*, self, 0x04);
        CSF_AT(void*, sentinel, 0x04) = sentinel;

        sentinel = CSF_AT(void*, self, 0x04);
        CSF_AT(uint32_t, self, 0x08) = 0;
        CSF_AT(void*, sentinel, 0x00) = sentinel;

        sentinel = CSF_AT(void*, self, 0x04);
        CSF_AT(void*, sentinel, 0x08) = sentinel;

        sentinel = CSF_AT(void*, self, 0x04);
        *result = reinterpret_cast<int*>(CSF_AT(void*, sentinel, 0x00));
        return result;
    }

    if (first != last)
    {
        do
        {
            void* nodeToErase = current;
            if (CSF_AT(uint8_t, current, 0x11) == 0)
            {
                void* right = CSF_AT(void*, current, 0x08);
                if (CSF_AT(uint8_t, right, 0x11) == 0)
                {
                    current = right;
                    void* left = CSF_AT(void*, current, 0x00);
                    while (CSF_AT(uint8_t, left, 0x11) == 0)
                    {
                        current = left;
                        left = CSF_AT(void*, current, 0x00);
                    }
                }
                else
                {
                    void* parent = CSF_AT(void*, current, 0x04);
                    while (CSF_AT(uint8_t, parent, 0x11) == 0)
                    {
                        if (current != CSF_AT(void*, parent, 0x08))
                            break;
                        current = parent;
                        parent = CSF_AT(void*, parent, 0x04);
                    }
                    current = parent;
                }
            }

            EraseTreeNodeFromRange(
                reinterpret_cast<uint32_t*>(self), 0,
                reinterpret_cast<int*>(&first),
                static_cast<int>(reinterpret_cast<std::uintptr_t>(nodeToErase)));
        } while (current != last);
    }

    *result = reinterpret_cast<int*>(current);
    return result;
}

HOOK(0x00735900, TreeEraseRange, "auto");
