#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_4AF450, 0x004AF450, (void* self, void* edx, float* a3, uint32_t* a6));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(char, __fastcall, sub_4DC660, 0x004DC660, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

static char __fastcall TEstatico_CopyTargetPositionIfFree(S_004af450* self, void* /*edx*/, float* a2, uint32_t* a3)
{
    char result = sub_4AF450(self, nullptr, a2, a3);
    if (result)
        return result;

    int linkedValue = GetLinkedObjectValue_4DB5C0(reinterpret_cast<uint8_t*>(self) + 8, nullptr);
    int ownerBase = linkedValue ? linkedValue - 4 : 0;
    void* owner = reinterpret_cast<void*>(ownerBase);
    int v7 = CSF_AT(int, owner, 0x2C);

    int singletonValue = GetValue8B0554();
    void* manager = CSF_AT(void*, reinterpret_cast<void*>(singletonValue), 0x1FD8);

    char blocked = sub_4DC660(static_cast<uint8_t*>(manager) + 0x54, nullptr,
                              v7 + 0x1C,
                              reinterpret_cast<int>(a2 + 3));
    if (blocked)
        return result;

    uint32_t* src = reinterpret_cast<uint32_t*>(a2);
    a3[0] = src[3];
    a3[1] = src[4];
    a3[2] = src[5];
    return 1;
}

HOOK(0x004AF540, TEstatico_CopyTargetPositionIfFree, "auto");
