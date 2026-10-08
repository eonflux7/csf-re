#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callees bound by address.
CSF_FUNC(void*, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuLoading_LayoutCenteredFrameEdges, 0x005385D0, (CMenuLoading* self, void* edx));
CSF_FUNC(char, __fastcall, CMenuLoading_ProcessEnabledLoadingSections, 0x00538870, (CMenuLoading* self, void* edx));

// Data referenced.
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Virtual dispatch helpers. SLOT = byte offset / 4. Object types are not declared here, so they are void*.
static void CallVirtualVoid(void* object, int slotIndex)
{
    reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, object, 0)[slotIndex])(object, nullptr);
}

static void* CallVirtualPtr(void* object, int slotIndex)
{
    return reinterpret_cast<void*(__fastcall*)(void*, void*)>(CSF_AT(void**, object, 0)[slotIndex])(object, nullptr);
}

static int CallVirtualInt(void* object, int slotIndex)
{
    return reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, object, 0)[slotIndex])(object, nullptr);
}

static bool CallVirtualBool(void* object, int slotIndex)
{
    return reinterpret_cast<bool(__fastcall*)(void*, void*)>(CSF_AT(void**, object, 0)[slotIndex])(object, nullptr);
}

// One pointer argument pushed on the stack (callee pops it, ret 4).
static void CallVirtualVoidWithPtrArg(void* object, int slotIndex, void* argument)
{
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(CSF_AT(void**, object, 0)[slotIndex])(object, nullptr, argument);
}

// One int argument pushed on the stack (callee pops it, ret 4).
static bool CallVirtualBoolWithIntArg(void* object, int slotIndex, int argument)
{
    return reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(CSF_AT(void**, object, 0)[slotIndex])(object, nullptr, argument);
}

// Double -> int64 truncation with __ftol2 semantics: NaN or out-of-range yields INT64_MIN.
static int64_t ConvertDoubleToInt64Truncated(double value)
{
    static constexpr double kInt64LowerInclusive = -9223372036854775808.0;
    static constexpr double kInt64UpperExclusive = 9223372036854775808.0;
    if (!(value >= kInt64LowerInclusive && value < kInt64UpperExclusive)) {
        return INT64_MIN;
    }
    return static_cast<int64_t>(value);
}

static int __fastcall CMenuLoading_AdvanceLoadingProgress(CMenuLoading* self, void* /*edx*/, float elapsedDelta)
{
    // Original order: read field_003c, store the accumulated elapsed time into field_0034, then branch.
    const float updatedElapsedSeconds = elapsedDelta + self->field_0034;
    const uint32_t initialProgressObject = self->field_003c;
    self->field_0034 = updatedElapsedSeconds;

    if (initialProgressObject != 0) {
        void* loadingManagerChild = MenuControls_ResolveObject(GetGlobalInt(), nullptr);
        if (CallVirtualBool(loadingManagerChild, 27)) {
            CallVirtualVoid(reinterpret_cast<void*>(self->field_003c), 31);
            void* managerChildAfter = MenuControls_ResolveObject(GetGlobalInt(), nullptr);
            CallVirtualVoid(managerChildAfter, 28);
        }
    }

    void* managerChildForLoad = MenuControls_ResolveObject(GetGlobalInt(), nullptr);
    bool isLoadingActive = CallVirtualBoolWithIntArg(managerChildForLoad, 29, 0);

    if (isLoadingActive) {
        if (self->field_0040 != 0 && CallVirtualPtr(reinterpret_cast<void*>(self->field_0040), 55) != nullptr) {
            // Unrounded x87 quotient (double is exact enough here: the comparison and the float rounding agree).
            const double unroundedFillRatio =
                static_cast<double>(self->field_0034) / static_cast<double>(self->field_0048);
            float fillRatio = static_cast<float>(unroundedFillRatio);
            if (unroundedFillRatio > static_cast<double>(g_TransitionOne)) {
                fillRatio = 1.0f;
            }

            void* fillRectTarget = CallVirtualPtr(reinterpret_cast<void*>(self->field_0040), 55);
            float fillRect[4] = {0.0f, 0.0f, fillRatio, 1.0f};
            CallVirtualVoidWithPtrArg(fillRectTarget, 19, fillRect);

            void* pairSourceOwner = CallVirtualPtr(reinterpret_cast<void*>(self->field_0040), 55);
            const uint32_t* sourcePair = static_cast<const uint32_t*>(CallVirtualPtr(pairSourceOwner, 15));

            // The original reads sourcePair[0] even though its value is overwritten; the volatile
            // local keeps that read from being optimized away.
            [[maybe_unused]] volatile uint32_t sourceLowDwordRead = sourcePair[0];

            // float * float is exact in double (and in x87 extended), so the product is the exact value.
            const double scaledWidth = static_cast<double>(fillRatio) * static_cast<double>(self->field_0044);
            const uint32_t sourceHighDword = sourcePair[1];

            uint32_t scaledPair[2];
            // Low dword comes from the converted value; high dword is the source's second dword.
            scaledPair[0] = static_cast<uint32_t>(ConvertDoubleToInt64Truncated(scaledWidth));
            scaledPair[1] = sourceHighDword;

            void* scaledPairTarget = CallVirtualPtr(reinterpret_cast<void*>(self->field_0040), 55);
            CallVirtualVoidWithPtrArg(scaledPairTarget, 14, scaledPair);
            CallVirtualVoid(reinterpret_cast<void*>(self->field_0040), 77);
        }

        if (self->field_002c != 0) {
            CallVirtualVoid(reinterpret_cast<void*>(self->field_002c), 77);
        }

        CallVirtualVoid(self, 24);
        CMenuLoading_ProcessEnabledLoadingSections(self, nullptr);
        CMenuLoading_LayoutCenteredFrameEdges(self, nullptr);

        void* childManagerForFinish = MenuControls_ResolveObject(GetGlobalInt(), nullptr);
        CallVirtualVoid(childManagerForFinish, 30);
    }

    void* loadingPresenter = LookupObjectById(GetGlobalInt(), nullptr);
    return CallVirtualInt(loadingPresenter, 11);
}

HOOK(0x00538A50, CMenuLoading_AdvanceLoadingProgress, "auto");
