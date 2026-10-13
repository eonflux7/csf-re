#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));

// Stores a2 as the attached region at +0x0C, then forwards GetReferencedObject(a2)
// to the vtable slot +0x10 of the object held at +0x08. Returns that call's result.
static int __fastcall C_0085ec18_AttachRegionToEntity(C_0085ec18* self, void* /*edx*/, C_0085ec18* a2)
{
    void* owner = CSF_AT(void*, self, 0x8);
    void** ownerVtable = CSF_AT(void**, owner, 0);

    CSF_AT(void*, self, 0xC) = a2;

    void* referencedObject = GetReferencedObject(a2, nullptr);

    void* currentOwner = CSF_AT(void*, self, 0x8);
    return ((int(__fastcall*)(void*, void*, void*))ownerVtable[4])(currentOwner, nullptr, referencedObject);
}

HOOK(0x004FBBE0, C_0085ec18_AttachRegionToEntity, "auto");
