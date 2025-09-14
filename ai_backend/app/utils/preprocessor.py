"""Utilities for converting telemetry payloads into model features."""
from __future__ import annotations

from ..models.telemetry_payload import TelemetryPayload


def extract_features(payload: TelemetryPayload) -> dict[str, float]:
    """Convert the telemetry payload into a simple feature mapping."""
    return {"altitude": payload.altitude, "velocity": payload.velocity}
