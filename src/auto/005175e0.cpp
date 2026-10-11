#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Binding by address for the original's callees.
CSF_FUNC(int, __fastcall, sub_5167D0, 0x005167D0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, sub_5154B0, 0x005154B0, (int* a1));
CSF_FUNC(uint32_t*, __fastcall, sub_517230, 0x00517230, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Reimplementation of sub_5175E0 (__thiscall, ret 0).
// Counts the 80-byte entries between field_0008 and field_000c. If any exist,
// fetches a key via the vtable slot +0x24, builds a temporary string from it,
// reloads the embedded sub-object at +4 from that string, then frees the
// temporary's heap buffers.
static int __fastcall C_00840af8_ReloadEntryFromKey(C_00840af8* self, void* /*edx*/)
{
    // Stack frame of the original: [ebp-0x54] .. [ebp-0x00], 0x54 bytes.
    // Offsets from the frame base: a3 +0x00, v5 +0x04, v6 +0x18, Block +0x38,
    // v8 +0x3C, v9 +0x40, v10 +0x50.
    alignas(8) uint8_t frame[0x54];

    int result = static_cast<int>(self->field_0008);
    uint8_t* pad = reinterpret_cast<uint8_t*>(self) + 4;

    if (result != 0)
    {
        int byteSpan = static_cast<int>(CSF_AT(uint32_t, self, 0x0C) - self->field_0008);
        int entryCount = byteSpan / 80;
        result = entryCount;
        CSF_AT(int, frame, 0x50) = entryCount;

        if (entryCount != 0)
        {
            void* key = reinterpret_cast<void*>(static_cast<intptr_t>(self->vSlot24()));
            sub_5167D0(frame, nullptr, key);

            sub_5154B0(reinterpret_cast<int*>(pad));

            sub_517230(pad, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(frame)));

            void* block = CSF_AT(void*, frame, 0x38);
            result = static_cast<int>(reinterpret_cast<intptr_t>(block));
            if (block)
                result = Mem_FreeDebug(block);

            CSF_AT(void*, frame, 0x38) = nullptr;
            CSF_AT(int, frame, 0x3C) = 0;
            CSF_AT(int, frame, 0x40) = 0;

            if (CSF_AT(uint32_t, frame, 0x18) >= 8u)
            {
                void* v5 = CSF_AT(void*, frame, 0x04);
                result = static_cast<int>(reinterpret_cast<intptr_t>(v5));
                if (v5)
                    return Mem_FreeDebug(v5);
            }
        }
    }
    return result;
}

HOOK(0x005175E0, C_00840af8_ReloadEntryFromKey, "auto");
