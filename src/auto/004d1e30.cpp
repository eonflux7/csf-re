#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, WeaponFirearms_ActivateContext, 0x006C8550, (void* self, void* edx));
CSF_FUNC(int, __fastcall, WeaponFirearms_UpdateContextFromObject, 0x006C8740, (void* self, void* edx, int a2, float a3));

static int __fastcall S_004d1e30_UpdateWeaponContext(
    S_004d1e30* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(self->field_0014);
    void* collection = CSF_AT(void*, owner, 0x44);
    void** collectionVtable = CSF_AT(void**, collection, 0);

    int currentIndex =
        ((int(__fastcall*)(void*, void*))collectionVtable[0xFC / 4])(
            collection, nullptr);
    void* entry =
        ((void*(__fastcall*)(void*, void*, int))collectionVtable[0xC8 / 4])(
            collection, nullptr, currentIndex);
    if (!entry)
        return 0;

    void* resources =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, entry, 0)[0xA8 / 4])(
            entry, nullptr);
    void* context =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, entry, 0)[0xAC / 4])(
            entry, nullptr);

    if (!resources || !context)
        return reinterpret_cast<int>(context);

    int resourceIndex =
        ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, resources, 0)[0x98 / 4])(
            resources, nullptr, 100);
    if (resourceIndex == -1)
        return resourceIndex;

    void* resource =
        ((void*(__fastcall*)(void*, void*, int))CSF_AT(void**, resources, 0)[0x8C / 4])(
            resources, nullptr, resourceIndex);
    if (!resource)
        return 0;

    WeaponFirearms_ActivateContext(context, nullptr);

    alignas(8) std::uint8_t scratchBytes[0x10];
    owner = reinterpret_cast<void*>(self->field_0014);

    // The pending push of this address is not an argument to the getter.
    void* descriptor =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0xA4 / 4])(
            owner, nullptr);
    int object =
        ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, resource, 0)[0x18 / 4])(
            resource, nullptr, CSF_AT(int, descriptor, 0x4));

    // That pending stack word becomes the final call's float argument.
    std::uintptr_t scratchAddress =
        reinterpret_cast<std::uintptr_t>(scratchBytes);
    float legacyArgument;
    std::memcpy(&legacyArgument, &scratchAddress, sizeof(legacyArgument));

    return WeaponFirearms_UpdateContextFromObject(
        context, nullptr, object, legacyArgument);
}

HOOK(0x004D1E30, S_004d1e30_UpdateWeaponContext, "auto");
