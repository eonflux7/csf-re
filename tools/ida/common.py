"""Shared idalib helpers. `import idapro` must happen before any ida_* import."""
import os, sys, json
import idapro  # noqa: F401  (must be first)

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
IDB = os.path.join(ROOT, "ida", "CommXPC.exe.i64")
EXE = os.path.join(ROOT, "work", "pc", "CommXPC.exe")


def open_db(path=None, auto=True):
    path = path or (IDB if os.path.exists(IDB) else EXE)
    if idapro.open_database(path, auto) != 0:
        sys.exit(f"failed to open {path}")
    import ida_auto
    ida_auto.auto_wait()


def close_db(save=True):
    idapro.close_database(save)
