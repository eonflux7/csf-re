#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, NormalizeRegionCode, 0x00761300, (int a1));

// Returns true when this character's region code is compatible with the
// region reported by the record selected through the 0x1FC0 provider.
static bool __fastcall CPlayerCharacter_IsRegionMatching(C_00833908* self, void* /*edx*/, int a2)
{
    uint8_t* manager = reinterpret_cast<uint8_t*>(GetValue8B0554());
    void* provider = CSF_AT(void*, manager, 0x1FC0);

    // provider->vtbl[2](a2): virtual lookup that returns the record for a2.
    void* record = ((void* (__fastcall*)(void*, void*, int))CSF_AT(void**, provider, 0)[2])(provider, nullptr, a2);

    int recordRegion = NormalizeRegionCode(CSF_AT(int, record, 0xE0));
    if (recordRegion != 0 && recordRegion != static_cast<int>(self->field_0054))
        return false;

    if (CSF_AT(int, record, 0xDC) == 3)
        return static_cast<int>(self->field_0054) == 1;

    return true;
}

HOOK(0x494D50, CPlayerCharacter_IsRegionMatching, "auto");
