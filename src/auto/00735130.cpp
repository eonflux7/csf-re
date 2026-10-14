#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, AssignString, 0x00403C60, (void*, void*, const char*, uint32_t));
CSF_FUNC(void*, __fastcall, AssignStringRange, 0x00403B60, (void*, void*, void*, uint32_t, uint32_t));
CSF_FUNC(void*, __fastcall, ExceptionCtor, 0x007AF6A0, (void*, void*));
CSF_FUNC(void*, __stdcall, ThrowException, 0x007AF666, (void*, void*));
CSF_FUNC(void**, __fastcall, Advance, 0x00733C10, (void**, void*));
CSF_FUNC(void*, __cdecl, Minimum, 0x0050E590, (void*));
CSF_FUNC(void*, __cdecl, Maximum, 0x00732CE0, (void*));
CSF_FUNC(void*, __fastcall, RotateLeft, 0x0050E530, (void*, void*, void*));
CSF_FUNC(void*, __fastcall, RotateRight, 0x0050E5B0, (void*, void*, void*));
CSF_FUNC(char, __cdecl, FreeNode, 0x00690E60, (void*));

static void*& left(void* n) { return CSF_AT(void*, n, 0); }
static void*& parent(void* n) { return CSF_AT(void*, n, 4); }
static void*& right(void* n) { return CSF_AT(void*, n, 8); }
// Flags follow the value in this instantiation, not the generic tree header.
static uint8_t& color(void* n) { return CSF_AT(uint8_t, n, 0x10); }
static uint8_t& nil(void* n) { return CSF_AT(uint8_t, n, 0x11); }

static void** __fastcall EraseTreeNodeFromRange(void* self, void* /*edx*/, void** result, void* node)
{
    void* erased = node;
    if (nil(node))
    {
        alignas(4) uint8_t text[28];
        CSF_AT(uint32_t, text, 24) = 15;
        CSF_AT(uint32_t, text, 20) = 0;
        text[4] = 0;
        AssignString(text, nullptr, CSF_STR(0x00830FC4, "invalid map/set<T> iterator"), 27);
        alignas(4) uint8_t exception[40];
        ExceptionCtor(exception, nullptr);
        CSF_AT(void**, exception, 0) = CSF_VTBL(0x008769D4);
        CSF_AT(uint32_t, exception, 36) = 15;
        CSF_AT(uint32_t, exception, 32) = 0;
        exception[16] = 0;
        AssignStringRange(exception + 12, nullptr, text, 0, UINT32_MAX);
        CSF_AT(void**, exception, 0) = CSF_VTBL(0x008769EC);
        // The throw cannot return normally. If it does, the binary's next
        // instruction takes EAX as the erased node, not the iterator argument.
        erased = ThrowException(exception, reinterpret_cast<void*>(CSF_ADDR_DATA(0x00887184)));
    }
    Advance(&node, nullptr);
    void* child;
    void* childParent;
    bool splice = false;
    if (nil(left(erased)))
        child = right(erased);
    else if (nil(right(erased)))
        child = left(erased);
    else
    {
        child = right(node);
        splice = node != erased;
    }
    if (!splice)
    {
        childParent = parent(erased);
        if (!nil(child))
            parent(child) = childParent;
        void* head = CSF_AT(void*, self, 4);
        if (parent(head) == erased)
            parent(head) = child;
        else if (left(childParent) == erased)
            left(childParent) = child;
        else
            right(childParent) = child;
        head = CSF_AT(void*, self, 4);
        if (left(head) == erased)
        {
            void* newLeft = nil(child) ? childParent : Minimum(child);
            left(CSF_AT(void*, self, 4)) = newLeft;
        }
        head = CSF_AT(void*, self, 4);
        if (right(head) == erased)
            right(head) = nil(child) ? childParent : Maximum(child);
    }
    else
    {
        parent(left(erased)) = node;
        left(node) = left(erased);
        if (node == right(erased))
            childParent = node;
        else
        {
            childParent = parent(node);
            if (!nil(child))
                parent(child) = childParent;
            left(childParent) = child;
            right(node) = right(erased);
            parent(right(erased)) = node;
        }
        void* head = CSF_AT(void*, self, 4);
        if (parent(head) == erased)
            parent(head) = node;
        else
        {
            void* p = parent(erased);
            if (left(p) == erased)
                left(p) = node;
            else
                right(p) = node;
        }
        parent(node) = parent(erased);
        uint8_t oldColor = color(node);
        color(node) = color(erased);
        color(erased) = oldColor;
    }
    if (color(erased) == 1)
    {
        if (child != parent(CSF_AT(void*, self, 4)))
        {
            do
            {
                if (color(child) != 1)
                    break;
                void* sibling = left(childParent);
                if (child == left(childParent))
                {
                    sibling = right(childParent);
                    if (!color(sibling))
                    {
                        color(sibling) = 1;
                        color(childParent) = 0;
                        RotateLeft(self, nullptr, childParent);
                        sibling = right(childParent);
                    }
                    if (!nil(sibling))
                    {
                        if (color(left(sibling)) != 1 || color(right(sibling)) != 1)
                        {
                            if (color(right(sibling)) == 1)
                            {
                                color(left(sibling)) = 1;
                                color(sibling) = 0;
                                RotateRight(self, nullptr, sibling);
                                sibling = right(childParent);
                            }
                            color(sibling) = color(childParent);
                            color(childParent) = 1;
                            color(right(sibling)) = 1;
                            RotateLeft(self, nullptr, childParent);
                            break;
                        }
                        color(sibling) = 0;
                    }
                }
                else
                {
                    if (!color(sibling))
                    {
                        color(sibling) = 1;
                        color(childParent) = 0;
                        RotateRight(self, nullptr, childParent);
                        sibling = left(childParent);
                    }
                    if (!nil(sibling))
                    {
                        if (color(right(sibling)) != 1 || color(left(sibling)) != 1)
                        {
                            if (color(left(sibling)) == 1)
                            {
                                color(right(sibling)) = 1;
                                color(sibling) = 0;
                                RotateLeft(self, nullptr, sibling);
                                sibling = left(childParent);
                            }
                            color(sibling) = color(childParent);
                            color(childParent) = 1;
                            color(left(sibling)) = 1;
                            RotateRight(self, nullptr, childParent);
                            break;
                        }
                        color(sibling) = 0;
                    }
                }
                child = childParent;
                bool atRoot = childParent == parent(CSF_AT(void*, self, 4));
                childParent = parent(childParent);
                if (atRoot)
                    break;
            } while (true);
        }
        color(child) = 1;
    }
    FreeNode(erased);
    uint32_t size = CSF_AT(uint32_t, self, 8);
    if (size)
        CSF_AT(uint32_t, self, 8) = size - 1;
    *result = node;
    return result;
}

HOOK(0x00735130, EraseTreeNodeFromRange, "auto");
