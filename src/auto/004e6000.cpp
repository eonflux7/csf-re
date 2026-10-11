#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, Sub_4E55C0, 0x004E55C0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static void __fastcall C_0083e9d0_PruneRejectedEntries(C_0083e9d0* self, void* /*edx*/, uint32_t* arg0)
{
    // Stack object: [0]=vtable, [4],[8]=copied fields, [0xC]=saved 'this' (ebp-4 in the original)
    alignas(8) uint8_t entryBytes[0x10];
    int count = static_cast<int>(arg0[2]);
    if (count == 0)
        return;

    int index = count - 1;
    if (index < 0)
        return;

    CSF_AT(C_0083e9d0*, entryBytes, 0xC) = self;

    int byteOffset = 12 * index;
    do {
        uint8_t* element = reinterpret_cast<uint8_t*>(arg0[0]) + byteOffset;
        CSF_AT(void**, entryBytes, 0) = CSF_VTBL(0x0082D448);
        CSF_AT(uint32_t, entryBytes, 4) = CSF_AT(uint32_t, element, 4);
        CSF_AT(uint32_t, entryBytes, 8) = CSF_AT(uint32_t, element, 8);

        C_0083e9d0* current = CSF_AT(C_0083e9d0*, entryBytes, 0xC);
        if (current->field_004d) {
            bool accepted = Sub_4E55C0(current, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(entryBytes)), -1);
            if (!accepted) {
                int newCount = static_cast<int>(arg0[2]) - 1;
                arg0[2] = static_cast<uint32_t>(newCount);
                if (index != newCount) {
                    uint8_t* dst = reinterpret_cast<uint8_t*>(arg0[0]) + byteOffset;
                    _memcpy(dst, dst + 12, 12 * (newCount - index));
                }
            }
        }

        --index;
        byteOffset -= 12;
        CSF_AT(void**, entryBytes, 0) = CSF_VTBL(0x0082D410);
    } while (index >= 0);
}

HOOK(0x004E6000, C_0083e9d0_PruneRejectedEntries, "auto");
