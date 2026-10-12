#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
static CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
static CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
static CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
static CSF_FUNC(int, __stdcall, GfxSkin_ForwardWithSkinCallback, 0x006C5CB0, (int a1));
static CSF_FUNC(int, __fastcall, GfxTextureLoader_FindResourceIndex, 0x006D4600, (void* self, void* edx, int a2));
static CSF_FUNC(int, __fastcall, GfxTextureLoader_ReleaseIndexedResource, 0x006D4790, (void* self, void* edx, int a1, int a2));

static int __fastcall S_006dcf90_ReleaseResource(S_006dcf90* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0004);
    if (result != 0)
    {
        const int resource = static_cast<int>(self->field_0004);
        GetGlobalValue_6C5760();
        GfxSkin_ForwardWithSkinCallback(resource);

        int global = GetGlobalInt();
        int accessor = GlobalInt_ResolveAccessor(reinterpret_cast<void*>(global), nullptr);
        void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(accessor), nullptr);

        result = GfxTextureLoader_FindResourceIndex(
            provider, nullptr, static_cast<int>(self->field_0004));
        if (result != -1)
        {
            result = GfxTextureLoader_ReleaseIndexedResource(
                provider, nullptr, result, static_cast<int>(self->field_0004));
        }
        self->field_0004 = 0;
    }
    return result;
}

HOOK(0x006DCF90, S_006dcf90_ReleaseResource, "auto");
