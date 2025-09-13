#!/usr/bin/env python3
"""Export telemetry records from a SQLite database to JSON or CSV."""

import argparse
import csv
import json
import sqlite3
from pathlib import Path


def export(db_path: Path, out_path: Path, fmt: str) -> int:
    conn = sqlite3.connect(db_path)
    cur = conn.cursor()
    cur.execute("SELECT timestamp, altitude, velocity FROM telemetry ORDER BY timestamp")
    rows = cur.fetchall()
    conn.close()

    if fmt == "json":
        with open(out_path, "w", encoding="utf-8") as f:
            json.dump(
                [
                    {"timestamp": ts, "altitude": alt, "velocity": vel}
                    for ts, alt, vel in rows
                ],
                f,
                indent=2,
            )
    else:  # csv
        with open(out_path, "w", newline="", encoding="utf-8") as f:
            writer = csv.writer(f)
            writer.writerow(["timestamp", "altitude", "velocity"])
            writer.writerows(rows)
    return len(rows)


def main() -> None:
    parser = argparse.ArgumentParser(description="Export telemetry data from a SQLite DB")
    parser.add_argument("--db", type=Path, default=Path("telemetry.db"), help="Path to SQLite database")
    parser.add_argument("--out", type=Path, required=True, help="Destination file")
    parser.add_argument("--format", choices=["json", "csv"], default="json", help="Export format")
    args = parser.parse_args()

    count = export(args.db, args.out, args.format)
    print(f"Exported {count} rows to {args.out}")


if __name__ == "__main__":
    main()
