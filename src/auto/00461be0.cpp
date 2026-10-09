#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// __thiscall, 3 stack args (a2..a4 unused), callee pops 12 bytes.
static int __fastcall ResetObjectStateAndReleaseChild(void* self, void* /*edx*/,
                                                      int /*unusedArg2*/, int /*unusedArg3*/, int /*unusedArg4*/)
{
    CSF_AT(int, self, 0x60) = -1;
    CSF_AT(int, self, 0x64) = -1;

    int childObjectHandle = CSF_AT(int, self, 0x7C);
    CSF_AT(int, self, 0x18) = 1;
    CSF_AT(uint8_t, self, 0x82) = 1;
    CSF_AT(int, self, 0x6C) = -1;
    CSF_AT(int, self, 0x70) = 0;
    CSF_AT(int, self, 0x74) = -1;
    CSF_AT(int, self, 0x78) = 0;

    if (childObjectHandle != 0) {
        void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(childObjectHandle));
        // Virtual slot 0x64 -> index 25: IsFinished-style query; if it returns false, call slot 0x60 (index 24).
        uint8_t queryResult = ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[25])(child, nullptr);
        if (!queryResult) {
            void* childAgain = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(int, self, 0x7C)));
            ((void(__fastcall*)(void*, void*))CSF_AT(void**, childAgain, 0)[24])(childAgain, nullptr);
        }
    }

    int resolvedResult = CSF_AT(int, self, 0xC);
    CSF_AT(int, self, 0x7C) = 0;

    if (resolvedResult >= 0) {
        int handleAddress = static_cast<int>(reinterpret_cast<uintptr_t>(self) + 4);
        void* resolvedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, handleAddress)));
        if (resolvedObject) {
            resolvedResult = CSF_AT(int, resolvedObject, 0x34);
            if (resolvedResult) {
                CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(resolvedResult)), 0x18) |= 1u;
            }
        }
    }
    return resolvedResult;
}

HOOK(0x00461BE0, ResetObjectStateAndReleaseChild, "auto");
