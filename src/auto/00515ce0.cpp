#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, MenuAccount_GateCheck_696B60, 0x00696B60, (void* self, void* edx, void* a2));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static int* __fastcall ProfileSavegames_GetScenarioCollection(void* self, void* /*edx*/)
{
    int* collection = reinterpret_cast<int*>(static_cast<uint8_t*>(self) + 4);
    uint32_t begin = CSF_AT(uint32_t, self, 0x08);
    int32_t count = 0;
    if (begin != 0)
    {
        count = static_cast<int32_t>(
            CSF_AT(uint32_t, self, 0x0C) - begin) / 80;
    }

    if (count == 0)
        return collection;

    uint32_t offset = 0;
    uint32_t remaining = static_cast<uint32_t>(count);
    do
    {
        void* entry = reinterpret_cast<void*>(
            CSF_AT(uint32_t, self, 0x08) + offset);

        if (static_cast<uint8_t>(
                MenuAccount_GateCheck_696B60(entry, nullptr, &g_byte_82E65C)) != 0)
        {
            entry = reinterpret_cast<void*>(
                CSF_AT(uint32_t, self, 0x08) + offset);
            TextInputSubobject_Init(
                entry, nullptr,
                reinterpret_cast<uint8_t*>(
                    const_cast<char*>(CSF_STR(0x00840AEC, "Old Default"))));
        }

        offset += 80;
        --remaining;
    } while (remaining != 0);

    return collection;
}

HOOK(0x00515CE0, ProfileSavegames_GetScenarioCollection, "auto");
