#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee bindings (by original address)
CSF_FUNC(int, __fastcall, SceneFindSubObject, 0x004DE7A0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(char, __fastcall, SubObjectPair_Process, 0x004E0730, (void* self, void* edx, float* a2, uint32_t* a3, int a4, int a5, int a6, char a7));

// Returns true when the sub-object is present and neither of the pair is flagged (bit 0 of byte +4 clear).
static bool IsSubObjectUsable(uint32_t subObject)
{
    uint8_t* sub = reinterpret_cast<uint8_t*>(subObject);
    return (CSF_AT(uint8_t, sub, 4) & 1) == 0;
}

static char __fastcall S_004dfbc0_TrySetupSubObjectPair(S_004dfbc0* self, void* /*edx*/, unsigned int a2, int a3)
{
    uint8_t* obj = reinterpret_cast<uint8_t*>(a2);

    uint32_t subObject = CSF_AT(uint32_t, obj, 0x18);
    if (!subObject)
        subObject = static_cast<uint32_t>(SceneFindSubObject(reinterpret_cast<void*>(self->field_0004), nullptr, a2));

    uint32_t otherSubObject = CSF_AT(uint32_t, obj, 0x1C);
    if (!otherSubObject)
        otherSubObject = static_cast<uint32_t>(SceneFindSubObject(reinterpret_cast<void*>(self->field_0004), nullptr, a2 + 12));

    if (!subObject || !otherSubObject || !IsSubObjectUsable(subObject) || !IsSubObjectUsable(otherSubObject))
        return 0;

    self->field_0008 = CSF_AT(uint32_t, obj, 0x24);
    char result = SubObjectPair_Process(
        self, nullptr,
        reinterpret_cast<float*>(obj),
        reinterpret_cast<uint32_t*>(obj + 12),
        static_cast<int>(subObject),
        static_cast<int>(otherSubObject),
        a3,
        static_cast<char>(CSF_AT(uint32_t, obj, 0x20)));
    self->field_0008 = 0x7f7fffff;
    return result;
}

HOOK(0x4E07B0, S_004dfbc0_TrySetupSubObjectPair, "auto");
