#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, LookupProviderSlotIndex, 0x007686E0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, ApplyProviderSlotBlend, 0x004F8800, (void* self, void* edx, int arg0, int a2, int a4, int a5));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static char __stdcall ApplyClampedFactorToSelectedProvider(
    int /*a1*/, int /*a2*/, uint32_t* argRefs)
{
    int globalValue = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);
    if (provider == nullptr)
        return 0;

    int keyRecord = CSF_AT(int, reinterpret_cast<void*>(argRefs[0]), 4);
    int managerValue = GetValue8B0554();
    int slotKey = CSF_AT(int, reinterpret_cast<void*>(keyRecord), 4);
    void* slotOwner = CSF_AT(void*, reinterpret_cast<void*>(managerValue), 0x1F90);

    int slotIndex = LookupProviderSlotIndex(slotOwner, nullptr, slotKey);
    if (slotIndex == -1)
        return static_cast<char>(slotIndex);

    uint32_t firstHolder = CSF_AT(uint32_t, reinterpret_cast<void*>(argRefs[1]), 4);
    uint32_t secondHolder = CSF_AT(uint32_t, reinterpret_cast<void*>(argRefs[2]), 4);
    uint32_t blendHolder = CSF_AT(uint32_t, reinterpret_cast<void*>(argRefs[3]), 4);
    float blendFactor = CSF_AT(float, reinterpret_cast<void*>(blendHolder), 4);

    if (blendFactor < g_fZero)
    {
        blendFactor = 0.0f;
    }
    else if (blendFactor > static_cast<volatile float&>(g_TransitionOne)
          || blendFactor >= static_cast<volatile float&>(g_TransitionOne))
    {
        blendFactor = 0.99000001f;
    }

    int firstAddress = static_cast<int>(firstHolder + 4);
    int secondAddress = static_cast<int>(secondHolder + 4);
    int32_t blendBits;
    std::memcpy(&blendBits, &blendFactor, sizeof(blendBits));

    return ApplyProviderSlotBlend(
        provider, nullptr, slotIndex, firstAddress, secondAddress, blendBits);
}

HOOK(0x004606C0, ApplyClampedFactorToSelectedProvider, "auto");
