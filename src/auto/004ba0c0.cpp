#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiDistracted_GetBaseStateName, 0x004AA8C0, (void* self, void* edx, void* a2));
CSF_FUNC(C_008319c0*, __fastcall, S_004aef70_ResolvePendingTarget, 0x004AF100, (S_004aef70* self, void*));
CSF_FUNC(unsigned int, __fastcall, CoverAttack_Sub_4B9BA0, 0x004B9BA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_004D10F0_ResetFields, 0x004D10F0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(char*, __cdecl, AiDistracted_QueryLinkedStatusText, 0x007613E0, (int a1));

static const char* __fastcall TDudoso_ResetAndGetName(
    TDudoso* self, void* /*edx*/, TAiState* context)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(context, nullptr);
    std::uintptr_t objectAddress =
        linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;

    void* owner = CSF_AT(void*, reinterpret_cast<void*>(objectAddress), 0x2C);
    void* status = CSF_AT(void*, owner, 0x34);
    AiDistracted_QueryLinkedStatusText(CSF_AT(int, status, 0x2FC));

    linkedValue = GetLinkedObjectValue_4DB5C0(context, nullptr);
    objectAddress =
        linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;
    S_004aef70_ResolvePendingTarget(
        reinterpret_cast<S_004aef70*>(objectAddress), nullptr);

    linkedValue = GetLinkedObjectValue_4DB5C0(context, nullptr);
    objectAddress =
        linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;
    C_004D10F0_ResetFields(
        reinterpret_cast<void*>(objectAddress + 0x690), nullptr);

    linkedValue = GetLinkedObjectValue_4DB5C0(context, nullptr);
    objectAddress =
        linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;
    CoverAttack_Sub_4B9BA0(reinterpret_cast<void*>(objectAddress), nullptr);

    linkedValue = GetLinkedObjectValue_4DB5C0(context, nullptr);
    objectAddress =
        linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;
    CSF_AT(std::uint8_t, reinterpret_cast<void*>(objectAddress), 0x6CC) = 0;
    CSF_AT(std::uint8_t, self, 0x30) = 0;

    return AiDistracted_GetBaseStateName(self, nullptr, context);
}

HOOK(0x004BA0C0, TDudoso_ResetAndGetName, "auto");
