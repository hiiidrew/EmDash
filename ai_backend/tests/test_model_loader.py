from ai_backend.app.services.model_loader import load_model


def test_load_model_has_altitude_weight():
    model = load_model()
    assert "altitude" in model.weights
    assert model.bias > 0
