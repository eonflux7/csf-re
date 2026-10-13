#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 12-byte entry at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(std::uint32_t*, __cdecl, AiAttackTasks_ConstructEntityHandle, 0x0044B070,
         (std::uint32_t* a1, int a2, int a3, int a4, int a5));

// Grows the 12-byte-entry array and inserts one entry (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, AiAttackTasks_InsertEntityHandle, 0x0044B8C0,
         (void* self, void* edx, std::uint32_t* a2, int a3, int a4));

// Appends one 12-byte entry to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to AiAttackTasks_InsertEntityHandle.
static std::uint32_t* __fastcall AiAttackTasks_AppendEntityHandle(
    S_0044b8c0* self, void* /*edx*/, int entityHandle)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x0044B8C0);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t index = 0;

    if (begin != 0)
    {
        index = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 12);
    }

    if (begin == 0 ||
        index >= static_cast<std::uint32_t>(
                     static_cast<std::int32_t>(self->field_000c - begin) / 12))
    {
        return AiAttackTasks_InsertEntityHandle(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&entityHandle),
            static_cast<int>(self->field_0008),
            entityHandle);
    }

    const std::uint32_t end = self->field_0008;
    std::uint32_t* result = AiAttackTasks_ConstructEntityHandle(
        reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(end)), 1,
        entityHandle,
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        entityHandle);

    self->field_0008 = end + 12u;
    return result;
}

HOOK(0x0044B9A0, AiAttackTasks_AppendEntityHandle, "auto");
