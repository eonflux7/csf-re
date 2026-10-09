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

CSF_FUNC(int, __fastcall, sub_443000, 0x00443000, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(void*, __fastcall, GetGlobalSubObject_4FC0A0, 0x004FC0A0, (void* self, void* edx));

// Reads the 32-bit value stored at (base + offset).
static int ReadDwordAt(int base, int offset)
{
    return *reinterpret_cast<int*>(static_cast<std::uintptr_t>(base) + static_cast<std::uintptr_t>(offset));
}

// Stack state block passed to sub_443000:
//   block[0] = 1 (flag), block[1] = first pair member's inner value,
//   block[2] = 0,        block[3] = second pair member's inner value.
static int __stdcall ApplyPairStateToGlobalSubObject(int /*a1*/, int /*a2*/, int* pairPointers)
{
    int block[4];
    block[1] = 0;
    block[0] = 1;
    block[2] = 0;

    int firstObject = pairPointers[0];
    block[1] = ReadDwordAt(ReadDwordAt(firstObject, 4), 4);

    int secondObject = pairPointers[1];
    block[3] = ReadDwordAt(ReadDwordAt(secondObject, 4), 4);

    void* globalValue = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
    void* globalSubObject = GetGlobalSubObject_4FC0A0(globalValue, nullptr);

    int stateArg = static_cast<int>(reinterpret_cast<std::uintptr_t>(&block[0]));
    return sub_443000(globalSubObject, nullptr, stateArg);
}

HOOK(0x0045B7B0, ApplyPairStateToGlobalSubObject, "auto");
