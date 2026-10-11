#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(char, __fastcall, IsAssociationEnabled, 0x0040EC00,
                (void* self, void* edx));
static CSF_FUNC(char, __fastcall, MatchesAssociatedObject, 0x0040F0A0,
                (void* self, void* edx, void* a2));

static char __fastcall CanAssociateObject(void* self, void* /*edx*/, void* object)
{
    if (!IsAssociationEnabled(CSF_AT(void*, self, 0x4C), nullptr))
        return 0;

    if (!object)
        return 0;

    if (!reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr))
        return 0;

    void* intermediate = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr);

    void* associatedObject = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, intermediate, 0)[0x28 / 4])(intermediate, nullptr);

    return MatchesAssociatedObject(
        CSF_AT(void*, self, 0x4C), nullptr, associatedObject);
}

HOOK(0x00491FD0, CanAssociateObject, "auto");
