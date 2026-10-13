#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector helper; survey cluster 1093, stride 48.
using Value = legacy::vc71::element<48>;
struct Operations {
    using value_type = Value;
    static __forceinline void destroy(Value* dest) {
        using Destructor = void (__thiscall*)(Value*, uint32_t);
        auto* table = *reinterpret_cast<Destructor**>(dest);
        table[0](dest, 0);
    }
};
static void __stdcall Replica_00453230(Value* begin, Value* end) {
    legacy::vc71::vector_destroy_range<Operations>(begin, end);
}

HOOK(0x00453230, Replica_00453230, "stl");
