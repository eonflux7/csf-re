# VC7.1 STL binary survey — first slice

Derived from db/stl.json, read-only export/funcs + disasm, and the original image. No library source was consulted. Cluster IDs identify normalized instruction shapes, not unique template members. `null` is not a cluster: each such address is listed separately below. Names below describe observed algorithms; exact original spelling is inferred.

| Kind | Instances | Bytes |
|---|---:|---:|
| list | 22 | 4009 |
| string | 18 | 4681 |
| tree | 38 | 8955 |
| vector | 337 | 103959 |
| vector<bool> | 3 | 441 |
| Total | 418 | 122045 |

## Mechanical extraction contract

Use exact code heads from export/disasm (including EH chunks), decoding bytes from CommXPC.exe with Capstone. Recover container offsets from loads/stores relative to entry ECX, iterator stride from loop increments/decrements and pointer differences. Power-of-two element sizes are arithmetic right shifts; other sizes use signed division or reciprocal-multiply sequences. Allocation size multiplies element count by the same stride. Cross-check all three; do not infer size from a misleading generated class name or pseudocode pointer type. Recover call conventions from ret N, caller cleanup and register use, not Hex-Rays phantom arguments.

Recover copy construction from the initial alias-protection temporary and the uninitialized_copy/fill loop: destination ECX or first stack argument plus source, sometimes a null-destination test. Recover assignment independently from fill/copy_backward: it may preserve a vptr and may call an owning string/record assignment. Recover destruction from _Destroy loops, direct calls, embedded string teardown, virtual calls and EH funclets. A missing direct destructor can be trivial or hand-inlined; inspect every instance. Type names need callers, field accesses and vtables; byte size alone is insufficient. The replica can use opaque aligned bytes plus operation traits until semantic names are established.

Vector: allocator word +0, first +4, last +8, end +12, size 16. Capacity growth is old_capacity + floor(old_capacity/2), with uint32 overflow check; max_size is UINT32_MAX (not UINT32_MAX/sizeof(T)). _Insert_n copies the argument before checking count, preserving aliases. Tree: left/parent/right at +0/+4/+8; value starts at +12, color/isnil AFTER the value, varying per instance (e.g. +24/+25, +44/+45). Tree container head +4 and size +8. List nodes: next/prev +0/+4, value +8 (12-byte pointer nodes here); container sentinel +4 and count +8. Strings: allocator +0, 16-byte buffer/pointer union +4, length +20, capacity +24; char SSO capacity 15, wchar16 capacity 7. vector<bool> uses bit count +0 and a word-vector starting +4; it cannot use vector<T> operations.

Allocator 0x690DC0 is cdecl(size,file,line), called with file=line=0 here; 0x690E60 frees those buffers. Element-owned storage sometimes uses 0x6910F0/0x691110 instead. They are different boundaries. Preserve call boundaries initially so emudiff can compare events, and preserve aliasing, call order, count-zero behavior, signed pointer differences and EH cleanup.

## Per-kind / cluster table

