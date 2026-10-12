#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, GetLinkedPositionWeight, 0x004AD850, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_ApplyLinkedPosition, 0x004BAD20, (void* self, void* edx, int a2, char a3, uint32_t* a4, int a5, char a6));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __stdcall AiDistracted_ApplyLinkedPosition(C_0085e7b8* object)
{
    auto* linkedFields = reinterpret_cast<void*>(GetLinkedObjectField10(object, nullptr));
    ++CSF_AT(uint32_t, linkedFields, 0x1C);

    uint32_t linked = static_cast<uint32_t>(GetLinkedObjectValue_4DB5C0(object, nullptr));
    uint32_t base = linked ? linked - 4 : 0;
    UnnamedApply_4DC180(object, nullptr, static_cast<int>(base + 432));

    linked = static_cast<uint32_t>(GetLinkedObjectValue_4DB5C0(object, nullptr));
    uint32_t weightBase = linked ? linked - 4 : 0;

    linked = static_cast<uint32_t>(GetLinkedObjectValue_4DB5C0(object, nullptr));
    uint32_t positionBase = linked ? linked - 4 : 0;

    float weight = static_cast<float>(
        GetLinkedPositionWeight(reinterpret_cast<void*>(weightBase + 1540), nullptr));
    int weightBits;
    std::memcpy(&weightBits, &weight, sizeof(weightBits));

    auto* position = reinterpret_cast<uint32_t*>(
        static_cast<uint32_t>(GetLinkedObjectField10(object, nullptr)) + 16);
    void* provider = GetCachedObjectProvider(object, nullptr);

    return AiCore_ApplyLinkedPosition(
        reinterpret_cast<void*>(positionBase + 432), nullptr,
        reinterpret_cast<int>(provider), 1, position, weightBits, 0);
}

HOOK(0x004BBA50, AiDistracted_ApplyLinkedPosition, "auto");
