#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpUserDataArrayGetName, 0x005870B0, (int a1));
CSF_FUNC(int, __cdecl, RpGeometryGetUserDataArrayCount, 0x0058BCE0, (int a1));
CSF_FUNC(int, __cdecl, RpGeometryGetUserDataArray, 0x0058BD10, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpUserDataArrayGetInt, 0x0058BE50, (int a1, int a2));

static bool __cdecl GfxObjectProps_IsNamedGeometryUserDataZero(
    void* object, const char* propertyName)
{
    const int geometry = CSF_AT(int, object, 0x18);
    bool isZero = false;

    if (geometry)
    {
        const uint32_t count =
            static_cast<uint32_t>(RpGeometryGetUserDataArrayCount(geometry));

        for (uint32_t index = 0; index < count; ++index)
        {
            const int array =
                RpGeometryGetUserDataArray(geometry, static_cast<int>(index));
            const char* name =
                reinterpret_cast<const char*>(RpUserDataArrayGetName(array));

            if (std::strcmp(name, propertyName) == 0)
                isZero = RpUserDataArrayGetInt(array, 0) == 0;
        }
    }

    return isZero;
}

HOOK(0x006B8900, GfxObjectProps_IsNamedGeometryUserDataZero, "auto");
