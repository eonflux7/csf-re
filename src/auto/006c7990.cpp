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

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(unsigned int, __fastcall, SkinState_PreUpdate, 0x006C5F30, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SkinState_ApplyFlag800, 0x006C5F70, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SkinState_ApplyFlag20, 0x006C6190, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SkinState_ApplyFlag40, 0x006C7500, (void* self, void* edx));

static char __fastcall SkinState_RunFlaggedPhases(S_006c5cd0* self, void* /*edx*/)
{
    self->field_00f5 = 1;
    if ((self->field_0008 & 1) != 0)
        SkinState_PreUpdate(self, nullptr);

    void* firstObject = LookupObjectById(
        reinterpret_cast<void*>(static_cast<std::intptr_t>(GetGlobalInt())), nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, firstObject, 0)[15])(firstObject, nullptr, 1);

    if ((self->field_0008 & 0x800) != 0)
        SkinState_ApplyFlag800(self, nullptr);

    void* secondObject = LookupObjectById(
        reinterpret_cast<void*>(static_cast<std::intptr_t>(GetGlobalInt())), nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, secondObject, 0)[15])(secondObject, nullptr, 2);

    if ((self->field_0008 & 0x20) != 0)
        SkinState_ApplyFlag20(self, nullptr);

    if ((self->field_0008 & 0x40) != 0)
    {
        char result = static_cast<char>(SkinState_ApplyFlag40(self, nullptr));
        self->field_00f5 = 0;
        return result;
    }

    self->field_00f5 = 0;
    return 1;
}

HOOK(0x006C7990, SkinState_RunFlaggedPhases, "auto");
