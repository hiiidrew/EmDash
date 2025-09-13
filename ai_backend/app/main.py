"""Entry point for the AI backend service."""
from fastapi import FastAPI

from .routes.telemetry import router as telemetry_router

app = FastAPI(title="EmDash AI Backend")
app.include_router(telemetry_router)
