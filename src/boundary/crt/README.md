# Standalone CRT boundary

`crt.cpp` binds the original CRT addresses to **x86 /MT UCRT + vcruntime**
from the xwin toolchain, not msvcr71.dll. It is excluded from the hooked DLL
and also guarded by `CSF_STANDALONE`. This is forwarding, not CRT replacement.

The address/function inventory is the `FORWARD` and `HOOK` list in crt.cpp.
There are 52 bindings (the original link-test demand is smaller because unused
CSF_FUNC declarations are not object-file references). Varargs wrappers use
va_list forwarding; `_snprintf` / `_vsnprintf` use the underscore variants,
not ISO snprintf, retaining the legacy truncation/termination contract.
This worktree's fresh baseline demands 51 CRT slots, not the task's cached 49:
the additional `_String_base::_Xran` / `_Xlen` entries throw modern runtime
out_of_range / length_error with the original messages. Old error-object layout
and old catch metadata must not be assumed compatible merely from type names.

Register helpers:

* `007AF86C` forwards naked to the toolchain `_ftol2`: ST0 input, EDX:EAX result.
* `007AF8F0`, `007AFA60`, `007AFFFA` forward naked to the UCRT's actual
  `_CIasin` / `_CIpow` / `_CIfmod` x87 helpers (libucrt.lib in the SDK's
  ucrt/x86 directory). The contract's stack-based prototypes for some
  of these are wrong for assembly callers. Standalone-only caller guards in
  the three affected auto files use the already available scalar math instead.
* `007AF226` forwards naked to `__CxxFrameHandler`, preserving EAX FuncInfo;
  `007AF666` forwards to `_CxxThrowException` (__stdcall, pop 8).
* Original `__alloca_probe`, `_ftol`, `__EH_prolog`, `__SEH_prolog` have no
  demanded address slot in the baseline. Recompiled clang-cl code calls its
  own compiler helper symbols, supplied by /MT libraries, not original
  prolog addresses. They must NOT be implemented as ordinary C++ functions.
* The same distinction applies to `@__security_check_cookie@4`: link the
  toolchain cookie/checker together; do not compare a modern cookie against
  an original-image global. Old naked thunks with original image addresses
  are a separate cutover backlog item.

## Compatibility limits (not execution-verified)

| Area | Difference / restriction |
|---|---|
| FILE | VC7.1's public FILE structure is not the UCRT's opaque stream. Only pointers obtained from this boundary's fopen can be used with its fclose/fflush/fprintf. Original `_iob` data and direct FILE field reads cannot be copied across. |
| locale / ctype | Uses the current UCRT locale (initially C); per-thread locale state and tables are not original data. Character inputs must satisfy the C API domain. |
| printf / scanf | UCRT invalid-parameter handling, `%n` policy, non-finite float spelling and wide-format extensions differ from VC7.1; no byte-identical format guarantee. |
| x87 / controlfp | Forwarded control words use the modern runtime's x87/MXCSR treatment and startup defaults. No attempt to recreate VC7.1 process startup FP initialization; audit/init explicitly later. |
| heap | malloc/calloc/realloc/free/_msize must all use this /MT instance. No msvcr71 heap handle or original live allocation can cross cutover. A game pool interior pointer must not be given directly to UCRT free/realloc/_msize. |
| exception | Original storage is 12 bytes, vptr/message/ownership dword; modern x86 std::exception is asserted to have the same extent, but uses a bool ownership flag and a different vtable/what fallback string. Padding is cleared on construction/copy. Size alone is NOT a complete ABI proof. |
| EH | The runtime exports the legacy handler and throw entry, but original FuncInfo/ThrowInfo have absolute addresses. Rebased/generated EH metadata and every retained naked EH thunk still need audit. Forwarding alone does not prove exceptions work. |
| errno / rand / threads | All state belongs to this UCRT instance, not old CRT globals. _beginthread produces modern CRT-owned thread state/OS handles. |

Normal undefined symbols such as `_memset`, `_malloc` and the security checker
should be supplied by the real CRT at the eventual exe link, not by recursive
replacement wrappers. The link-test does not link or inspect runtime libraries.