| Kind | Cluster | Count | Bytes | Observed member / operation | Addresses |
|---|---:|---:|---:|---|---|
| list | 134 | 6 | 804 | _Buynode (12 B, pointer value) | 00552e60, 006d07c0, 0073ceb0, 0073fb10, 007412d0, 00782220 |
| list | 186 | 2 | 622 | MISCLASSIFIED game network enumeration with inline push_back | 0073f3b0, 0073f4f0 |
| list | 460 | 2 | 244 | MISCLASSIFIED game message insertion with inline push_back | 0073ff40, 00740790 |
| list | 693 | 3 | 159 | push_back (pointer) | 00556880, 0073ff00, 00741de0 |
| string | 233 | 2 | 496 | assign(string,pos,count), char | 00403b60, 004109e0 |
| string | 242 | 2 | 482 | assign(ptr,count), char | 00403c60, 00429b60 |
| tree | 16 | 5 | 3445 | erase(iterator): unlink + RB fixup + free | 0054b640, 006b7670, 00735130, 0073a0a0, 0075f1e0 |
| tree | 17 | 8 | 3320 | _Insert: allocate/link + RB fixup | 00440280, 00519d20, 0054b4a0, 0069a990, 006b7ac0, 00735760, 0073a360, 0075f560 |
| tree | 89 | 12 | 1104 | _Lrotate / _Rrotate (mixed directions in one shape) | 0043e0d0, 0043e1f0, 0050e530, 0050e5b0, 0054b020, 0054b080, 0069a650, 0069a6b0, 00739910, 00739970, 0075ed60, 0075ef00 |
| tree | 609 | 6 | 180 | _Max | 0043e090, 0054af00, 0069a610, 00732ce0, 007394c0, 0075ed20 |
| tree | 629 | 6 | 174 | _Min | 0043e0b0, 0050e590, 0054af20, 0069a630, 007394e0, 0075ed40 |
| vector | 0 | 16 | 9504 | _Insert_n (4 B) | 00403ee0, 0046c940, 0046fd20, 00473230, 00481a30, 00488c10, 0050caa0, 00533eb0, 0054e7c0, 00555b70, 006c1910, 00719db0, 007275f0, 0072c170, 00730ba0, 00769d60 |
| vector | 4 | 11 | 6699 | _Insert_n (8 B) | 00434190, 00469aa0, 0046d3e0, 0049bb80, 004dd600, 004dfe80, 0054b9f0, 00558890, 006b99a0, 0071b9c0, 007721f0 |
| vector | 11 | 8 | 5320 | _Insert_n (16 B) | 00442250, 004424f0, 00469d10, 0051a7d0, 006b5a90, 006def50, 00717440, 00731440 |
| vector | 20 | 14 | 3206 | reserve (4/8 B) | 00404140, 00469880, 004729c0, 00481940, 00488af0, 0049b1d0, 004fd1a0, 0050cd00, 0054d1b0, 006b9ee0, 006dad80, 006e66d0, 0071bcc0, 00769fc0 |
| vector | 21 | 4 | 3144 | _Insert_n (60 B) | 006e1aa0, 006e1dc0, 00723c90, 007759f0 |
| vector | 22 | 4 | 3089 | _Insert_n (52/68/100/348 B) | 0044c810, 0046a9f0, 00775d10, 00782c70 |
| vector | 25 | 4 | 2780 | _Insert_n (12 B) | 0044b5e0, 00715970, 00770910, 00771c00 |
| vector | 26 | 9 | 2631 | reserve (44/52/68/88/100/104/164/168 B) | 0044cb20, 005027b0, 00547160, 0069f0b0, 0075d7d0, 00771230, 007760b0, 007761e0, 0078c360 |
| vector | 27 | 4 | 2452 | _Insert_n (16 B) | 004dd270, 005160b0, 0055abf0, 006daad0 |
| vector | 28 | 3 | 2310 | _Insert_n (88/104/200 B) | 00502460, 005136b0, 00771390 |
| vector | 31 | 3 | 2172 | _Insert_n (24 B) | 0054a2b0, 006be160, 00768800 |
| vector | 32 | 3 | 2154 | _Insert_n (20 B) | 004542c0, 00459b80, 006fad30 |
| vector | 47 | 2 | 1638 | _Insert_n (44 B) | 007729e0, 007784c0 |
| vector | 48 | 2 | 1616 | _Insert_n (28 B) | 0046ddf0, 004706a0 |
| vector | 51 | 2 | 1548 | _Insert_n (196/556 B) | 00440890, 00762f80 |
| vector | 56 | 2 | 1498 | _Insert_n (72 B) | 0075e390, 0077d1c0 |
| vector | 60 | 2 | 1398 | _Insert_n (32 B) | 0046db30, 00697d50 |
| vector | 61 | 2 | 1396 | _Insert_n (32 B) | 005479f0, 0072f220 |
| vector | 64 | 5 | 1336 | reserve (12/20/24/40 B) | 004549e0, 00459460, 0054d2a0, 00768ae0, 00770bd0 |
| vector | 66 | 30 | 1311 | _Ufill wrapper (representative 28 B; stride per instance) | 0040b8b0, 00440420, 00440450, 0044c7e0, 0046a9c0, 0046db00, 00470590, 004f3040, 00502430, 00513680, 00547290, 0069ed50, 006ce6e0, 006d4f60, 006de390, 006e1a30, 006f8960, 00723be0, 0075b260, 0075d400, 00762f20, 00771360, 007729b0, 00775930, 00775980, 007763f0, 00778490, 00782c20, 00784500, 0078c490 |
| vector | 79 | 2 | 1206 | _Insert_n (4 B) | 0046d180, 00696410 |
| vector | 81 | 2 | 1200 | _Insert_n (32 B) | 0043d890, 006faad0 |
| vector | 83 | 4 | 1180 | reserve (28/56/124 B) | 0040bb80, 00440020, 004f3070, 0050c450 |
| vector | 86 | 4 | 1146 | reserve (12/24/40/72 B) | 00454af0, 00459340, 0046fb30, 0075e240 |
| vector | 105 | 23 | 966 | _Ufill wrapper (representative 20 B; stride per instance) | 004498b0, 0044b040, 0044b200, 00453200, 00453b20, 00458a00, 00458a30, 00459030, 0046f650, 005450d0, 00549fc0, 005590c0, 00559a40, 006bd7c0, 006c9260, 006fa2a0, 00714ce0, 00727870, 0075e360, 00768760, 007707b0, 00771bd0, 0077d160 |
| vector | 116 | 4 | 872 | copy constructor (28/60 B) | 00410e70, 006de8d0, 00723b00, 00776760 |
| vector | 120 | 20 | 860 | _Ufill wrapper (representative 32 B; stride per instance) | 0043d7a0, 00441ee0, 00469200, 0046dad0, 004dc9b0, 00515e30, 0051a7a0, 005479c0, 0055abc0, 00697d20, 0069be30, 006b5a60, 006da950, 006def20, 006fa2d0, 00717410, 0071bdb0, 007243f0, 0072f1f0, 00731390 |
| vector | 137 | 17 | 782 | _Ufill wrapper (representative 4 B; stride per instance) | 00403b30, 0046c5a0, 0046f5b0, 004727b0, 004818c0, 004879f0, 0050ca50, 0051af50, 00533e80, 0054e750, 00554660, 006c18e0, 00719d50, 007272f0, 0072be80, 00730b10, 00769b10 |
| vector | 157 | 18 | 702 | _Ufill wrapper (representative 8 B; stride per instance) | 00433620, 00468e70, 0046c3b0, 0046c3e0, 0046c5f0, 0046c7b0, 004991b0, 004dc9e0, 004dfdd0, 0054b420, 00558690, 00695980, 00699390, 006b4a20, 006b9550, 0070d0b0, 0071aee0, 007720d0 |
| vector | 199 | 2 | 576 | reserve (68/196 B) | 00440150, 0075b140 |
| vector | 201 | 14 | 558 | range assignment, element operator= (stride varies) | 0043fa20, 0044c2a0, 0046eda0, 004f2ec0, 005134c0, 0051a2b0, 006b55c0, 006dedd0, 00716580, 00723fd0, 007311c0, 00762d70, 00773bd0, 00784350 |
| vector | 224 | 2 | 516 | reserve (16 B) | 00469970, 006b5d80 |
| vector | 257 | 2 | 458 | reserve (16 B) | 004dd510, 00516320 |
| vector | 268 | 2 | 432 | copy constructor (44 B) | 0075d9c0, 00772d20 |
| vector | 356 | 4 | 312 | _Buy (28/44/60/100 B) | 00410b80, 006ce590, 0075d970, 00775690 |
| vector | 433 | 3 | 267 | _Buy (164/552/556 B) | 00510760, 00510f90, 005111c0 |
| vector | 439 | 7 | 258 | _Destroy range (stride varies) | 0043f860, 0051ac60, 0051bf40, 00709ab0, 0075e040, 00762960, 0077d190 |
| vector | 505 | 5 | 220 | _Ufill wrapper (representative 112 B; stride per instance) | 004347d0, 00455500, 00516d70, 00755f20, 00773cd0 |
| vector | 548 | 1 | 101 | _Xlen throw helper | 006ce410 |
| vector | 622 | 3 | 177 | fill assignment (44 B; string + record) | 00772850, 00776050, 00776d50 |
| vector | 678 | 2 | 162 | _Buy (4/8 B) | 005587e0, 00730b40 |
| vector | 687 | 2 | 160 | _Buy (12/20 B) | 00558840, 00715920 |
| vector | 715 | 3 | 150 | fill assignment (20 B; scalar + record) | 00449550, 00558f20, 005597d0 |
| vector | 782 | 3 | 135 | copy_backward (40 B trivial) | 00458630, 006de100, 006fa150 |
| vector | 827 | 3 | 126 | fill assignment (40 B trivial) | 00458360, 006dcff0, 006f9fb0 |
| vector | 921 | 2 | 108 | fill assignment (32 B; scalar + string) | 0069ba20, 0072f050 |
| vector | 1054 | 2 | 80 | uninitialized_copy forwarding overload (phantom Hex-Rays args) | 0047f240, 0047f270 |
| vector | 1076 | 2 | 76 | fill assignment (12 B polymorphic; vptr preserved) | 0044ad00, 0070ce20 |
| vector | 1093 | 2 | 74 | _Destroy range (12 B; virtual destructor) | 0044afc0, 00453230 |
| vector<bool> | 548 | 1 | 101 | _Xlen throw helper | 0046fac0 |

