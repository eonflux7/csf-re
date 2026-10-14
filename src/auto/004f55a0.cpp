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

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(char, __fastcall, sub_4F54A0, 0x004F54A0, (void* self, void* edx, int a2, char a3, char a4, char a5));
CSF_FUNC(int, __fastcall, SceneSubsystem_LookupEntry, 0x00519AF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneMissionSetup_GetAnimationEntry, 0x00772120, (void* self, void* edx, int a2));

// Resolves the object stored in the global context at +0x1FC4 and calls its
// virtual slot 2 (offset +8) with the entry id, returning the slot index result.
static int __stdcall ApplyAnimationEntryToProvider_Dummy(int) { return 0; }

static char __fastcall ApplyAnimationEntryToProvider(S_004f5420* self, void* /*edx*/, int a2)
{
    uint8_t* context = reinterpret_cast<uint8_t*>(GetValue8B0554());
    void* providerObject = CSF_AT(void*, context, 0x1FC4);
    int slotIndex = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, providerObject, 0)[2])(providerObject, nullptr, a2);
    int resultIndex = slotIndex;
    char result = static_cast<char>(resultIndex);

    if (resultIndex != -1)
    {
        uint8_t* animationEntry = reinterpret_cast<uint8_t*>(
            SceneMissionSetup_GetAnimationEntry(CSF_AT(void*, GetValue8B0554(), 0x1FC4), nullptr, resultIndex));

        int lookupValue = SceneSubsystem_LookupEntry(CSF_AT(void*, GetValue8B0554(), 0x1FC4), nullptr, resultIndex);

        result = sub_4F54A0(self, nullptr,
                            lookupValue,
                            static_cast<char>(CSF_AT(uint8_t, animationEntry, 0x58)),
                            static_cast<char>(CSF_AT(uint8_t, animationEntry, 0x59)),
                            static_cast<char>(CSF_AT(uint8_t, animationEntry, 0x5A)));
    }
    return result;
}

HOOK(0x004F55A0, ApplyAnimationEntryToProvider, "auto");
