#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, CallbackPredicates_EvaluatePrimaryPredicate,
         0x004725F0, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, CallbackPredicates_EvaluateSecondaryPredicate,
         0x00471550, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, CallbackPredicates_EvaluateTertiaryPredicate,
         0x004715F0, (void* self, void* edx, int a2));

[[maybe_unused]] static const auto kPredicateVtable = CSF_VTBL(0x004725F0);

static bool __fastcall CallbackPredicates_EvaluateCallbackPredicate(
    std::uint8_t* self, void* /*edx*/, S_004725f0* operand, int callback)
{
    const std::uint8_t* callbackTypePointer =
        CSF_AT(std::uint8_t*, reinterpret_cast<void*>(callback), 0x04);
    const std::uint8_t callbackType = *callbackTypePointer;
    const std::uint8_t* operandTypePointer =
        reinterpret_cast<const std::uint8_t*>(operand->field_0004);

    if (callbackType != *operandTypePointer && callbackType != 3)
        return false;

    switch (CSF_AT(std::int8_t, self, 0x01))
    {
    case 0:
        return CallbackPredicates_EvaluatePrimaryPredicate(
            operand, nullptr, callback);
    case 1:
        return !CallbackPredicates_EvaluatePrimaryPredicate(
            operand, nullptr, callback);
    case 2:
        return CallbackPredicates_EvaluateSecondaryPredicate(
            operand, nullptr, callback);
    case 3:
        return !CallbackPredicates_EvaluateTertiaryPredicate(
            operand, nullptr, callback);
    case 4:
        return CallbackPredicates_EvaluateTertiaryPredicate(
            operand, nullptr, callback);
    case 5:
        return !CallbackPredicates_EvaluateSecondaryPredicate(
            operand, nullptr, callback);
    default:
        return false;
    }
}

HOOK(0x00472820, CallbackPredicates_EvaluateCallbackPredicate, "auto");
