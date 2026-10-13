#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwTextureDestroy, 0x006317E0, (int texture));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Walks the circular list of managed texture entries (rooted at field_002c).
// Each node: +0 next, +4 prev, +8 entry. Each entry: +0 key, +4 texture, +0x18 refcount.
// Decrements the refcount of the entry whose key equals `key`; when it reaches
// zero (or goes negative) the node is unlinked, the texture destroyed and the
// entry freed. Only the first matching entry is processed.
static void** __fastcall C_00860db0_ReleaseManagedResource(C_00860db0* self, void* /*edx*/, void** key)
{
    void** result = nullptr;
    uint8_t* head = reinterpret_cast<uint8_t*>(self->field_002c);
    uint8_t* node = CSF_AT(uint8_t*, head, 0);
    bool done = false;

    if (node != head) {
        do {
            if (done)
                break;

            uint8_t* entry = CSF_AT(uint8_t*, node, 8);
            result = key;
            if (CSF_AT(void**, entry, 0) == key) {
                int oldRefs = CSF_AT(int, entry, 0x18);
                int newRefs = oldRefs - 1;
                CSF_AT(int, entry, 0x18) = newRefs;
                result = reinterpret_cast<void**>(static_cast<intptr_t>(newRefs));

                if (newRefs < 0 || oldRefs == 1) {
                    uint8_t* next = CSF_AT(uint8_t*, node, 0);
                    if (node != reinterpret_cast<uint8_t*>(self->field_002c)) {
                        uint8_t* prev = CSF_AT(uint8_t*, node, 4);
                        CSF_AT(uint8_t*, prev, 0) = next;
                        CSF_AT(uint8_t*, next, 4) = prev;
                        Mem_FreeDebug(node);
                        --self->field_0030;
                    }
                    node = next;

                    if (CSF_AT(void**, entry, 0))
                        RwTextureDestroy(CSF_AT(int, entry, 4));
                    CoreMemory_Free(entry);
                }
                done = true;
            } else {
                node = CSF_AT(uint8_t*, node, 0);
            }
        } while (node != reinterpret_cast<uint8_t*>(self->field_002c));
    }

    return result;
}

HOOK(0x006D0970, C_00860db0_ReleaseManagedResource, "auto");
