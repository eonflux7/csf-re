#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_FindProperty, 0x006932B0, (S_00692b80* self, void* /*edx*/, char* name));
CSF_FUNC(int, __fastcall, DataNode_ReadVector3, 0x00693020, (std::uint8_t* self, void* /*edx*/, float* output));

// Looks up a named property on this node. If found, reads it as a Vector3 into
// the output buffer. Otherwise zeroes the three float components of the output.
static int __fastcall DataNode_ReadVector3Property(S_00692b80* self, void* /*edx*/, char* propertyName, std::uint32_t outputAddress)
{
    S_00692b80* foundNode = S_00692b80_FindProperty(self, nullptr, propertyName);
    if (foundNode != nullptr)
    {
        return DataNode_ReadVector3(reinterpret_cast<std::uint8_t*>(foundNode), nullptr,
                                    reinterpret_cast<float*>(outputAddress));
    }

    *reinterpret_cast<std::uint32_t*>(outputAddress) = 0;
    *reinterpret_cast<std::uint32_t*>(outputAddress + 4) = 0;
    *reinterpret_cast<std::uint32_t*>(outputAddress + 8) = 0;
    return static_cast<int>(outputAddress);
}

HOOK(0x00693A60, DataNode_ReadVector3Property, "auto");
