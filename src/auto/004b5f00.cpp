#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, CoverAttack_CheckState_4ADAE0, 0x004ADAE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall AiSearch_UpdateCompletionStreak(
    void* self, void* /*edx*/, void* character)
{
    int linked = GetLinkedObjectValue_4DB5C0(character, nullptr);
    uint32_t baseAddress = linked ? static_cast<uint32_t>(linked) - 4 : 0;
    auto* base = reinterpret_cast<uint8_t*>(baseAddress);

    if (CSF_AT(int, base, 0x608) == -1)
    {
        CSF_AT(uint32_t, self, 0x18) = 0;
        return static_cast<char>(baseAddress);
    }

    linked = GetLinkedObjectValue_4DB5C0(character, nullptr);
    baseAddress = linked ? static_cast<uint32_t>(linked) - 4 : 0;
    base = reinterpret_cast<uint8_t*>(baseAddress);

    void* coverState = base + 0x5DC;
    void* selectedHandle = CSF_AT(int, base, 0x630) < 0
        ? static_cast<void*>(base + 0x610)
        : static_cast<void*>(base + 0x628);

    char stateResult = CoverAttack_CheckState_4ADAE0(
        coverState, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(selectedHandle)));

    if (!stateResult)
    {
        CSF_AT(uint32_t, self, 0x18) = 0;
        return stateResult;
    }

    linked = GetLinkedObjectValue_4DB5C0(character, nullptr);
    baseAddress = linked ? static_cast<uint32_t>(linked) - 4 : 0;
    base = reinterpret_cast<uint8_t*>(baseAddress);

    selectedHandle = CSF_AT(int, base, 0x630) < 0
        ? static_cast<void*>(base + 0x610)
        : static_cast<void*>(base + 0x628);

    int resolvedAddress = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(selectedHandle)));

    if (!resolvedAddress)
        return static_cast<char>(resolvedAddress);

    void* resolved = reinterpret_cast<void*>(static_cast<uint32_t>(resolvedAddress));
    uint32_t activeObject = CSF_AT(uint32_t, resolved, 0x50);
    if (activeObject)
        return static_cast<char>(activeObject);

    linked = GetLinkedObjectValue_4DB5C0(character, nullptr);
    baseAddress = linked ? static_cast<uint32_t>(linked) - 4 : 0;
    base = reinterpret_cast<uint8_t*>(baseAddress);

    void* owner = CSF_AT(void*, base, 0x2C);
    if (CSF_AT(uint32_t, resolved, 0x274) != CSF_AT(uint32_t, owner, 0x20))
        return static_cast<char>(reinterpret_cast<uintptr_t>(owner));

    uint32_t count = CSF_AT(uint32_t, self, 0x18);
    uint32_t requiredCount = CSF_AT(uint32_t, self, 0x14);
    ++count;
    CSF_AT(uint32_t, self, 0x18) = count;
    if (count >= requiredCount)
        CSF_AT(int, self, 0x04) = 3;

    return static_cast<char>(count);
}

HOOK(0x004B5F00, AiSearch_UpdateCompletionStreak, "auto");
