#!/usr/bin/env python3
"""Set name/status/module of functions in db/functions.csv.

Usage: fn.py <ea> [name=X] [status=todo|done|verified|escalated|lib] [module=M] [src=opus|agent|user|...]
"""
import os, sys
sys.path.insert(0, os.path.dirname(__file__))
from triage import load_csv, save_csv


def update(rows, ea, **kv):
    r = rows[ea]
    if "name" in kv:
        r["name"] = kv["name"]
        r["name_src"] = kv.get("src", r["name_src"] or "opus")
    for k in ("status", "module"):
        if k in kv:
            r[k] = kv[k]


if __name__ == "__main__":
    rows = load_csv()
    ea = int(sys.argv[1], 16)
    kv = dict(a.split("=", 1) for a in sys.argv[2:])
    update(rows, ea, **kv)
    save_csv(rows)
    print(rows[ea])
