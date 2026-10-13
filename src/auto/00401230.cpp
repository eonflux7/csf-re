#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));

// Removes the entry whose id (at entry+8) matches a2->id (a2+4) from the
// 20-byte entry array at self+8 (count at self+0x10). The removed slot is
// filled by the last entry. Returns 1 if an entry was removed, else 0.
static char __fastcall RemoveGlobalObjectReference(void* self, void* /*edx*/, void* a2)
{
    char result = 0;
    uint32_t index = 0;

    if (CSF_AT(uint32_t, self, 0x10) != 0) {
        uint32_t byteOffset = 0;
        do {
            if (result)
                break;

            uint8_t* entries = CSF_AT(uint8_t*, self, 0x8);
            uint8_t* entry = entries + byteOffset;

            if (CSF_AT(uint32_t, entry, 8) == CSF_AT(uint32_t, a2, 4)) {
                int globalIntValue = GetGlobalInt();
                int renderHandle = RenderHandle_FromObject(reinterpret_cast<void*>(globalIntValue), nullptr);
                void* handleObj = reinterpret_cast<void*>(renderHandle);

                uint32_t entryValue = CSF_AT(uint32_t, CSF_AT(uint8_t*, self, 0x8) + byteOffset, 0);
                using SlotTwelveFn = void(__fastcall*)(void* self, void* edx, uint32_t value);
                SlotTwelveFn slotTwelve = reinterpret_cast<SlotTwelveFn>(CSF_AT(void**, handleObj, 0)[12]);
                slotTwelve(handleObj, nullptr, entryValue);

                if (index < CSF_AT(uint32_t, self, 0x10) - 1) {
                    uint8_t* arr = CSF_AT(uint8_t*, self, 0x8);
                    uint32_t count = CSF_AT(uint32_t, self, 0x10);
                    uint8_t* last = arr + 20 * count - 20;
                    uint8_t* dst = CSF_AT(uint8_t*, self, 0x8) + byteOffset;

                    CSF_AT(uint32_t, dst, 0) = CSF_AT(uint32_t, last, 0);
                    CSF_AT(uint32_t, dst, 8) = CSF_AT(uint32_t, last, 8);
                    CSF_AT(uint32_t, dst, 0xC) = CSF_AT(uint32_t, last, 0xC);
                    CSF_AT(uint32_t, dst, 0x10) = CSF_AT(uint32_t, last, 0x10);
                }

                CSF_AT(uint32_t, self, 0x10) = CSF_AT(uint32_t, self, 0x10) - 1;
                result = 1;
            }

            ++index;
            byteOffset += 20;
        } while (index < CSF_AT(uint32_t, self, 0x10));
    }

    return result;
}

HOOK(0x00401230, RemoveGlobalObjectReference, "auto");
