#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Temporary-string helpers (MSVC __fastcall, the stack object is the "this").
CSF_FUNC(void*, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));

// Field-dispatch callees.
CSF_FUNC(char, __stdcall, NetCogible_BaseDispatchField, 0x0065E6F0, (int a1, int a2, int a3, int a4));
CSF_FUNC(char, __fastcall, NetSpawnMessages_FormatFieldValue, 0x007512B0, (void* self, void* , void* nameOwner, void** output));

static int __stdcall NetProyectil_DispatchFieldByName(int a1, int a2, int a3, int a4)
{
    // Temporary string object lives on the stack (4 bytes in the original frame).
    alignas(4) uint8_t tempStringBytes[4];

    void* nameString = InitializeTemporaryString(tempStringBytes, nullptr,
                                                 reinterpret_cast<std::int32_t>(CSF_STR(0x00867F00, "m_dsProyectilDescription")));
    char nameMatches = NameMatchesString(reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2)) + 8, nullptr,
                                         reinterpret_cast<void**>(nameString));
    DestroyTemporaryString(tempStringBytes, nullptr);

    if (nameMatches)
    {
        void* self = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a1)) + 0x44;
        return NetSpawnMessages_FormatFieldValue(self, nullptr,
                                                 reinterpret_cast<void*>(static_cast<uintptr_t>(a3)),
                                                 reinterpret_cast<void**>(static_cast<uintptr_t>(a4)));
    }
    return NetCogible_BaseDispatchField(a1, a2, a3, a4);
}

HOOK(0x747500, NetProyectil_DispatchFieldByName, "auto");
