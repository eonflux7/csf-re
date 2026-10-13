#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// VC7.1 _Ufill forwarding overload, cluster 120; binary stride 16.
using Value = legacy::vc71::element<16>;
CSF_FUNC(uint32_t, __cdecl, FillHelper, 0x00731210, (uint32_t dest, uint32_t count, const Value* value, void* context, uint32_t tag));
struct Operations {
    using value_type = Value;
    static constexpr bool zero_tag = false;
    static __forceinline void fill(uint32_t dest, uint32_t count, const Value* value, void* context, uint32_t tag) {
        FillHelper(dest, count, value, context, tag);
    }
};
static uint32_t __fastcall StlUfill_00731390(void* self, void*, uint32_t dest, uint32_t count, const Value* value) {
    return legacy::vc71::vector_ufill<Operations>(self, dest, count, value);
}
HOOK(0x00731390, StlUfill_00731390, "stl");
