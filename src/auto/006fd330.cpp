#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, CharacterModel_InitializeTrace, 0x00401060, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, MovilAPunto_AssignPointAndTag, 0x004A9F00, (void* self, void* , int tag, const int* point));
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

static std::uint32_t* __fastcall CLightObject_GetWorldMatrixPointer(
    CLightObject* self, void* /*edx*/)
{
    const auto lightType = self->vSlot18();
    if (lightType == 0 || lightType == 1)
        return nullptr;

    if (CSF_AT(std::uint8_t, self, 0x34))
        return reinterpret_cast<std::uint32_t*>(
            CSF_AT(std::uint32_t, self, 0x38));

    void* light = reinterpret_cast<void*>(
        CSF_AT(std::uint32_t, self, 0x0C));
    CSF_AT(std::uint8_t, self, 0x34) = 1;

    const std::uint32_t frame = CSF_AT(std::uint32_t, light, 0x04);
    if (!frame)
        return reinterpret_cast<std::uint32_t*>(
            CSF_AT(std::uint32_t, self, 0x38));

    std::uint32_t* matrix = RwFrameGetLTM(frame);
    light = reinterpret_cast<void*>(
        CSF_AT(std::uint32_t, self, 0x0C));
    void* cachedPoint = reinterpret_cast<void*>(
        CSF_AT(std::uint32_t, self, 0x38));
    const int tag = CSF_AT(int, light, 0x14);
    const auto* position = reinterpret_cast<const std::uint8_t*>(matrix) + 0x30;

    alignas(8) std::uint8_t pointBytes[0x10];
    CSF_AT(int, pointBytes, 0x0C) = tag;

    if (cachedPoint)
    {
        CSF_AT(int, pointBytes, 0x00) = CSF_AT(int, position, 0x00);
        CSF_AT(int, pointBytes, 0x04) = CSF_AT(int, position, 0x04);
        CSF_AT(int, pointBytes, 0x08) = CSF_AT(int, position, 0x08);
        MovilAPunto_AssignPointAndTag(
            cachedPoint, nullptr, tag, reinterpret_cast<const int*>(pointBytes));
        return reinterpret_cast<std::uint32_t*>(
            CSF_AT(std::uint32_t, self, 0x38));
    }

    const int allocation = CoreMemory_AllocUntagged(16);
    if (!allocation)
    {
        CSF_AT(std::uint32_t, self, 0x38) = 0;
        return nullptr;
    }

    CSF_AT(int, pointBytes, 0x00) = CSF_AT(int, position, 0x00);
    CSF_AT(int, pointBytes, 0x04) = CSF_AT(int, position, 0x04);
    CSF_AT(int, pointBytes, 0x08) = CSF_AT(int, position, 0x08);

    std::uint32_t* result = CharacterModel_InitializeTrace(
        reinterpret_cast<void*>(static_cast<std::uint32_t>(allocation)),
        nullptr, tag, reinterpret_cast<std::uint32_t*>(pointBytes));
    CSF_AT(std::uint32_t, self, 0x38) =
        reinterpret_cast<std::uint32_t>(result);
    return result;
}

HOOK(0x006FD330, CLightObject_GetWorldMatrixPointer, "auto");
