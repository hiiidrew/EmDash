#!/usr/bin/env python3
"""Populate a SQLite database with sample telemetry data.

This script is useful for local development and testing. It creates a
`telemetry` table if it does not exist and inserts a configurable number
of rows containing timestamped altitude and velocity values.
"""

import argparse
import sqlite3
import time
import random
from pathlib import Path


def seed_database(db_path: Path, count: int) -> int:
    """Create the database (if needed) and insert ``count`` rows."""
    conn = sqlite3.connect(db_path)
    cur = conn.cursor()
    cur.execute(
        """
        CREATE TABLE IF NOT EXISTS telemetry (
            timestamp INTEGER,
            altitude REAL,
            velocity REAL
        )
        """
    )

    now = int(time.time())
    rows = []
    for i in range(count):
        rows.append(
            (
                now + i,
                1000.0 + 5 * i,  # synthetic altitude in meters
                250.0 + random.uniform(-5, 5),  # synthetic velocity in m/s
            )
        )
    cur.executemany("INSERT INTO telemetry VALUES (?,?,?)", rows)
    conn.commit()
    conn.close()
    return len(rows)


def main() -> None:
    parser = argparse.ArgumentParser(description="Seed a SQLite database with telemetry data")
    parser.add_argument("--db", type=Path, default=Path("telemetry.db"), help="Path to SQLite database")
    parser.add_argument("--count", type=int, default=10, help="Number of rows to insert")
    args = parser.parse_args()

    inserted = seed_database(args.db, args.count)
    print(f"Inserted {inserted} rows into {args.db}")


if __name__ == "__main__":
    main()
