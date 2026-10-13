#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, ConstructWideString, 0x00696F50, (void* self, void* edx, wchar_t* String));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(uint32_t*, __fastcall, SpawnPoints_ApplyAndReleaseBlock_554130, 0x00554130, (void* self, void* , char a2, void* Block, int a4, int a5, int a6, int a7, unsigned int a8));
CSF_FUNC(uint32_t*, __fastcall, SpawnPoints_ApplyAndReleaseBlock_554190, 0x00554190, (void* self, void* , char a2, void* Block, int a4, int a5, int a6, int a7, unsigned int a8));
CSF_FUNC(uint32_t*, __fastcall, SpawnPoints_UpdateSideAndFreeBlock, 0x005541F0, (void* self, void* , char a2, char a3, void* Block, int a5, int a6, int a7, int a8, unsigned int a9));

// Clears the console input line and its three state fields after dispatching the
// pending input (0x100 mode: 0 = apply and release, 1 = update side and free, 2 = apply and release).
static void __fastcall C_0083f1f8_SubmitInput(C_0083f1f8* self, void* /*edx*/)
{
    wchar_t* inputText = reinterpret_cast<wchar_t*>(&self->field_0012);

    if (self->field_0012 != 0)
    {
        uint32_t mode = self->field_0100;
        if (mode == 0)
        {
            alignas(4) uint8_t argBlock[0x1c];
            ConstructWideString(argBlock, nullptr, inputText);
            void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobal8B0688()));
            SpawnPoints_ApplyAndReleaseBlock_554190(
                target, nullptr,
                static_cast<char>(CSF_AT(uint32_t, argBlock, 0x00)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, argBlock, 0x04))),
                static_cast<int>(CSF_AT(uint32_t, argBlock, 0x08)),
                static_cast<int>(CSF_AT(uint32_t, argBlock, 0x0C)),
                static_cast<int>(CSF_AT(uint32_t, argBlock, 0x10)),
                static_cast<int>(CSF_AT(uint32_t, argBlock, 0x14)),
                CSF_AT(uint32_t, argBlock, 0x18));
        }
        else if (mode == 1)
        {
            alignas(4) uint8_t argBlock[0x1c];
            ConstructWideString(argBlock, nullptr, inputText);
            uint32_t sideValue = self->field_0104;
            void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobal8B0688()));
            SpawnPoints_UpdateSideAndFreeBlock(
                target, nullptr,
                static_cast<char>(sideValue),
                static_cast<char>(CSF_AT(uint32_t, argBlock, 0x00)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, argBlock, 0x04))),
                static_cast<int>(CSF_AT(uint32_t, argBlock, 0x08)),
                static_cast<int>(CSF_AT(uint32_t, argBlock, 0x0C)),
                static_cast<int>(CSF_AT(uint32_t, argBlock, 0x10)),
                static_cast<int>(CSF_AT(uint32_t, argBlock, 0x14)),
                CSF_AT(uint32_t, argBlock, 0x18));
        }
        else if (mode == 2)
        {
            alignas(4) uint8_t argBlock[0x1c];
            ConstructWideString(argBlock, nullptr, inputText);
            void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobal8B0688()));
            SpawnPoints_ApplyAndReleaseBlock_554130(
                target, nullptr,
                static_cast<char>(CSF_AT(uint32_t, argBlock, 0x00)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, argBlock, 0x04))),
                static_cast<int>(CSF_AT(uint32_t, argBlock, 0x08)),
                static_cast<int>(CSF_AT(uint32_t, argBlock, 0x0C)),
                static_cast<int>(CSF_AT(uint32_t, argBlock, 0x10)),
                static_cast<int>(CSF_AT(uint32_t, argBlock, 0x14)),
                CSF_AT(uint32_t, argBlock, 0x18));
        }
    }

    self->field_0012 = 0;
    self->field_00dc = 0;
    self->field_00e8 = 0;
    self->field_00ec = 0;
}

HOOK(0x004F9C20, C_0083f1f8_SubmitInput, "auto");
