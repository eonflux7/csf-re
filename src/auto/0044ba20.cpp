#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 12-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(int, __cdecl, AiAttackTasks_ConstructTangent, 0x0044AE20,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the 12-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_InsertTangent, 0x0044B930,
         (void* self, void* edx, uint32_t* a2, char* a3, uint32_t* a4));

// Appends one 12-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to AiAttackTasks_InsertTangent.
static uint32_t* __fastcall AiAttackTasks_AppendTangent(
    S_0044b930* self, void* /*edx*/, uint32_t* element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x0044B930);

    const uint32_t begin = self->field_0004;
    uint32_t index = 0;

    if (begin != 0)
    {
        index = static_cast<uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 12);
    }

    if (begin == 0 ||
        index >= static_cast<uint32_t>(
                     static_cast<std::int32_t>(self->field_000c - begin) / 12))
    {
        return AiAttackTasks_InsertTangent(
            self, nullptr,
            reinterpret_cast<uint32_t*>(&element),
            reinterpret_cast<char*>(static_cast<std::uintptr_t>(self->field_0008)),
            element);
    }

    const uint32_t end = self->field_0008;
    uint32_t* result = reinterpret_cast<uint32_t*>(static_cast<std::intptr_t>(
        AiAttackTasks_ConstructTangent(
            static_cast<int>(end), 1,
            static_cast<int>(reinterpret_cast<std::intptr_t>(element)),
            static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
            static_cast<int>(reinterpret_cast<std::intptr_t>(element)))));

    self->field_0008 = end + 12u;
    return result;
}

HOOK(0x0044BA20, AiAttackTasks_AppendTangent, "auto");
