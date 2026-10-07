#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __stdcall, DestroyTreeBranch, 0x00739B80,
         (void** root));
CSF_FUNC(void**, __fastcall, EraseTreeNode, 0x0073A0A0,
         (void* self, void* edx, void** result, void* node));

static void** __fastcall EraseTreeRange(
    void* self, void* /*edx*/, void** result, void* first, void* last)
{
    void* current = first;
    void* sentinel = CSF_AT(void*, self, 0x04);

    if (current == CSF_AT(void*, sentinel, 0x00) && last == sentinel)
    {
        DestroyTreeBranch(CSF_AT(void**, sentinel, 0x04));

        sentinel = CSF_AT(void*, self, 0x04);
        CSF_AT(void*, sentinel, 0x04) = sentinel;

        sentinel = CSF_AT(void*, self, 0x04);
        CSF_AT(std::uint32_t, self, 0x08) = 0;
        CSF_AT(void*, sentinel, 0x00) = sentinel;

        sentinel = CSF_AT(void*, self, 0x04);
        CSF_AT(void*, sentinel, 0x08) = sentinel;

        sentinel = CSF_AT(void*, self, 0x04);
        *result = CSF_AT(void*, sentinel, 0x00);
        return result;
    }

    while (current != last)
    {
        void* nodeToErase = current;

        if (CSF_AT(std::uint8_t, current, 0x49) == 0)
        {
            void* right = CSF_AT(void*, current, 0x08);
            if (CSF_AT(std::uint8_t, right, 0x49) == 0)
            {
                current = right;
                void* left = CSF_AT(void*, current, 0x00);
                while (CSF_AT(std::uint8_t, left, 0x49) == 0)
                {
                    current = left;
                    left = CSF_AT(void*, current, 0x00);
                }
            }
            else
            {
                void* parent = CSF_AT(void*, current, 0x04);
                while (CSF_AT(std::uint8_t, parent, 0x49) == 0)
                {
                    if (current != CSF_AT(void*, parent, 0x08))
                        break;

                    current = parent;
                    parent = CSF_AT(void*, parent, 0x04);
                }
                current = parent;
            }
        }

        EraseTreeNode(self, nullptr, &first, nodeToErase);
    }

    *result = current;
    return result;
}

HOOK(0x0073A500, EraseTreeRange, "auto");
