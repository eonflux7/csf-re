#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, DestroyTreeBranch, 0x0050EA80,
         (void* self, void* edx, void** root));
CSF_FUNC(int*, __fastcall, EraseTreeNodeFromRange, 0x00735130,
         (uint32_t* a1, int a2, int* a3, int a4));

static int** __fastcall MapEraseRange(
    S_0050f230* self, void* /*edx*/, int** result, int* first, int* last)
{
    void* current = first;
    void* lastNode = last;

    void* sentinel = reinterpret_cast<void*>(self->field_0004);

    if (first == CSF_AT(void*, sentinel, 0x00) && last == sentinel)
    {
        DestroyTreeBranch(self, nullptr, CSF_AT(void**, sentinel, 0x04));

        sentinel = reinterpret_cast<void*>(self->field_0004);
        CSF_AT(void*, sentinel, 0x04) = sentinel;

        self->field_0008 = 0;

        sentinel = reinterpret_cast<void*>(self->field_0004);
        CSF_AT(void*, sentinel, 0x00) = sentinel;

        sentinel = reinterpret_cast<void*>(self->field_0004);
        CSF_AT(void*, sentinel, 0x08) = sentinel;

        sentinel = reinterpret_cast<void*>(self->field_0004);
        *result = reinterpret_cast<int*>(CSF_AT(void*, sentinel, 0x00));
        return result;
    }

    if (first != last)
    {
        int* rangeCursor = first;

        do
        {
            void* nodeToErase = current;

            if (CSF_AT(std::uint8_t, current, 0x11) == 0)
            {
                void* right = CSF_AT(void*, current, 0x08);
                if (CSF_AT(std::uint8_t, right, 0x11) == 0)
                {
                    current = right;
                    void* left = CSF_AT(void*, current, 0x00);
                    while (CSF_AT(std::uint8_t, left, 0x11) == 0)
                    {
                        current = left;
                        left = CSF_AT(void*, current, 0x00);
                    }
                }
                else
                {
                    void* parent = CSF_AT(void*, current, 0x04);
                    while (CSF_AT(std::uint8_t, parent, 0x11) == 0)
                    {
                        if (current != CSF_AT(void*, parent, 0x08))
                            break;

                        current = parent;
                        parent = CSF_AT(void*, parent, 0x04);
                    }
                    current = parent;
                }
            }

            EraseTreeNodeFromRange(reinterpret_cast<uint32_t*>(self), 0,
                                   reinterpret_cast<int*>(&rangeCursor),
                                   static_cast<int>(reinterpret_cast<std::uintptr_t>(nodeToErase)));
        } while (current != lastNode);
    }

    *result = reinterpret_cast<int*>(current);
    return result;
}

HOOK(0x0050F230, MapEraseRange, "auto");
