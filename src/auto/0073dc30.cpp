#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, NetSession_ConstructString, 0x0066F130, (void* self, void* edx, char* a2));
CSF_FUNC(void, __fastcall, NetSession_DestroyString, 0x0066F150, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0085839c_StartSession, 0x0073D350, (void* self, void* edx, void** a2));
CSF_FUNC(char, __cdecl, NetSession_InitializeRuntime, 0x007A0150, ());

static void __fastcall C_0085839c_StartSessionByIndex(
    C_0085839c* self, void* /*edx*/, uint32_t entryIndex)
{
    const uint32_t state = CSF_AT(uint32_t, self, 0x98);
    if (state == 6 || state == 0)
        return;

    if (!CSF_AT(uint8_t, self, 0x14F)) {
        NetSession_InitializeRuntime();
        CSF_AT(uint8_t, self, 0x14F) = 1;
    }

    void* sentinel = CSF_AT(void*, self, 0xA0);
    void* entry = CSF_AT(void*, sentinel, 0);
    if (entry == sentinel)
        return;

    uint32_t index = 0;
    while (index != entryIndex) {
        entry = CSF_AT(void*, entry, 0);
        sentinel = CSF_AT(void*, self, 0xA0);
        ++index;
        if (entry == sentinel)
            return;
    }

    const int collection = SceneLoader_GetWorldObjectCollection(
        reinterpret_cast<uint8_t*>(entry) + 8, nullptr);

    void* sessionName = nullptr;
    NetSession_ConstructString(
        &sessionName, nullptr,
        reinterpret_cast<char*>(static_cast<uintptr_t>(collection)));

    if (!CSF_AT(uint8_t, self, 0x14F)) {
        NetSession_InitializeRuntime();
        CSF_AT(uint8_t, self, 0x14F) = 1;
    }

    C_0085839c_StartSession(self, nullptr, &sessionName);
    CSF_AT(uint8_t, self, 0x104) = 0;
    NetSession_DestroyString(&sessionName, nullptr);
}

HOOK(0x0073DC30, C_0085839c_StartSessionByIndex, "auto");
