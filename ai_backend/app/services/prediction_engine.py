"""Prediction engine that generates autopilot recommendations."""
from __future__ import annotations

from ..models.ai_response import AIResponse
from ..models.telemetry_payload import TelemetryPayload
from ..utils.preprocessor import extract_features


class PredictionEngine:
    """Combine preprocessing and model evaluation to produce a response."""

    def __init__(self, model, threshold: float = 0.0):
        self.model = model
        self.threshold = threshold

    def recommend(self, payload: TelemetryPayload) -> AIResponse:
        features = extract_features(payload)
        score = self.model.evaluate(features)
        action = "ASCEND" if score > self.threshold else "DESCEND"
        confidence = abs(score) / (abs(self.model.bias) + 1e-9)
        return AIResponse(action=action, confidence=confidence)
