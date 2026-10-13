#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, LengthPrefixedBuffer_TerminateAtLength, 0x00694C20, (void* self, void*));
CSF_FUNC(void**, __fastcall, LoadState_Construct, 0x00694C80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CoreZlibLog_BeginRecord, 0x006951B0, (void* self, void* edx, char a2, char a3, int a4));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndGroup_6951D0, 0x006951D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CoreZlibLog_BeginGroup, 0x00695470, (void* self, void* edx, char* a2, int a3, char a4));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));
CSF_FUNC(uint16_t*, __fastcall, ProcessRecordRange_6A2460, 0x006A2460, (void* self, void* edx, S_00694cd0* a1, int a2));
CSF_FUNC(void*, __fastcall, ResetEmbeddedMemberAt8, 0x00712D10, (void** self, void* edx));

CSF_GLOBAL(const char*, g_pszKey_PREFERENCES, 0x008A6B90);

static char __fastcall C_0086c2b4_SavePreferences(
    C_0086c2b4* self, void* /*edx*/, const char* filename, int recordRange)
{
    alignas(8) std::uint8_t stateBytes[0x918];
    S_00694cd0& state = *reinterpret_cast<S_00694cd0*>(stateBytes);

    LoadState_Construct(&state, nullptr);
    CoreZlibLog_BeginRecord(&state, nullptr, 0, 0, 0);
    CoreZlibLog_BeginGroup(
        &state, nullptr, const_cast<char*>(g_pszKey_PREFERENCES), 0, 0);
    ProcessRecordRange_6A2460(self, nullptr, &state, recordRange);
    CoreZlibLog_EndGroup_6951D0(&state, nullptr);
    CoreZlibLog_EndGroup_6951D0(&state, nullptr);

    void* file = CoreFiles_OpenSceneFile(
        filename, const_cast<char*>(CSF_STR(0x00840044, "w")), 0);

    if (!file)
    {
        ResetEmbeddedMemberAt8(reinterpret_cast<void**>(&state), nullptr);
        return 0;
    }

    const std::uint32_t length = CSF_AT(std::uint32_t, &state, 0x0C);
    void** fileVtable = CSF_AT(void**, file, 0);
    const int buffer = LengthPrefixedBuffer_TerminateAtLength(&state, nullptr);

    reinterpret_cast<void (__fastcall*)(void*, void*, int, std::uint32_t)>(
        fileVtable[0x0C / 4])(file, nullptr, buffer, length);

    CloseSceneFile(file);
    ResetEmbeddedMemberAt8(reinterpret_cast<void**>(&state), nullptr);
    return 1;
}

HOOK(0x0069DC30, C_0086c2b4_SavePreferences, "auto");
