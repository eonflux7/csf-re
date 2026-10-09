#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, sub_44BEA0, 0x0044BEA0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, DefaultDestination_Get, 0x004FE4A0, (void* self, void* edx));

static int __stdcall ForwardFloatAndFlagToDefaultDestination(int /*a1*/, int /*a2*/, uint32_t* a3)
{
    // Flag: true when byte at [a3[1]->+4 + 1] is zero.
    uint32_t* flagHolder = reinterpret_cast<uint32_t*>(a3[1]);
    uint8_t* flagBase = reinterpret_cast<uint8_t*>(flagHolder[1]);
    int flagIsZero = (flagBase[1] == 0) ? 1 : 0;

    // Float at [[a3[0]]->+4] + 4, truncated to an integer.
    uint32_t* speedHolder = reinterpret_cast<uint32_t*>(a3[0]);
    uint8_t* speedBase = reinterpret_cast<uint8_t*>(speedHolder[1]);
    float speed = *reinterpret_cast<float*>(speedBase + 4);
    int speedInt = static_cast<int>(static_cast<int64_t>(speed));

    void* globalValue = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_8B0568()));
    void* destination = DefaultDestination_Get(globalValue, nullptr);

    return sub_44BEA0(destination, nullptr, speedInt, flagIsZero);
}

HOOK(0x45B400, ForwardFloatAndFlagToDefaultDestination, "auto");
