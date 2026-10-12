#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, ResetPrimarySubsystem, 0x00448350, (void* self, void* edx));
static CSF_FUNC(void, __fastcall, ResetFinalSubsystem, 0x004483A0, (void* self, void* edx));
static CSF_FUNC(int, __cdecl, ReleaseSubsystemEntries, 0x004490D0, (int a1, int a2, int a3, int a4));
static CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
static CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void __fastcall C_0083114c_ResetSubsystemC4(C_0083114c* self, void* /*edx*/)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);
    ResetPrimarySubsystem(bytes + 0x04, nullptr);

    if (self->field_0018)
    {
        ReleaseSubsystemEntries(
            static_cast<int>(self->field_0018),
            static_cast<int>(CSF_AT(uint32_t, self, 0x1C)),
            reinterpret_cast<int>(bytes + 0x14),
            0);
        if (self->field_0018)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0018));
    }

    self->field_0018 = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;

    auto* object = reinterpret_cast<uint8_t*>(self->field_002c);
    self->field_0030 = 0;
    self->field_0034 = 0;
    if (object)
    {
        if (CSF_AT(uint32_t, object - 4, 0))
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0])(object, nullptr, 3);
        }
        else
        {
            Mem_Free(object - 4);
        }
        self->field_002c = 0;
    }

    ResetFinalSubsystem(bytes + 0x3C, nullptr);
}

HOOK(0x0044A250, C_0083114c_ResetSubsystemC4, "auto");
