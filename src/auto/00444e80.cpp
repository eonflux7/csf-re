#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

static double __fastcall ColorAnim_ComputeValue(
    std::uint32_t* self, void* /*edx*/)
{
    void* state13 = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* state14 = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* state4 = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* state6 = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* state8 = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* state10 = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* state12 = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());

    // Unsigned-to-double casts replace the compiler's signed-load/bias idiom.
    return static_cast<double>(CSF_AT(std::uint32_t, self, 0x30))
               * CSF_AT(float, state12, 0x1D5C)
         + static_cast<double>(CSF_AT(std::uint32_t, self, 0x28))
               * CSF_AT(float, state10, 0x1D58)
         + static_cast<double>(CSF_AT(std::uint32_t, self, 0x20))
               * CSF_AT(float, state8, 0x1D54)
         + static_cast<double>(CSF_AT(std::uint32_t, self, 0x18))
               * CSF_AT(float, state6, 0x1D50)
         + static_cast<double>(CSF_AT(std::uint32_t, self, 0x10))
               * CSF_AT(float, state4, 0x1D4C)
         + static_cast<double>(CSF_AT(std::uint32_t, self, 0x38))
               * CSF_AT(float, state14, 0x1D44)
         + static_cast<double>(CSF_AT(std::uint32_t, self, 0x34))
               * CSF_AT(float, state13, 0x1D48);
}

HOOK(0x00444E80, ColorAnim_ComputeValue, "auto");
