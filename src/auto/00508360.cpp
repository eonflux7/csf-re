#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AppMain_DestroyResourceArray, 0x00507DE0,
         (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, AppMain_DestroyReferencedObjectArray, 0x00507EA0,
         (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, AppMain_DestroyAuxiliaryResourceArray, 0x00508180,
         (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0,
         (void* self, void* edx));

static unsigned int __fastcall S_00508360_ClearResources(
    S_00508360* self, void* /*edx*/)
{
    unsigned int result = self->field_004c;
    unsigned int index = 0;
    std::uint32_t entryOffset = 0;

    while (index < result)
    {
        auto* entry =
            reinterpret_cast<std::uint8_t*>(self->field_0044) + entryOffset;
        void* object = CSF_AT(void*, entry, 0x1c);

        ((void(__fastcall*)(void*, void*, unsigned int))
             CSF_AT(void**, object, 0)[0x30 / 4])(
            object, nullptr, CSF_AT(std::uint8_t, entry, 0x20));

        ReleaseObjectReference(CSF_AT(void*, entry, 0x1c), nullptr);

        result = self->field_004c;
        ++index;
        entryOffset += 36;
    }

    void* resources = reinterpret_cast<void*>(self->field_0034);
    self->field_0038 = 0;
    self->field_003c = 0;
    if (resources)
    {
        result = AppMain_DestroyResourceArray(resources, nullptr, 3);
        self->field_0034 = 0;
    }

    void* entries = reinterpret_cast<void*>(self->field_0044);
    self->field_0048 = 0;
    self->field_004c = 0;
    if (entries)
    {
        result = AppMain_DestroyReferencedObjectArray(entries, nullptr, 3);
        self->field_0044 = 0;
    }

    void* auxiliaryResources = reinterpret_cast<void*>(self->field_0054);
    self->field_0058 = 0;
    self->field_005c = 0;
    if (auxiliaryResources)
    {
        result = AppMain_DestroyAuxiliaryResourceArray(
            auxiliaryResources, nullptr, 3);
        self->field_0054 = 0;
    }

    void* ownedObject = reinterpret_cast<void*>(self->field_0030);
    if (ownedObject)
    {
        result = ((int(__fastcall*)(void*, void*, int))
                      CSF_AT(void**, ownedObject, 0)[0])(
            ownedObject, nullptr, 1);
        self->field_0030 = 0;
    }

    return result;
}

HOOK(0x00508360, S_00508360_ClearResources, "auto");
