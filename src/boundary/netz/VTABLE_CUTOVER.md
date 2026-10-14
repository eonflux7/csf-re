# WinMain vtable boundary cutover

The supplied report contains **128 NetZ-labelled trap addresses**, not 128
virtual methods. Walking source-installed tables against `db/vtables.json`
finds **23 distinct trap addresses actually present in reached tables**. All
23 are supplied here. Two additional direct-demand local accessors are supplied;
**103 addresses remain deliberately unresolved**. This is not a completed
WinMain dependency closure or a Windows startup result.

## Ordered work

1. **68B5F0 first:** exact `lea eax,[ecx+4]; ret`, not a loaded member or
   fabricated network object. Three blocking tables:

   | Table | Newly supplied targets (zero-based slots) | Previously supplied |
   |---|---|---|
   | 83FF18, Options settings | 68B5F0(8), 681D60(12), 681F40(18), 681F30(24), 681FB0(28) | 686C10(6) |
   | 861328, render base | 68B5F0(6), 681FB0(7), 676ED0(10), 7A8ED0(22,23), 7A7B60(25) | 6683F0(8), 64AF90(24) |
   | 861460, resource screen | 68B5F0(6), 681FB0(7), 676ED0(10), 7A8ED0(22,23) | 6683F0(8), 64AF90(24) |

   Eight new unique addresses; all eleven distinct NetZ targets supplied.
   Existing 64AF90 now preserves EAX exactly (`ret 8`) rather than imposing
   a zero result on an original no-op. No targets of these three are left.

2. Other immediate options/startup table: **83FED8 slot 12, 68ECA0** returns
   the original byte at self+10. One additional unique target. Shared entries
   in other settings/render tables reuse the address-keyed wrappers above.

3. Remaining reached tables/classes: **fourteen** additional virtual trap
   targets, plus two direct local accessors (**6472B0**, EDX:EAX pair;
   **681FC0**, member+34). The full per-table/slot inventory is
   [VTABLE_CUTOVER.json](VTABLE_CUTOVER.json), including installed-by functions
   and all 128 original paths/decisions. Its `vtables` section groups targets
   by original table, avoiding misleading duplicate shared-method counts.

## Boundary contracts and intentionally deferred work

`vtable_contracts.json` freezes independently inspected original instruction
evidence, cleanup and ownership. Tiny local accessors/constants/no-ops retain
original registers, AL-only returns, true constants, member identity and stack
cleanup. They are not converted into false network queries. No connection,
transport, request, callback or session is invented.

**668230** is an exact ECX-preserving tail dispatcher through self->vtable[2].
In reached table **867358 slot 3**, its destination is the genuine game renderer
**738870**, with proved ret0. The thunk does not invent ret0 or any other ret N:
the dispatched target owns cleanup. This differs from 64B110's unsafe lookup
and unknown class-result tail. Neither forwarding nor returning original true
constants represents online success.

The other **103** report entries are downstream direct dependencies, not
unfilled methods of source-installed tables. They remain visible traps because
constructors need embedded ownership/layout audits; registry/object results
need identity and consumer audits; serialization/output contracts cannot be
replaced by bare zero; rendering/native-resource helpers are shared operations,
not failed network queries; indirect lookup tails have unresolved ABI/lifetime
hazards. Every address and its reason is listed separately in the JSON inventory.
No claim of multiplayer-only reachability is made.

## Offline validation

`test_vtables.py`, executed by `test_offline.py` after `test_winmain.py`, compares
compiled instructions and register/flag/memory results against each inspected tiny
original contract, with eight nonzero/random/null object states. It checks stack
cleanup/nonvolatiles and dispatches through relocated original slots of the
three blocking tables plus 867358. The indirect dispatcher is tested with both
a controlled target and the actual compiled game renderer 738870's empty-buffer
branch (no RW calls), not an online library or successful network mock. The
existing actual game session/selection/initializer tests remain mandatory.

The tests run under Unicorn with heap/CRT/Win32 doubles. No Windows run, full
WinMain, populated network state or SP playability is established. Worker
command results and unresolved before/after counts belong in the worker report.
