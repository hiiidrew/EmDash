import pytest
from fastapi.testclient import TestClient

from ai_backend.app.main import app


@pytest.fixture()
def client() -> TestClient:
    return TestClient(app)
