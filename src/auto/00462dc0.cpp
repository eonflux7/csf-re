#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __fastcall, sub_462D00, 0x00462D00, (void* self, void* edx, int a1, char a2, char a3, int a4));

static uint32_t ReadDwordAt(uint32_t address) {
    return *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(address));
}

static char __stdcall DispatchThroughChainedTarget(int a1, int a2, int a3) {
    uint32_t chainHead = ReadDwordAt(static_cast<uint32_t>(a3));
    uint32_t chainNext = ReadDwordAt(chainHead + 4);
    uint32_t targetObject = ReadDwordAt(chainNext + 4);
    return sub_462D00(nullptr, nullptr, static_cast<int>(targetObject), 0, 1, a1);
}

HOOK(0x462DC0, DispatchThroughChainedTarget, "auto");
