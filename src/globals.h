// Address binding for the hook phase. At cutover (CSF_STANDALONE) these become real definitions/declarations.
//
//   CSF_GLOBAL(COptions*, g_pOptions, 0x8BAF8C);                 // global variable
//   CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x6910F0, (size_t));     // call an original game function
//
// Calling through CSF_FUNC always works in the hook phase: if the target is already reimplemented and hooked,
// the original entry JMPs to our version.
#pragma once
#include <cstddef>
#include <cstdint>

#ifndef CSF_STANDALONE
#define CSF_GLOBAL(type, name, addr) static auto& name = *reinterpret_cast<type*>(addr)
#define CSF_FUNC(ret, conv, name, addr, args) static ret(conv* const name) args = reinterpret_cast<ret(conv*) args>(addr)
#else
#define CSF_GLOBAL(type, name, addr) extern type name
#define CSF_FUNC(ret, conv, name, addr, args) ret conv name args
#endif
