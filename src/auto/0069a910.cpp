#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Address of a pointer value as an integer, used to track the low byte of eax.
static uint32_t AddrOf(const void* p) {
    return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(p));
}

// Walks the in-order tree of deferred references from the head sentinel. For each node it
// invokes the value object's virtual slot 7 with a2, then advances to the next node whose
// flag at +0x15 is set, using the original's left/parent/right links at +0 / +4 / +8.
// The return value is the low byte of eax as it stood at exit.
static char __fastcall SceneLoader_ProcessDeferredReferences(uint8_t* self, void* /*edx*/, int a2)
{
    uint8_t* head = CSF_AT(uint8_t*, self, 0x04);
    uint32_t eaxReg = AddrOf(head);
    uint8_t* node = CSF_AT(uint8_t*, head, 0x00);

    if (node != head) {
        do {
            uint8_t* valueObj = CSF_AT(uint8_t*, node, 0x0C);
            void** vtbl = CSF_AT(void**, valueObj, 0);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(vtbl[7])(valueObj, nullptr, a2);

            uint8_t flag = CSF_AT(uint8_t, node, 0x15);
            eaxReg = (eaxReg & 0xFFFFFF00u) | flag;

            if (flag == 0) {
                uint8_t* rightChild = CSF_AT(uint8_t*, node, 0x08);
                eaxReg = AddrOf(rightChild);

                if (CSF_AT(uint8_t, rightChild, 0x15) == 0) {
                    // Descend to the leftmost flagged node under the right child.
                    uint8_t* cursor = rightChild;
                    uint8_t* leftPtr = CSF_AT(uint8_t*, cursor, 0x00);
                    eaxReg = AddrOf(leftPtr);
                    while (CSF_AT(uint8_t, leftPtr, 0x15) == 0) {
                        cursor = leftPtr;
                        leftPtr = CSF_AT(uint8_t*, cursor, 0x00);
                        eaxReg = AddrOf(leftPtr);
                    }
                    node = cursor;
                } else {
                    // Climb to the nearest flagged ancestor reached from a left branch.
                    uint8_t* cursor = node;
                    uint8_t* parent = CSF_AT(uint8_t*, node, 0x04);
                    eaxReg = AddrOf(parent);
                    if (CSF_AT(uint8_t, parent, 0x15) == 0) {
                        for (;;) {
                            if (CSF_AT(uint8_t*, parent, 0x08) != cursor) {
                                break;
                            }
                            cursor = parent;
                            parent = CSF_AT(uint8_t*, parent, 0x04);
                            eaxReg = AddrOf(parent);
                            if (CSF_AT(uint8_t, parent, 0x15) != 0) {
                                break;
                            }
                        }
                    }
                    node = parent;
                }
            }
        } while (node != CSF_AT(uint8_t*, self, 0x04));
    }

    return static_cast<char>(static_cast<uint8_t>(eaxReg & 0xFFu));
}

HOOK(0x0069A910, SceneLoader_ProcessDeferredReferences, "auto");
