"""API routes exposing autopilot recommendations based on telemetry."""
from fastapi import APIRouter

from ..models.ai_response import AIResponse
from ..models.telemetry_payload import TelemetryPayload
from ..services.model_loader import load_model
from ..services.prediction_engine import PredictionEngine

router = APIRouter(prefix="/telemetry", tags=["telemetry"])

_model = load_model()
_engine = PredictionEngine(_model)


@router.post("/autopilot", response_model=AIResponse)
def autopilot(payload: TelemetryPayload) -> AIResponse:
    """Return an autopilot action based on the supplied telemetry."""
    return _engine.recommend(payload)
