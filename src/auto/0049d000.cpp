#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(unsigned int, __fastcall, RegisterSceneObjects, 0x00475800, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, S_0048e570_ActivateFocusEntry, 0x0048E570, (S_0048e570* self, void* , int* entry));

static int __fastcall S_0048e570_ApplyEntryAndActivate(S_0048e570* self, void* /*edx*/, int* a2)
{
    self->field_0180 = static_cast<uint32_t>(a2[8]);
    self->field_0184 = static_cast<uint32_t>(a2[9]);
    self->field_0188 = static_cast<uint32_t>(a2[6]);
    self->field_0179 = 0;

    uint8_t* sceneArg = nullptr;
    if (self != reinterpret_cast<S_0048e570*>(12))
        sceneArg = reinterpret_cast<uint8_t*>(self) + 4;
    int sceneArgValue = static_cast<int>(reinterpret_cast<uintptr_t>(sceneArg));

    S_0044d360* sceneRoot =
        reinterpret_cast<S_0044d360*>(static_cast<uintptr_t>(GetGlobalObjectField184()));
    RegisterSceneObjects(sceneRoot, nullptr, sceneArgValue);

    return S_0048e570_ActivateFocusEntry(self, nullptr, a2);
}

HOOK(0x0049D000, S_0048e570_ApplyEntryAndActivate, "auto");
