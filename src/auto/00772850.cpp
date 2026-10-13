#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector helper; survey cluster 622, stride 44.
using Value = legacy::vc71::element<44>;
CSF_FUNC(void, __fastcall, Operation0, 0x004109E0, (void* self, void* edx, const void* value, uint32_t pos, uint32_t count));
CSF_FUNC(void, __fastcall, Operation1, 0x00772500, (void* self, void* edx, const void* value));
struct Operations {
    using value_type = Value;
    static __forceinline void assign(Value* dest, const Value* source) {
        using legacy::vc71::address;
        using legacy::vc71::pointer;
        Operation0(pointer<void>(address(dest) + 0), nullptr, pointer<void>(address(source) + 0), 0, 0xFFFFFFFFu);
        Operation1(pointer<void>(address(dest) + 28), nullptr, pointer<void>(address(source) + 28));
    }
};
static uint32_t __cdecl Replica_00772850(uint32_t begin, uint32_t end, uint32_t value) {
    return legacy::vc71::vector_fill_assign<Operations>(begin, end, value);
}

HOOK(0x00772850, Replica_00772850, "stl");
