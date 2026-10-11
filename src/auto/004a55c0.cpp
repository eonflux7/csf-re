#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SpawnEntries_FindIndex, 0x004A4F10, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, SpawnPoints_GrowEntryBuffer, 0x00551CF0, (void* self, void*, int extraEntries));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static int __fastcall C_00834598_SetEntryValue(C_00834598* self, void* /*edx*/, int key, int value)
{
    int result = SpawnEntries_FindIndex(self, nullptr, key);

    if (value == -1) {
        if (result != -1) {
            int newCount = (int)self->field_0084 - 1;
            self->field_0084 = (uint32_t)newCount;
            if (result != newCount) {
                uint8_t* dst = reinterpret_cast<uint8_t*>(self->field_007c) + 8 * result;
                return (int)(intptr_t)_memcpy(dst, dst + 8, (std::size_t)(8 * (newCount - result)));
            }
        }
    } else if (result == -1) {
        uint8_t* buf = reinterpret_cast<uint8_t*>(self) + 0x7C;
        if (self->field_0084 >= self->field_0080) {
            SpawnPoints_GrowEntryBuffer(buf, nullptr, (int)CSF_AT(uint32_t, buf, 0xC));
        }
        uint32_t count = CSF_AT(uint32_t, buf, 8);
        int* entry = reinterpret_cast<int*>(CSF_AT(uint32_t, buf, 0) + 8 * count);
        result = (int)(count + 1);
        CSF_AT(uint32_t, buf, 8) = (uint32_t)result;
        entry[0] = key;
        entry[1] = value;
    } else {
        CSF_AT(int, reinterpret_cast<void*>(self->field_007c), 8 * result + 4) = value;
    }
    return result;
}

HOOK(0x004A55C0, C_00834598_SetEntryValue, "auto");