## Unclustered instances (each is independent)

| Kind | Address | Bytes | Observed operation / disposition |
|---|---|---:|---|
| string | 00403880 | 122 | erase(pos,count), char |
| vector | 0040bd50 | 800 | _Insert_n (28 B), individualized element operations |
| vector | 00410bd0 | 79 | _Buy (element 16 B) |
| string | 0042e200 | 117 | compare(pos,count,ptr,count), char |
| vector | 00435290 | 823 | _Insert_n (48 B), individualized element operations |
| string | 0043ea30 | 242 | append(string,pos,count), char |
| string | 0043eb30 | 280 | append(ptr,count), char |
| tree | 0043efc0 | 732 | erase(iterator), smaller node value/color offsets +16/+17 |
| vector | 00440540 | 841 | _Insert_n (56 B), individualized element operations |
| vector | 004498e0 | 807 | _Insert_n (20 B), individualized element operations |
| vector | 0044a030 | 293 | reserve (20 B nontrivial) |
| vector | 0044b2d0 | 775 | _Insert_n (12 B), individualized element operations |
| vector | 00454590 | 798 | _Insert_n (24 B), individualized element operations |
| vector | 00455530 | 795 | _Insert_n (48 B), individualized element operations |
| vector | 00455c50 | 289 | reserve (48 B) |
| vector | 004595a0 | 794 | _Insert_n (40 B), individualized element operations |
| vector | 004598c0 | 699 | _Insert_n (40 B), individualized element operations |
| vector | 0046cc00 | 706 | _Insert_n (8 B), individualized element operations |
| vector | 0046ced0 | 683 | _Insert_n (8 B), individualized element operations |
| vector<bool> | 0046ff80 | 130 | _Trim: word-end shrink, bit count, last-word mask |
| vector | 00470010 | 752 | _Insert_n (12 B), individualized element operations |
| vector<bool> | 004705c0 | 210 | insert bit range: bit-iterator offset + word-vector insertion |
| vector | 0047ef80 | 693 | _Insert_n (4 B), individualized element operations |
| string | 004856e0 | 136 | erase(pos,count), wchar16 |
| string | 00488830 | 278 | assign(string,pos,count), wchar16 |
| vector | 004f33d0 | 793 | _Insert_n (124 B), individualized element operations |
| vector | 00510510 | 87 | _Buy (element 80 B) |
| string | 00512630 | 220 | append(count,char) |
| string | 005158f0 | 116 | compare(pos,count,ptr,count), wchar16 |
| vector | 00516da0 | 819 | _Insert_n (80 B), individualized element operations |
| vector | 0051b0c0 | 558 | _Insert_n (1 B trivial) |
| string | 00533040 | 876 | replace(pos,count,string,pos,count), wchar16 |
| string | 00541150 | 212 | append(count,wchar16) |
| vector | 00545100 | 805 | _Insert_n (36 B), individualized element operations |
| vector | 005472c0 | 793 | _Insert_n (68 B), individualized element operations |
| list | 00556e80 | 614 | MISCLASSIFIED game routine with inline list operations; manual game hook |
| vector | 005590f0 | 771 | _Insert_n (20 B), individualized element operations |
| vector | 00559a70 | 789 | _Insert_n (20 B), individualized element operations |
| string | 006959f0 | 307 | replace(pos,count,n,wchar16) |
| string | 00696bd0 | 253 | append(string,pos,count), wchar16 |
| string | 00696cd0 | 291 | append(ptr,count), wchar16 |
| string | 00696e00 | 253 | assign(ptr,count), wchar16 |
| vector | 00699520 | 638 | _Insert_n (8 B), individualized element operations |
| vector | 0069bf30 | 701 | _Insert_n (32 B), individualized element operations |
| vector | 0069ed80 | 804 | _Insert_n (44 B), individualized element operations |
| list | 006a9fe0 | 127 | _Incsize with length_error |
| list | 006aa880 | 127 | MISCLASSIFIED game routine with inline list operations; manual game hook |
| vector | 006b4e70 | 252 | reserve (8 B nontrivial) |
| vector | 006b4f70 | 677 | _Insert_n (8 B), individualized element operations |
| vector | 006c9290 | 805 | _Insert_n (36 B), individualized element operations |
| vector | 006ce710 | 771 | _Insert_n (100 B), individualized element operations |
| list | 006d0ba0 | 206 | MISCLASSIFIED game routine with inline list operations; manual game hook |
| vector | 006d4f90 | 867 | _Insert_n (60 B), individualized element operations |
| vector | 006de600 | 719 | _Insert_n (28 B), individualized element operations |
| vector | 006f89b0 | 806 | _Insert_n (44 B), individualized element operations |
| vector | 0070d140 | 675 | _Insert_n (8 B), individualized element operations |
| vector | 0071be00 | 657 | _Insert_n, individualized element operations |
| vector | 00724470 | 683 | _Insert_n (16 B), individualized element operations |
| vector | 007278a0 | 805 | _Insert_n (36 B), individualized element operations |
| vector | 0072bee0 | 579 | _Insert range (4 B pointers) |
| vector | 00732610 | 774 | _Insert_n (16 B), individualized element operations |
| list | 0073fe80 | 125 | _Incsize with length_error |
| list | 00749420 | 238 | MISCLASSIFIED game routine with inline list operations; manual game hook |
| list | 00749510 | 218 | MISCLASSIFIED game routine with inline list operations; manual game hook |
| vector | 00755f50 | 778 | _Insert_n (80 B), individualized element operations |
| vector | 0075b290 | 780 | _Insert_n (68 B), individualized element operations |
| vector | 0075d4b0 | 798 | _Insert_n (44 B), individualized element operations |
| vector | 00773d60 | 772 | _Insert_n (96 B), individualized element operations |
| vector | 00776420 | 826 | _Insert_n (168 B), individualized element operations |
| list | 00782330 | 137 | MISCLASSIFIED game routine with inline list operations; manual game hook |
| list | 007823c0 | 388 | MISCLASSIFIED game routine with inline list operations; manual game hook |
| vector | 00784590 | 820 | _Insert_n (552 B), individualized element operations |
| vector | 0078c4c0 | 868 | _Insert_n (164 B), individualized element operations |

