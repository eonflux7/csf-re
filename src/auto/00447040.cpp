#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t*, __fastcall, SpatialQuery_AppendEntry, 0x00446C80,
         (void* self, void* edx, uint32_t* a2, int a3));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30,
         (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall SpatialQuery_CollectEntriesNearObject(
    void* self, void* /*edx*/, int handle, S_00446450* results)
{
    results->field_0008 = 0;

    void* object = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));

    if (object)
    {
        float* position = GetObjectPositionVector(object, nullptr);
        uint32_t remaining = CSF_AT(uint32_t, self, 0x24);
        uint32_t offset = 0;

        while (remaining != 0)
        {
            auto* entry = CSF_AT(uint8_t*, self, 0x1C) + offset;

            if (CSF_AT(int32_t, entry, 0) != -1)
            {
                float dx = position[0] - CSF_AT(float, entry, 0x14);
                float dy = position[1] - CSF_AT(float, entry, 0x18);
                float dz = position[2] - CSF_AT(float, entry, 0x1C);

                void* settings =
                    reinterpret_cast<void*>(GetGlobalValue_8BC1E0());

                if (std::fabs(dy) <= CSF_AT(float, settings, 0xACC))
                {
                    float radius = CSF_AT(float, entry, 0x24);
                    if (radius * radius > dz * dz + dx * dx + dy * dy)
                    {
                        SpatialQuery_AppendEntry(
                            results, nullptr,
                            reinterpret_cast<uint32_t*>(entry), 0);
                    }
                }
            }

            offset += 40;
            --remaining;
        }
    }

    return results->field_0008 != 0;
}

HOOK(0x00447040, SpatialQuery_CollectEntriesNearObject, "auto");
