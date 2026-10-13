#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector helper; survey cluster 921, stride 32.
using Value = legacy::vc71::element<32>;
CSF_FUNC(void, __fastcall, Operation0, 0x004109E0, (void* self, void* edx, const void* value, uint32_t pos, uint32_t count));
struct Operations {
    using value_type = Value;
    static __forceinline void assign(Value* dest, const Value* source) {
        using legacy::vc71::address;
        using legacy::vc71::pointer;
        *reinterpret_cast<uint32_t*>(dest) = *reinterpret_cast<const uint32_t*>(source);
        Operation0(pointer<void>(address(dest) + 4), nullptr, pointer<void>(address(source) + 4), 0, 0xFFFFFFFFu);
    }
};
static uint32_t __cdecl Replica_0069ba20(uint32_t begin, uint32_t end, uint32_t value) {
    return legacy::vc71::vector_fill_assign<Operations>(begin, end, value);
}

HOOK(0x0069BA20, Replica_0069ba20, "stl");
