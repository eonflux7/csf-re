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

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

// The getter returns a raw object address as int; reinterpret it as a pointer for field access.
static void* ToObjectPtr(int raw) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(raw)));
}

// Returns nonzero when both minimap viewport limits are strictly below their reference values.
static int __cdecl IsMinimapViewportWithinBounds() {
    int firstSingleton = GetValue8B0554();

    float referenceX = CSF_AT(float, ToObjectPtr(GetValue8B0554()), 0xdb4);
    float referenceY = CSF_AT(float, ToObjectPtr(firstSingleton), 0xdbc);

    int secondSingleton = GetValue8B0554();

    float limitX = CSF_AT(float, ToObjectPtr(GetValue8B0554()), 0xdc0);
    float limitY = CSF_AT(float, ToObjectPtr(secondSingleton), 0xdc8);

    if (limitX < referenceX && limitY < referenceY) {
        return 1;
    }
    return 0;
}

HOOK(0x0043D2C0, IsMinimapViewportWithinBounds, "auto");
