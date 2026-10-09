#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_4503E0, 0x004503E0, (void* self, void* edx, void* a2, float a3, int a4));
CSF_FUNC(uint32_t*, __fastcall, sub_450A80, 0x00450A80, (void* self, void* edx, uint32_t* a2));

static char __fastcall S_004503e0_ScanEntriesAndQueue(S_004503e0* self, void* /*edx*/)
{
    uint32_t activeIndex = self->field_0070;
    if (activeIndex == 0xFFFFFFFFu || activeIndex >= self->field_0068)
        return (char)activeIndex;

    uint32_t entry = reinterpret_cast<uint32_t*>(self->field_0060)[activeIndex];
    if (entry == 0)
        return (char)entry;

    if (self->field_0084 != 0)
        return (char)self->field_0084;

    uint32_t count = self->field_0018;
    self->field_0084 = 1;
    self->field_007c = 0;

    int a4[5];
    memset(a4, 0, 3 * sizeof(int));

    uint32_t i = 0;
    while (i < count)
    {
        C_008342e8* entryObject = reinterpret_cast<C_008342e8*>(
            reinterpret_cast<uint32_t*>(self->field_0010)[i]);
        if (entryObject && sub_4503E0(self, nullptr, entryObject, 1000000.0f, (int)(intptr_t)a4))
        {
            a4[0] = (int)(intptr_t)entryObject;
            sub_450A80(reinterpret_cast<uint8_t*>(self) + 0x74, nullptr,
                       reinterpret_cast<uint32_t*>(a4));
        }
        ++i;
    }

    return (char)count;
}

HOOK(0x00451100, S_004503e0_ScanEntriesAndQueue, "auto");
