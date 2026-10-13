#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, ResetGlobalValueObject, 0x00457AF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);
CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);

// Vtable of S_00457af0 as referenced by the original's call to 0x457af0.
[[maybe_unused]] static const void* const s_portraitOwnerVtable = CSF_VTBL(0x00457AF0);

// Integer handle returned by the engine lookups, used as an object pointer.
static void* IntHandleToPointer(int handle)
{
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(static_cast<std::uint32_t>(handle)));
}

static char __fastcall S_00457af0_UpdatePortraitPlayback(S_00457af0* self, void* /*edx*/, float elapsedFraction)
{
    if (self->field_0020 > g_flt_0082DC84)
        return ResetGlobalValueObject(self, nullptr);

    int globalValue = GetGlobalValue_4FBDE0();
    void* firstElement = IntHandleToPointer(GetFirstElement(IntHandleToPointer(globalValue), nullptr));
    void* cachedProvider = GetCachedObjectProvider(firstElement, nullptr);
    void* referencedObject = GetReferencedObject(cachedProvider, nullptr);

    // Slot 0x10 / 4 = 4 on the referenced object returns the child object.
    void* childObject = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, referencedObject, 0)[4])(referencedObject, nullptr);

    float scaledTime = elapsedFraction * self->field_0024 * g_MessageBoxAnimationScale;

    // Slot 0x40 / 4 = 16 on the child object, called with the scaled time.
    return ((char (__fastcall*)(void*, void*, float))CSF_AT(void**, childObject, 0)[16])(childObject, nullptr, scaledTime);
}

HOOK(0x00457ED0, S_00457af0_UpdatePortraitPlayback, "auto");
