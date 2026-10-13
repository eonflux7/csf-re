#pragma once
#include <cstdint>

// Diagnostic identity only. Unlike CSF_ADDR_FN/DATA/VT this must NOT be used
// as a callable/dereferenceable pointer into the new process.
#define CSF_ADDR_ORIGINAL(addr) (static_cast<uintptr_t>(addr))
namespace csf {
[[noreturn]] void unresolved(uintptr_t address);
int run_original_initializers();
}
