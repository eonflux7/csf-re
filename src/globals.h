// Address binding. Every reference to something of the original binary goes through these macros, keyed by its
// original address, so the same source builds both ways:
//
//   CSF_GLOBAL(COptions*, g_pOptions, 0x8BAF8C);                 // global variable
//   CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x6910F0, (size_t));     // call an original game function
//   self->vtbl = CSF_VTBL(0x82D410);                             // a vtable of the original
//   cb = CSF_ADDR_FN(0x506BD0);  CSF_ADDR_DATA(0x82EB98)  CSF_ADDR_VT(0x830030)  // an address as a value (uintptr_t)
//
// Hook phase: the macros yield the original addresses. Calling through CSF_FUNC always works: if the target is
// already reimplemented and hooked, the original entry JMPs to our version.
//
// Standalone (CSF_STANDALONE, cutover): the link is keyed by address, never by name, so local names and spellings
// (0x6B1380 / 0x006B1380) don't matter. HOOK(0xADDR, Fn) defines csf::fn_slot<ADDR>::p = &Fn; a CSF_FUNC reads it.
// csf::gv<ADDR> (globals) and csf::vt<ADDR> (vtables) are defined by generated data at cutover.
// tools/linktest.py compiles everything this way: undefined slots = the remaining work.
#pragma once
#include <cstddef>
#include <cstdint>

#ifdef CSF_STANDALONE
#pragma clang diagnostic ignored "-Wundefined-var-template"  // defined in another TU (or the remaining work)
namespace csf {
template <uintptr_t A> struct fn_slot { static void* const p; };   // defined by HOOK(A, ...)
template <uintptr_t A> struct gv { static unsigned char d[]; };     // original data at A (generated)
template <uintptr_t A> struct vt { static void* const t[]; };       // original vtable at A (generated)
}  // namespace csf
#endif

#ifndef CSF_STANDALONE
#define CSF_GLOBAL(type, name, addr) static auto& name = *reinterpret_cast<type*>(addr)
#define CSF_FUNC(ret, conv, name, addr, args) static ret(conv* const name) args = reinterpret_cast<ret(conv*) args>(addr)
// An original address used as a value (stored callback, vtable compare, type-id string, table base): uintptr_t
#define CSF_ADDR_FN(addr) (static_cast<uintptr_t>(addr))
#define CSF_ADDR_DATA(addr) (static_cast<uintptr_t>(addr))
#define CSF_ADDR_VT(addr) (static_cast<uintptr_t>(addr))
#else
#define CSF_GLOBAL(type, name, addr) static auto& name = *reinterpret_cast<type*>(::csf::gv<(addr)>::d)
#define CSF_FUNC(ret, conv, name, addr, args) \
    static ret(conv* const& name) args = *reinterpret_cast<ret(conv* const*) args>(&::csf::fn_slot<(addr)>::p)
#define CSF_ADDR_FN(addr) (reinterpret_cast<uintptr_t>(::csf::fn_slot<(addr)>::p))
#define CSF_ADDR_DATA(addr) (reinterpret_cast<uintptr_t>(::csf::gv<(addr)>::d))
#define CSF_ADDR_VT(addr) (reinterpret_cast<uintptr_t>(::csf::vt<(addr)>::t))
#endif
#define CSF_VTBL(addr) (reinterpret_cast<void**>(CSF_ADDR_VT(addr)))

// Access a field not (yet) declared in a struct: CSF_AT(float, self, 0x48) = 1.0f;
// p may be a const pointer or an address integer (the game has no const; the original reads through anything).
#define CSF_AT(T, p, off) \
    (*reinterpret_cast<T*>(const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(p)) + (off)))

// A string constant of the original binary. Hook phase: the original address (pointer identity can matter,
// e.g. names used as type ids). Standalone: the literal.
#ifndef CSF_STANDALONE
#define CSF_STR(addr, text) (reinterpret_cast<const char*>(addr))
#else
#define CSF_STR(addr, text) (text)
#endif
