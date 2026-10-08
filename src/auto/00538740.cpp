#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __cdecl, MenuLoading_AdjustFloatPair,
         0x0042F400, (float* pair));

// On x86, regparm(3) receives these first three arguments in EAX, EDX,
// and ECX respectively. stdcall pops only the three remaining stack
// arguments (12 bytes). This captures incoming EAX without inline assembly.
static int __stdcall __attribute__((regparm(3)))
CMenuLoading_ApplyEntryTransformsAndDecrementIndex(
    int incomingEax, void* /*edx*/, CMenuLoading* self,
    void* targetObject, int* entryIndex, int slotIndex)
{
    if (targetObject == nullptr)
        return incomingEax;

    uint32_t pairA[2] = {0, 0};
    uint32_t pairB[2] = {0, 0};

    auto* bytes = reinterpret_cast<uint8_t*>(self);
    uint32_t recordOffset = static_cast<uint32_t>(*entryIndex) << 5;

    pairA[0] = CSF_AT(uint32_t, bytes + recordOffset, 0x5C);
    pairA[1] = CSF_AT(uint32_t, bytes + recordOffset, 0x60);
    MenuLoading_AdjustFloatPair(reinterpret_cast<float*>(pairA));

    recordOffset = static_cast<uint32_t>(*entryIndex) << 5;
    pairB[0] = CSF_AT(uint32_t, bytes + recordOffset, 0x64);
    pairB[1] = CSF_AT(uint32_t, bytes + recordOffset, 0x68);
    MenuLoading_AdjustFloatPair(reinterpret_cast<float*>(pairB));

    ((void(__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, targetObject, 0)[9])(targetObject, nullptr, pairA);
    ((void(__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, targetObject, 0)[13])(targetObject, nullptr, pairB);
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, targetObject, 0)[19])(
            targetObject, nullptr,
            bytes + (static_cast<uint32_t>(slotIndex) << 5) + 0x6C);
    ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, targetObject, 0)[2])(targetObject, nullptr);

    // Reuse the same buffers: earlier virtual calls may retain their addresses.
    recordOffset = static_cast<uint32_t>(*entryIndex) << 5;
    pairA[0] = CSF_AT(uint32_t, bytes + recordOffset, 0xBC);
    pairA[1] = CSF_AT(uint32_t, bytes + recordOffset, 0xC0);
    MenuLoading_AdjustFloatPair(reinterpret_cast<float*>(pairA));

    recordOffset = static_cast<uint32_t>(*entryIndex) << 5;
    pairB[0] = CSF_AT(uint32_t, bytes + recordOffset, 0xC4);
    pairB[1] = CSF_AT(uint32_t, bytes + recordOffset, 0xC8);
    MenuLoading_AdjustFloatPair(reinterpret_cast<float*>(pairB));

    ((void(__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, targetObject, 0)[9])(targetObject, nullptr, pairA);
    ((void(__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, targetObject, 0)[13])(targetObject, nullptr, pairB);

    recordOffset = static_cast<uint32_t>(*entryIndex) << 5;
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, targetObject, 0)[19])(
            targetObject, nullptr, bytes + recordOffset + 0xCC);
    int result = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, targetObject, 0)[2])(targetObject, nullptr);

    *entryIndex = static_cast<int>(static_cast<uint32_t>(*entryIndex) - 1u);
    return result;
}

HOOK(0x00538740, CMenuLoading_ApplyEntryTransformsAndDecrementIndex, "auto");
