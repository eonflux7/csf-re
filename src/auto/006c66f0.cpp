#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, GfxSkin_IsObjectSupported, 0x006B3820, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxSkin_CheckObjectEligibility, 0x006B3870, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxSkin_ProcessTypeFiveHandle, 0x006C63D0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, GfxSkin_RegisterObjectProvider, 0x006DA700, (void* self, void* edx, void* a2, int a3));

static char __fastcall C_008656d0_ProcessObject(
    C_008656d0* self, void* /*edx*/, C_0085e7b8* object)
{
    ++CSF_AT(uint32_t, self, 0xF8);

    const int handleValue = RenderHandle_FromObject(object, nullptr);
    void* handle = reinterpret_cast<void*>(handleValue);
    void* handleObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, handle, 0)[0x1C / 4])(handle, nullptr);

    void* provider = GetCachedObjectProvider(handleObject, nullptr);
    if (reinterpret_cast<uintptr_t>(
            GetReferencedObject(provider, nullptr)) == 5)
    {
        const char result =
            GfxSkin_ProcessTypeFiveHandle(self, nullptr, handleValue);
        if (result)
            ++CSF_AT(uint32_t, self, 0x108);
        return result;
    }

    const bool supported = GfxSkin_IsObjectSupported(object, nullptr);
    if (supported)
    {
        if (GfxSkin_CheckObjectEligibility(object, nullptr))
        {
            void* flags =
                reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0x20 / 4])(object, nullptr);

            if ((CSF_AT(uint32_t, flags, 0) & 0x4000) != 0 &&
                static_cast<int8_t>(self->field_0008) < 0)
            {
                const int managerId = GetGlobalInt();
                const int manager = ResolveClumpManager(managerId, nullptr);
                const int entry = ResolveEntryLookupValue(
                    reinterpret_cast<void*>(manager), nullptr);
                void* objectProvider = GetCachedObjectProvider(
                    CSF_AT(void*, self, 0xF0), nullptr);
                GfxSkin_RegisterObjectProvider(
                    reinterpret_cast<void*>(entry), nullptr, object,
                    reinterpret_cast<int>(objectProvider));
            }
        }
        ++CSF_AT(uint32_t, self, 0x108);
    }

    return static_cast<char>(supported);
}

HOOK(0x006C66F0, C_008656d0_ProcessObject, "auto");
