#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x004FE7F0, __thiscall-style with one stack arg (ret 4).
CSF_FUNC(unsigned int, __fastcall, sub_4FE7F0, 0x004FE7F0, (void* self, void* edx, char a2));
// Original: 0x00503710, __thiscall, no stack args.
CSF_FUNC(int, __fastcall, sub_503710, 0x00503710, (void* self, void* edx));
// Original: 0x004FE520, __thiscall, no stack args.
CSF_FUNC(int, __fastcall, sub_4FE520, 0x004FE520, (void* self, void* edx));
// Original: 0x00501AB0, ret 4 (one stack arg).
CSF_FUNC(int, __fastcall, sub_501AB0, 0x00501AB0, (void* a1, int a2, char a3));

// Resets the scene-loader state: clears the scene-list flags and the current scene index.
static int __fastcall CSceneLoaderBase_ResetSceneState(C_0083f578* self, void* /*edx*/)
{
    sub_4FE7F0(self, nullptr, 0);
    sub_503710(self, nullptr);
    sub_4FE520(self, nullptr);

    // Virtual slot 0x3C (index 15) called with argument 0.
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x3C / 4])(self, nullptr, 0);

    int result = sub_501AB0(self, 0, 0);

    CSF_AT(uint8_t, self, 0xB5) = 0;
    CSF_AT(uint8_t, self, 0xE0) = 0;
    CSF_AT(int, self, 0x0C) = 0;
    return result;
}

HOOK(0x503790, CSceneLoaderBase_ResetSceneState, "auto");
