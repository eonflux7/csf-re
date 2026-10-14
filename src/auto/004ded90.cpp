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

// Scene file helpers and the scene-state methods this function calls.
CSF_FUNC(void*, __cdecl, OpenSceneFile, 0x00698350, (const char* path, const char* mode, int flags));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(int, __fastcall, SceneResource_Reset, 0x004DE880, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneResource_ParseHeader, 0x004DEAD0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, SceneResource_LoadBlock, 0x004DE680, (void* self, void* edx, unsigned int* a2));

// Vtable of S_004de880, referenced by the constructor-like call at 0x4DED9B.
[[maybe_unused]] static const void* const kSceneResourceVtbl = CSF_VTBL(0x004DE880);

static char __fastcall ApplySceneResourceName(S_004de880* self, void* /*edx*/, int a2)
{
    self->field_0005 = 0;
    SceneResource_Reset(self, nullptr);

    void* sceneFile = OpenSceneFile(reinterpret_cast<const char*>(a2), CSF_STR(0x0083E130, "r"), 0);
    if (sceneFile)
    {
        // Virtual read at vtable slot 2: the 4-byte header word is read into the parameter slot a2 itself
        // (the original's [ebp+8]) and tested from there; if the read leaves it alone, a2 is still tested.
        ((void(__fastcall*)(void*, void*, int*, int))CSF_AT(void**, sceneFile, 0)[2])(
            sceneFile, nullptr, &a2, 4);

        if (!a2)
            self->field_0005 = SceneResource_ParseHeader(self, nullptr, reinterpret_cast<uint32_t*>(sceneFile));

        if (self->field_0005)
            SceneResource_LoadBlock(reinterpret_cast<void*>(self->field_0018), nullptr,
                                    reinterpret_cast<unsigned int*>(&self->field_0020));

        CloseSceneFile(sceneFile);
    }
    return static_cast<char>(self->field_0005);
}

HOOK(0x004DED90, ApplySceneResourceName, "auto");