## Fit and coverage

11 clear non-template game routines: 00556e80, 006aa880, 006d0ba0, 0073f3b0, 0073f4f0, 0073ff40, 00740790, 00749420, 00749510, 00782330, 007823c0. Their list calls caused seed/clone propagation to classify their whole bodies as STL. Keep them out of automatic template substitution. 00782330 searches a game-specific key in a list and optionally allocates a 16-byte game record before push_back; it is also excluded. The 47 unclustered vector entries mostly are _Insert_n variants with element operations inlined, plus _Buy/reserve/range-insert; they fit semantic families but not blind cluster cloning. No other confirmed non-template entry found in the representative/all-unclustered pseudocode review; this is not a proof that all other cluster members are interchangeable.

The three largest non-null vector clusters by bytes are 0 (16 / 9504), 4 (11 / 6699), 11 (8 / 5320). All are _Insert_n, for 4/8/16-byte elements respectively. Binary FuncInfo records for all 35 first-slice functions have four zero-action unwind entries and two try blocks (work/stl_eh.json); the saved value is freed on normal return only. The template deliberately has no owning C++ destructor for it. Cluster 11 contains two ownership layouts: three instances copy an array record with owner at +0 and use 0x691110; five copy an embedded vector with owner at +4 and use 0x690E60. Copy construction and range destruction are nontrivial. Implement one insertion algorithm with separate operation traits, keeping original range helper boundaries. Total first slice: 35 functions / 21523 bytes.

