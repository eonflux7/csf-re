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

CSF_FUNC(int, __fastcall, sub_6CCB70, 0x006CCB70, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_FinalizeObjectProvider, 0x006D4450, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, sub_6CE180, 0x006CE180, (void* self, void* edx));
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_6D6C00, 0x006D6C00, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_6BC080, 0x006BC080, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_6B6BC0, 0x006B6BC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(uint32_t*, __fastcall, sub_6D1180, 0x006D1180, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_6EF7B0, 0x006EF7B0, (void* self, void* edx));

// Vtable of the object type handled by SceneLoader_FinalizeObjectProvider (referenced at 0x6CCF84).
[[maybe_unused]] static const void* const kFinalizedProviderVtbl = CSF_VTBL(0x006D4450);

static int __fastcall C_00860bb8_AttachWorldClumpsAndFinalize(C_00860bb8* self, void* /*edx*/)
{
    self->field_0069 = 1;
    sub_6CCB70(self, nullptr);

    int globalInt = GetGlobalInt();
    int accessor = GlobalInt_ResolveAccessor(reinterpret_cast<void*>(globalInt), nullptr);
    void* cachedProvider = GetCachedObjectProvider(reinterpret_cast<void*>(accessor), nullptr);
    SceneLoader_FinalizeObjectProvider(cachedProvider, nullptr);

    int worldObjectCollection = SceneLoader_GetWorldObjectCollection(reinterpret_cast<void*>(accessor), nullptr);
    sub_6CE180(reinterpret_cast<void*>(worldObjectCollection), nullptr);

    if (self->field_006a)
    {
        int sceneObject = GetGlobalInt();
        int renderHandle = RenderHandle_FromObject(reinterpret_cast<void*>(sceneObject), nullptr);
        if (renderHandle)
            sub_6D6C00(reinterpret_cast<void*>(renderHandle), nullptr);
    }

    sub_6BC080(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008)), nullptr);
    sub_6B6BC0(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_000c)), nullptr);

    int clumpManagerObject = GetGlobalInt();
    int clumpManagerId = ResolveClumpManager(clumpManagerObject, nullptr);
    int result = static_cast<int>(reinterpret_cast<intptr_t>(
        sub_6D1180(reinterpret_cast<void*>(clumpManagerId), nullptr)));

    if (self->field_0064)
    {
        // Tail jump: ecx = field_0064 is forwarded as the callee's this.
        void* tailSelf = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0064));
        return static_cast<int>(sub_6EF7B0(tailSelf, nullptr));
    }
    return result;
}

HOOK(0x006CCF60, C_00860bb8_AttachWorldClumpsAndFinalize, "auto");
