#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, SceneObject_IsSecondCheckTrue, 0x0048E140, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(char, __fastcall, SceneObject_CheckWithOwnerFlags, 0x0048E5D0, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, CharTask_ResolveStateSlot, 0x004DB440, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, StateSlot_ApplyValue, 0x00421A40, (void* self, void* edx, int a2));

// Reads self->field_0008 (owner), then owner->field_0014 (the scene object it drives).
// Re-read at each use, as the original does.
static char __fastcall C_00834fa8_ApplyValueIfSceneObjectReady(C_00834fa8* self, void* /*edx*/, int* a2)
{
    char result = static_cast<char>(SceneObject_IsSecondCheckTrue(
        CSF_AT(void*, reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08)), 0x14), nullptr));
    if (result == 0)
        return result;

    result = static_cast<char>(IsField1BCNonNegative(
        CSF_AT(void*, reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08)), 0x14), nullptr));
    if (result != 0)
        return result;

    result = SceneObject_CheckWithOwnerFlags(
        CSF_AT(void*, reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08)), 0x14),
        nullptr,
        static_cast<int>(CSF_AT(uint32_t, self, 0x08)),
        2,
        1);
    if (result == 0)
        return result;

    if ((CSF_AT(uint8_t, CSF_AT(void*, reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08)), 0x14), 0x1F0) & 8) != 0)
    {
        result = static_cast<char>(*a2);
        CSF_AT(int, reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x0C)), 0x5C) = *a2;
        return result;
    }

    int value = *a2;
    int slotAddress = static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 0x48);
    int slot = CharTask_ResolveStateSlot(self, nullptr, slotAddress);
    result = static_cast<char>(StateSlot_ApplyValue(
        reinterpret_cast<void*>(static_cast<uintptr_t>(slot)), nullptr, value));
    return result;
}

HOOK(0x00422370, C_00834fa8_ApplyValueIfSceneObjectReady, "auto");
