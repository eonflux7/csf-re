#!/usr/bin/env python3
"""(Re)build db/functions.csv from export/functions.jsonl + db/regions.yaml.

Re-runnable: names, modules and statuses already in functions.csv are preserved; only region/lib
classification and sizes are refreshed. New functions are added; vanished ones are dropped (and reported).

status values: lib (never reverse), todo, done (gated reimpl), verified (playtested), escalated.
name_src: ida (auto/FLIRT), rw/netz/... (boundary id), agent, opus, user.
"""
import csv, json, os, sys
import yaml

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CSV = os.path.join(ROOT, "db", "functions.csv")
FIELDS = ["ea", "size", "region", "module", "name", "name_src", "status"]


def load_regions():
    return yaml.safe_load(open(os.path.join(ROOT, "db", "regions.yaml")))


def region_of(regions, ea):
    r = None
    for x in regions:
        if x["start"] <= ea < x["end"]:
            r = x
    return r


def load_csv():
    if not os.path.exists(CSV):
        return {}
    return {int(r["ea"], 16): r for r in csv.DictReader(open(CSV))}


def save_csv(rows):
    with open(CSV, "w", newline="") as f:
        w = csv.DictWriter(f, FIELDS, lineterminator="\n")
        w.writeheader()
        for ea in sorted(rows):
            w.writerow(rows[ea])


def main():
    regions = load_regions()
    old = load_csv()
    rows = {}
    for line in open(os.path.join(ROOT, "export", "functions.jsonl")):
        f = json.loads(line)
        ea = f["ea"]
        r = region_of(regions, ea)
        # FLIRT "lib" matches outside the CRT range are false positives on tiny functions (e.g. VS2010
        # Concurrency:: names in a 2006 binary): the address region wins.
        region = (r["lib"] or "game") if r else "crt" if f["lib"] else "unknown"
        flirt_bogus = f["lib"] and region != "crt"
        prev = old.get(ea)
        auto_name = f["name"]
        if prev:
            row = dict(prev)
            row["size"] = str(f["size"])
            row["region"] = region
            # region reclassification can move a function in/out of lib
            if region != "game" and row["status"] == "todo":
                row["status"] = "lib"
            elif region == "game" and row["status"] == "lib":
                row["status"] = "todo"
            if flirt_bogus and row["name_src"] == "ida":
                row["name"], row["name_src"] = "", ""
        else:
            named = not auto_name.startswith(("sub_", "nullsub_", "j_")) and not flirt_bogus
            row = dict(ea=f"{ea:08x}", size=str(f["size"]), region=region, module="",
                       name=auto_name if named else "", name_src="ida" if named else "",
                       status="todo" if region == "game" else "lib")
        rows[ea] = row
    gone = set(old) - set(rows)
    if gone:
        print(f"warning: {len(gone)} functions disappeared from export: {[hex(g) for g in sorted(gone)][:10]}")
    save_csv(rows)
    print(f"wrote {len(rows)} rows to db/functions.csv")


if __name__ == "__main__":
    main()
