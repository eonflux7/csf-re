#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(char, __fastcall, ObjectProvider_Commit, 0x004F53D0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall C_00861120_SetPendingRequest(
    C_00861120* self, void* /*edx*/, int requestAddress)
{
    void* request = reinterpret_cast<void*>(requestAddress);
    const std::uint32_t requestId = CSF_AT(std::uint32_t, request, 0x04);

    if (CSF_AT(std::uint32_t, self, 0x78) != requestId)
    {
        CSF_AT(std::uint32_t, self, 0x78) = requestId;
        CSF_AT(std::uint32_t, self, 0x7C) =
            CSF_AT(std::uint32_t, request, 0x08);
        ObjectProvider_Commit(self, nullptr);
    }

    auto* pendingHandle = reinterpret_cast<std::uint8_t*>(self) + 0x74;
    CSF_AT(std::uint32_t, self, 0x78) =
        CSF_AT(std::uint32_t, request, 0x04);
    CSF_AT(std::uint32_t, self, 0x7C) =
        CSF_AT(std::uint32_t, request, 0x08);

    if (CSF_AT(std::int32_t, self, 0x7C) < 0)
        return static_cast<char>(reinterpret_cast<std::uintptr_t>(pendingHandle));

    alignas(8) std::uint8_t temporary[0x08] = {};

    void* resolvedObject = reinterpret_cast<void*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(pendingHandle)));

    auto* resolvedHandle =
        reinterpret_cast<std::uint32_t* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, resolvedObject, 0)[0x38 / 4])(
                resolvedObject, nullptr, temporary);

    CSF_AT(std::uint32_t, self, 0x9C) = resolvedHandle[0];
    const std::uint32_t resolvedIndex = resolvedHandle[1];
    CSF_AT(std::uint32_t, temporary, 0x00) = 0;
    CSF_AT(std::uint32_t, temporary, 0x04) = 0;
    CSF_AT(std::uint32_t, self, 0xA0) = resolvedIndex;
    CSF_AT(std::uint32_t, self, 0x8C) = 0;
    CSF_AT(std::uint32_t, self, 0x90) = 0;
    CSF_AT(std::uint32_t, self, 0x94) = 0;
    CSF_AT(std::uint32_t, self, 0x98) = 0;

    const int provider = GetGlobalValue_4FBDE0();
    const int firstEntry = GetFirstElement(reinterpret_cast<void*>(provider), nullptr);
    void* lookupObject = reinterpret_cast<void*>(
        ResolveEntryLookupValue(reinterpret_cast<void*>(firstEntry), nullptr));

    CSF_AT(std::uint32_t*, temporary, 0x00) = RwMatrixCreate();
    CSF_AT(std::uint8_t, temporary, 0x04) = 1;

    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, lookupObject, 0)[0x54 / 4])(
            lookupObject, nullptr, temporary);

    const char ownsMatrix = static_cast<char>(
        CSF_AT(std::uint8_t, temporary, 0x04));
    if (ownsMatrix)
        return static_cast<char>(
            RwMatrixDestroy(CSF_AT(std::uint32_t*, temporary, 0x00)));

    return ownsMatrix;
}

HOOK(0x004F5EF0, C_00861120_SetPendingRequest, "auto");
