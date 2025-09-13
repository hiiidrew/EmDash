#!/usr/bin/env python3
"""Convert telemetry JSON to a minimal FlatBuffer binary.

The script expects a JSON file produced by ``export_telemetry.py`` and
writes a FlatBuffer containing a vector of simple ``Telemetry`` tables
(timestamp, altitude, velocity). It relies on the ``flatbuffers`` Python
package but does not require generated classes.
"""

import argparse
import json
from pathlib import Path

import flatbuffers


def build_telemetry(builder: flatbuffers.Builder, ts: int, altitude: float, velocity: float) -> int:
    """Build a Telemetry table and return its offset."""
    builder.StartObject(3)
    builder.PrependFloat32Slot(2, velocity, 0.0)
    builder.PrependFloat32Slot(1, altitude, 0.0)
    builder.PrependUint32Slot(0, ts, 0)
    return builder.EndObject()


def main() -> None:
    parser = argparse.ArgumentParser(description="Encode telemetry JSON into FlatBuffer binary")
    parser.add_argument("--input", type=Path, required=True, help="Input JSON file")
    parser.add_argument("--output", type=Path, required=True, help="Output binary file")
    args = parser.parse_args()

    with open(args.input, "r", encoding="utf-8") as f:
        data = json.load(f)

    records = data if isinstance(data, list) else [data]
    builder = flatbuffers.Builder(128)

    offsets = []
    for rec in records:
        offsets.append(
            build_telemetry(
                builder,
                int(rec["timestamp"]),
                float(rec["altitude"]),
                float(rec["velocity"]),
            )
        )

    # Build vector of telemetry offsets
    count = len(offsets)
    builder.StartVector(4, count, 4)
    for off in reversed(offsets):
        builder.PrependUOffsetTRelative(off)
    vec = builder.EndVector()

    # Root table containing the vector
    builder.StartObject(1)
    builder.PrependUOffsetTRelativeSlot(0, vec, 0)
    root = builder.EndObject()
    builder.Finish(root)

    with open(args.output, "wb") as f:
        f.write(builder.Output())

    print(f"Encoded {count} telemetry records to {args.output}")


if __name__ == "__main__":
    main()
