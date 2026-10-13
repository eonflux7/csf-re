#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector helper; survey cluster 715, stride 20.
using Value = legacy::vc71::element<20>;
CSF_FUNC(void, __fastcall, Operation0, 0x00559520, (void* self, void* edx, const void* value));
struct Operations {
    using value_type = Value;
    static __forceinline void assign(Value* dest, const Value* source) {
        using legacy::vc71::address;
        using legacy::vc71::pointer;
        *reinterpret_cast<uint32_t*>(dest) = *reinterpret_cast<const uint32_t*>(source);
        Operation0(pointer<void>(address(dest) + 4), nullptr, pointer<void>(address(source) + 4));
    }
};
static uint32_t __cdecl Replica_005597d0(uint32_t begin, uint32_t end, uint32_t value) {
    return legacy::vc71::vector_fill_assign<Operations>(begin, end, value);
}

HOOK(0x005597D0, Replica_005597d0, "stl");
