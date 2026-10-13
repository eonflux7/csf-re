#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// VC7.1 _Ufill forwarding overload, cluster 66; binary stride 200.
using Value = legacy::vc71::element<200>;
CSF_FUNC(uint32_t, __cdecl, FillHelper, 0x00512E90, (uint32_t dest, uint32_t count, const Value* value, void* context, uint32_t tag));
struct Operations {
    using value_type = Value;
    static constexpr bool zero_tag = false;
    static __forceinline void fill(uint32_t dest, uint32_t count, const Value* value, void* context, uint32_t tag) {
        FillHelper(dest, count, value, context, tag);
    }
};
static uint32_t __fastcall StlUfill_00513680(void* self, void*, uint32_t dest, uint32_t count, const Value* value) {
    return legacy::vc71::vector_ufill<Operations>(self, dest, count, value);
}
HOOK(0x00513680, StlUfill_00513680, "stl");
