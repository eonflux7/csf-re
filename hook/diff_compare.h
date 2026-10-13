#pragma once
#include <cstdint>
#include <intrin.h>

namespace hook::diff_detail {

inline bool on_stack(uint32_t p) {
    return p >= __readfsdword(8) && p < __readfsdword(4);  // TEB StackLimit .. StackBase
}

inline bool same_frame_return(uint32_t o, uint32_t u, uint32_t args) {
    // Returned hidden-result pointers can refer to locals in the function's popped frame. The two versions
    // have different layouts; addresses in the argument block or the caller's frame still have to match.
    return on_stack(o) && on_stack(u) && o < args && u < args;
}

inline bool same_arg(uint32_t o, uint32_t u, uint8_t width = 4) {
    uint32_t mask = width == 1 ? 0xFFu : width == 2 ? 0xFFFFu : ~0u;
    // A pushed byte/short can leave unrelated register bits above the value. Only full words can be pointers.
    return (o & mask) == (u & mask) || (mask == ~0u && on_stack(o) && on_stack(u));
}

}  // namespace hook::diff_detail
