#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// VC7.1 _Ufill forwarding overload, cluster 137; binary stride 1.
using Value = uint8_t;
CSF_FUNC(uint32_t, __cdecl, FillHelper, 0x0051AE40, (uint32_t dest, uint32_t count, const Value* value, void* context, uint32_t tag));
struct Operations {
    using value_type = Value;
    static constexpr bool zero_tag = true;
    static __forceinline void fill(uint32_t dest, uint32_t count, const Value* value, void* context, uint32_t tag) {
        FillHelper(dest, count, value, context, tag);
    }
};
static uint32_t __fastcall StlUfill_0051af50(void* self, void*, uint32_t dest, uint32_t count, const Value* value) {
    return legacy::vc71::vector_ufill<Operations>(self, dest, count, value);
}
HOOK(0x0051AF50, StlUfill_0051af50, "stl");
