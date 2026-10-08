"""Check src/types/auto_classes.h (+ typed vtables) against the IDB's local types (read-only): missing structs,
and structs whose member count differs from the header's data members (a parse error inside the body drops the
rest). --parse N: parse the first N vtbl structs into a scratch til and print IDA's messages."""
import os, re, sys
sys.path.insert(0, os.path.dirname(__file__))
from common import open_db, close_db
import ida_typeinf, ida_kernwin
from sync import header_decls

src = header_decls()
open_db()
if "--parse" in sys.argv:
    n = int(sys.argv[sys.argv.index("--parse") + 1])
    vt = re.findall(r"^struct \w+_vtbl \{.*?^\};", src, re.M | re.S)[:n]
    til = ida_typeinf.new_til("scratch", "scratch")
    pre = src[:src.index("struct C_")]
    fwd = "\n".join(l for l in src.splitlines() if re.match(r"^struct \w+;$", l))
    text = pre + fwd + "\n" + "\n".join(vt)
    errs = ida_typeinf.parse_decls(til, text, None, ida_typeinf.HTI_DCL | ida_typeinf.HTI_PAK1)
    e2 = ida_typeinf.parse_decls(ida_typeinf.new_til("s2", "s2"), text.replace(" this,", " self,").replace(" this)", " self)"),
                                 None, ida_typeinf.HTI_DCL | ida_typeinf.HTI_PAK1)
    t = ida_typeinf.tinfo_t()
    print("errors", errs, "| with `this` renamed:", e2, "| first vtbl parsed:",
          t.get_named_type(til, re.match(r"struct (\w+)", vt[0]).group(1)))
    close_db(save=False)
    sys.exit()
bodies = dict(re.findall(r"^struct (\w+)[^{;]*\{(.*?)^\};", src, re.M | re.S))
til = ida_typeinf.get_idati()
missing, short = [], []
for n, body in bodies.items():
    t = ida_typeinf.tinfo_t()
    if not t.get_named_type(til, n):
        missing.append(n)
        continue
    want = sum(1 for l in body.splitlines() if re.match(r"^\s+[\w:<>\*\s]+\b\w+(\[\w+\])*;", l))
    udt = ida_typeinf.udt_type_data_t()
    t.get_udt_details(udt)
    have = sum(1 for m in udt if not m.is_baseclass())
    if have < want:
        short.append((n, have, want))
print(f"{len(bodies)} structs, {len(missing)} missing {missing[:10]}, {len(short)} with fewer members than the "
      f"header: {short[:15]}")
close_db(save=False)
