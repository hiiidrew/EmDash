from ai_backend.app.models.telemetry_payload import TelemetryPayload


def test_autopilot_route(client):
    response = client.post(
        "/telemetry/autopilot", json={"altitude": 300, "velocity": 100}
    )
    assert response.status_code == 200
    data = response.json()
    assert data["action"] == "ASCEND"
    assert 0 <= data["confidence"] <= 1