## Proposal for remaining generation (no generator implementation for remaining instances)

A per-instance manifest needs address, member family, container base offset/layout, element byte size/alignment, signed difference rule, ABI/stack pop and return policy; allocator/free pair and debug args; copy constructor, assignment, destructor and range helper addresses/conventions; inline field copy/vptr rules; tree value size, color/isnil offsets, node allocation size and comparator; string character width, SSO threshold and traits operations; bit-iterator fields; exception cleanup stages. Keep evidence instruction addresses for every extracted fact. Generate a trait and thin HOOK wrapper from approved family templates; compile and gate every address, then emudiff with --src. Never treat inconclusive as passed.

Manual work: identify semantic element types and alignment; validate reciprocal divisions; separate inlined game bodies; audit aliased input and owning/refcounted/virtual element operations; recover exception guarantees and comparator behavior. Review every mixed-shape cluster (especially tree rotations and fill wrappers), not only its representative. Automatic inference should stop on contradictory size/ABI/call evidence.

Expected semantic-family coverage ceiling is 407/418 after excluding the 11 confirmed game routines, including support helpers and vector<bool>; it is an estimate, not achieved verification. The 345 entries with non-null cluster IDs include 4 misclassified game bodies: 341 are immediate family candidates. Remaining 66 eligible unclustered entries need explicit manifests/adapters, with inlined owning operations the main manual cost. Expect fewer than 407 to be mechanically inferred without review. Only this first slice has emitted hooks; untouched helpers remain original bindings for now. Keep and broaden the first-slice valid-container emulator fixtures (zero insert, both spare-capacity branches, reallocation, aliased value, overflow, throws) before broad installation: the stock generic profiles do not guarantee valid vector invariants.


## Survey evidence artifacts

work/stl_inventory.json records all 418 entries and exact allocator call-site instruction windows; allocation_stride_hint is a pseudocode-derived hint, to cross-check against those windows, not a fully verified type manifest. work/stl_slice_manifest.json contains the reviewed size/helper/owner-offset manifests for all 35 first-slice addresses. work/stl_evidence.txt and work/stl_shapes.txt retain the representative/unclustered shape review.
