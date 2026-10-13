# Standalone allocation policy

**Retain the game's allocator; give it one UCRT /MT backing heap.** Do not
replace the existing game HOOK slots (that would create duplicate definitions).
`operators.cpp` supplies global C++ scalar/array new/delete, sized delete and
nothrow variants. No code in this directory is part of the hooked DLL.

| Address | Owner / role | Standalone path |
|---|---|---|
| `006910F0` | game, untagged operator-new entry | existing wrapper -> `00690DC0` |
| `00691110` | game, operator-delete entry | existing wrapper -> `00690E60` |
| `00691120` | game, tagged allocation | existing small/tag special-case routing, otherwise `00690EB0` |
| `00690DC0` | game, size/file/line debug allocation | existing mutex, tracking/recovery, pool dispatch |
| `00690E60` | game, debug free | existing pool-aware free and tracking |
| `00690EB0` | game, size/alignment/file/line allocation | existing aligned-allocation routing (inner implementation currently ignores alignment) |
| `00690F70` / `00690FC0` | game, alternate free / reallocate | existing pool-aware implementation |
| `0069DEF0` / `0069DFA0` | game, allocator inner alloc/free | pool or backing malloc/free |
| `0069E050` / `0069E260` | game, realloc / size | check pool membership before UCRT realloc/_msize |
| `007B1EC2`, `007B17CC`, `007B1ED4`, `007B2081`, `007B2FB8` | CRT, backing allocation/free/realloc/calloc/size | boundary/crt -> this process's /MT runtime |
| `00647070` / `006470E0` | NetZ, node alloc/free | shim ignores removed NetZ manager/header/debug metadata; size is argument 2, block is argument 2; game heap |
| `00647140` / `00647160` / `006471C0` | NetZ, plain/tagged new/delete | game heap |

The NetZ allocator originally prepended a release callback/header and optionally
called registered malloc/free callbacks. No original NetZ library executes in
standalone, so those headers/callbacks are deliberately not retained. All five
new NetZ allocation slots must be taken together. Owned NetZ String payloads
still have their **separate four-byte length prefix**, supplied by boundary/netz;
String destruction frees the prefix base, not the character pointer.

Ordinary throwing C++ new normalizes zero to one byte and throws bad_alloc on
failure; direct game allocators retain their own size-zero/failure contract.
GameAlloc and legacy containers already use the game entries. Do not forward
CRT malloc back through global new: the inner game allocator calls malloc and
that would recurse. No initialization-time allocations are introduced here.

## Open safety blockers

* This is a compile-checked routing shim, not a startup/heap execution test.
  Global new requires the game allocator/mutex to have been initialized before
  any dynamic C++ static initialization. The standalone entry/startup job must
  establish that ordering, or choose a fully coordinated alternative backend.
* Several existing callers mis-group the cdecl NetZ allocator arguments. For
  example `src/auto/006a7000.cpp` declares `006470E0` with **one** argument and
  calls it with the manager result; the binary/contract require three arguments
  (manager, block, kind). Its preceding get-manager call wrongly owns arguments
  that belong to free. The shim cannot recover a missing block argument.
  Fix the callers before running these paths. manifest.json records all caller
  declarations of the five shim addresses; this is NOT solved by a link pass.
* Over-aligned C++17 new/delete are not overridden; no original VC7.1 entry
  guarantees such allocations. If introduced, they need a matching explicit
  game-heap aligned allocation/free policy, not arbitrary pool-interior frees.
* Original `0069E020` forwards to the normal allocator and ignores alignment.
  This task does not claim to fix that pre-existing implementation/contract.
* Pool interiors can only be freed/reallocated/sized by the game pool-aware
  functions. The physical backing heap is shared; this does not erase logical
  ownership or make an interior pointer a valid CRT allocation.
