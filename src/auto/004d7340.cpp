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

// Forwarder: returns pointer to the object's position vector (three dwords).
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (C_008342e8* self, void* edx));
// Returns the resolved scalar (double); the original stores it as a float.
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void* edx));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));

// Caches the owner object's placement data the first time it is queried.
// Returns the low byte of the resolved value pointer (or of the vtable query result).
static char __fastcall TAutonomo_CacheObjectPlacementOnce(void* self, void* /*edx*/)
{
    char result = CSF_AT(char, self, 0x64);
    if (result == 0)
    {
        CSF_AT(uint8_t, self, 0x64) = 1;

        void* owner = CSF_AT(void*, self, 0x08);
        void* sceneObject = CSF_AT(void*, owner, 0x14);

        uint32_t* position = reinterpret_cast<uint32_t*>(
            GetObjectPositionVector(reinterpret_cast<C_008342e8*>(sceneObject), nullptr));
        CSF_AT(uint32_t, self, 0x68) = position[0];
        CSF_AT(uint32_t, self, 0x6C) = position[1];
        CSF_AT(uint32_t, self, 0x70) = position[2];

        owner = CSF_AT(void*, self, 0x08);
        sceneObject = CSF_AT(void*, owner, 0x14);
        CSF_AT(float, self, 0x74) = static_cast<float>(
            C_008342e8_ComputeResolvedObjectScalar(reinterpret_cast<C_008342e8*>(sceneObject), nullptr));

        owner = CSF_AT(void*, self, 0x08);
        sceneObject = CSF_AT(void*, owner, 0x14);
        CSF_AT(uint32_t, self, 0x78) = CSF_AT(uint32_t, sceneObject, 0x1D8);
        CSF_AT(uint32_t, self, 0x7C) = CSF_AT(uint32_t, sceneObject, 0x1C4);
        CSF_AT(uint32_t, self, 0x80) = CSF_AT(uint32_t, sceneObject, 0x1E0);
        CSF_AT(uint32_t, self, 0x84) = 0xFFFFFFFFu;

        // Virtual query on sceneObject+0x44, vtable slot 0xD8 / 4 = 54.
        void* queryObject = CSF_AT(void*, sceneObject, 0x44);
        int queryResult = ((int(__fastcall*)(void*, void*))CSF_AT(void**, queryObject, 0)[0xD8 / 4])(queryObject, nullptr);
        result = static_cast<char>(queryResult);

        if (queryResult != 0)
        {
            int handle = Handle_GetValuePointer(reinterpret_cast<void*>(static_cast<uintptr_t>(queryResult)), nullptr);
            result = static_cast<char>(handle);
            CSF_AT(uint32_t, self, 0x84) = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(handle)), 0);
        }
    }
    return result;
}

HOOK(0x004D7340, TAutonomo_CacheObjectPlacementOnce, "auto");
