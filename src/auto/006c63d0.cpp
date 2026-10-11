#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int*, __fastcall, GfxSkin_GetEntryProperty, 0x006C4960, (void* self, void* edx, int* a2, int a3));
CSF_FUNC(int, __cdecl, GfxSkin_CheckEntryVariant, 0x006C5990, (int a2));
CSF_FUNC(int, __stdcall, GfxSkin_ApplyDirectParameters, 0x006C6340, (int a1, void* a2));
CSF_FUNC(int, __stdcall, GfxSkin_ApplyVariantParameters, 0x006C6370, (int a1, uint32_t* a2));

static char __fastcall C_008656d0_ApplyEntryParameters(
    C_008656d0* self, void* /*edx*/, int entryAddress)
{
    const int entry = entryAddress;
    void* object = reinterpret_cast<void*>(entry);
    const bool ready =
        reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x10 / 4])(object, nullptr);
    if (!ready)
        return 0;

    const bool hasVariant =
        static_cast<uint8_t>(GfxSkin_CheckEntryVariant(entry)) != 0;

    void* providerSource =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr);
    void* provider = GetCachedObjectProvider(providerSource, nullptr);
    const uintptr_t type =
        reinterpret_cast<uintptr_t>(GetReferencedObject(provider, nullptr));

    auto* bytes = reinterpret_cast<uint8_t*>(self);
    switch (type)
    {
    case 0:
    case 8:
    case 9:
        if (hasVariant)
            break;

        providerSource =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr);
        if (*GfxSkin_GetEntryProperty(
                providerSource, nullptr, &entryAddress, 29) == 8)
        {
            GfxSkin_ApplyDirectParameters(entry, bytes + 0x60);
        }
        else
        {
            GfxSkin_ApplyVariantParameters(
                entry, reinterpret_cast<uint32_t*>(bytes + 0x70));
        }
        return 1;

    case 1:
        GfxSkin_ApplyDirectParameters(entry, bytes + 0x50);
        return 1;

    case 4:
        if (hasVariant)
        {
            GfxSkin_ApplyVariantParameters(
                entry, reinterpret_cast<uint32_t*>(bytes + 0x40));
        }
        else
        {
            GfxSkin_ApplyDirectParameters(entry, &self->field_0030);
        }
        return 1;

    case 5:
        if (hasVariant)
        {
            GfxSkin_ApplyVariantParameters(
                entry, reinterpret_cast<uint32_t*>(bytes + 0xD0));
        }
        else
        {
            GfxSkin_ApplyDirectParameters(entry, bytes + 0xC0);
        }
        return 1;

    case 6:
        GfxSkin_ApplyVariantParameters(
            entry, reinterpret_cast<uint32_t*>(bytes + 0x70));
        return 1;

    case 7:
        break;

    case 12:
        GfxSkin_ApplyDirectParameters(entry, bytes + 0xB0);
        return 1;

    default:
        if (!hasVariant)
        {
            GfxSkin_ApplyDirectParameters(entry, bytes + 0x60);
            return 1;
        }
        break;
    }

    const int variant =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x30 / 4])(object, nullptr);
    switch (variant)
    {
    case 0:
        GfxSkin_ApplyVariantParameters(
            entry, reinterpret_cast<uint32_t*>(bytes + 0xA0));
        break;
    case 1:
        GfxSkin_ApplyVariantParameters(
            entry, reinterpret_cast<uint32_t*>(bytes + 0x90));
        break;
    case 2:
        GfxSkin_ApplyVariantParameters(
            entry, reinterpret_cast<uint32_t*>(bytes + 0x80));
        break;
    case 3:
        GfxSkin_ApplyVariantParameters(
            entry, reinterpret_cast<uint32_t*>(bytes + 0x40));
        break;
    default:
        break;
    }
    return 1;
}

HOOK(0x006C63D0, C_008656d0_ApplyEntryParameters, "auto");
