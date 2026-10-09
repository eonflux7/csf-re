#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(char, __fastcall, ProcessEntryValue, 0x00462B10, (void* self, void* edx, uint32_t* a1, int a2, char a3, char a4));
CSF_FUNC(int, __fastcall, GetCachedEntryObject, 0x0075C260, (void* self, void* edx));
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

// Original is __stdcall-shaped (3 stack args, ret 12) but reads ecx on entry and passes it
// as `this` to ProcessEntryValue, so ecx is declared as the first (register) parameter.
static int __fastcall ApplyCachedEntryValueToTarget(void* self, void* edx, int a1, int a2, int a3)
{
    // Session/state object at GetValue8B0554() + 0x1FD8
    C_0083e0d8* v3 = *reinterpret_cast<C_0083e0d8**>(
        reinterpret_cast<uint8_t*>(GetValue8B0554()) + 0x1FD8);

    // a3 -> P1; P1 -> P2 (at +4); value = P2[+4]
    uint32_t* argPtr = reinterpret_cast<uint32_t*>(a3);
    uint32_t* p1 = reinterpret_cast<uint32_t*>(argPtr[0]);
    uint32_t* p2 = reinterpret_cast<uint32_t*>(p1[1]);
    uint32_t entryValue = p2[1];

    uint32_t* v4 = reinterpret_cast<uint32_t*>(GetCachedEntryObject(v3, nullptr));
    ProcessEntryValue(self, nullptr, v4, static_cast<int>(entryValue), 0, 0);

    // Signed load of the object's +8 dword, with the bias added when negative (unsigned view).
    int32_t raw = static_cast<int32_t>(v4[2]);
    float value = static_cast<float>(raw);
    if (raw < 0)
        value += g_UnsignedIntegerBias;

    // a1 -> object; object+4 -> target; write float at target+4
    uint8_t* a1Obj = reinterpret_cast<uint8_t*>(a1);
    uint8_t* target = *reinterpret_cast<uint8_t**>(a1Obj + 4);
    *reinterpret_cast<float*>(target + 4) = value;

    v3->field_0030 = 0;
    return a1;
}

HOOK(0x00462E00, ApplyCachedEntryValueToTarget, "auto");
