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

CSF_GLOBAL(std::int32_t, g_PTankPluginOffset, 0x008B09EC);

// Stores a2 into the PTank plugin object (+0xC4), mirrors that value into the
// target reached through field_0004 -> +0x18 -> +0x20 -> [0] (when non-null),
// then sets bit 0x2000 in the plugin object's flags (+0x40) and returns the plugin object.
static int __fastcall PTankAtomic_StoreCreateValueAndSetFlag(C_00862110* self, void* /*edx*/, int a2)
{
    // Plugin object pointer: *(uint32*)(g_PTankPluginOffset + self->field_0004)
    uint32_t pluginObj = *reinterpret_cast<uint32_t*>(
        static_cast<uintptr_t>(static_cast<uint32_t>(g_PTankPluginOffset) + self->field_0004));
    CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(pluginObj)), 0xC4) = a2;

    uint32_t fieldValue = self->field_0004;
    uint32_t innerA = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(fieldValue)), 0x18);
    uint32_t innerB = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(innerA)), 0x20);
    uint32_t target = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(innerB)), 0);

    if (target != 0) {
        uint32_t pluginFromField = *reinterpret_cast<uint32_t*>(
            static_cast<uintptr_t>(fieldValue + static_cast<uint32_t>(g_PTankPluginOffset)));
        uint32_t storedValue = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(pluginFromField)), 0xC4);
        CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(target)), 4) = storedValue;
    }

    uint32_t resultObj = *reinterpret_cast<uint32_t*>(
        static_cast<uintptr_t>(static_cast<uint32_t>(g_PTankPluginOffset) + self->field_0004));
    CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(resultObj)), 0x40) |= 0x2000u;
    return static_cast<int>(resultObj);
}

HOOK(0x006EEC00, PTankAtomic_StoreCreateValueAndSetFlag, "auto");
