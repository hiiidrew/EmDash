from pydantic import BaseModel

class TelemetryPayload(BaseModel):
    """Incoming telemetry information for the AI autopilot service."""
    altitude: float
    velocity: float
