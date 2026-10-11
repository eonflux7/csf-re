#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __fastcall, sub_6C5B30, 0x006C5B30, (void* self, void* edx, int a2));

// Appends one {object, float, word} record to the growable array at list.
// list[0] = data pointer, list[1] = capacity, list[2] = count, list[3] = grow argument.
static int __stdcall GfxSkin_PushObjectRecord(void* obj, int32_t* list)
{
    // obj->vtbl[8]() -> float
    float recordFloat = ((float(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[8])(obj, nullptr);

    // obj->vtbl[10]() -> 16-bit value
    uint16_t recordWord = ((uint16_t(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[10])(obj, nullptr);

    if (static_cast<uint32_t>(list[2]) >= static_cast<uint32_t>(list[1]))
        sub_6C5B30(list, nullptr, list[3]);

    int32_t index = list[2];
    uint8_t* data = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(list[0])));
    uint8_t* entry = data + static_cast<size_t>(index) * 12;
    list[2] = index + 1;

    CSF_AT(void*, entry, 0) = obj;
    CSF_AT(float, entry, 4) = recordFloat;
    CSF_AT(int32_t, entry, 8) = static_cast<int32_t>(recordWord);

    return static_cast<int>(recordWord);
}

HOOK(0x6C6370, GfxSkin_PushObjectRecord, "auto");
