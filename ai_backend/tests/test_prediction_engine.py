from ai_backend.app.models.telemetry_payload import TelemetryPayload
from ai_backend.app.services.model_loader import load_model
from ai_backend.app.services.prediction_engine import PredictionEngine


def test_recommend_descend_above_threshold():
    model = load_model()
    engine = PredictionEngine(model)
    payload = TelemetryPayload(altitude=2000, velocity=100)
    response = engine.recommend(payload)
    assert response.action == "DESCEND"


def test_recommend_ascend_below_threshold():
    model = load_model()
    engine = PredictionEngine(model)
    payload = TelemetryPayload(altitude=200, velocity=100)
    response = engine.recommend(payload)
    assert response.action == "ASCEND"
