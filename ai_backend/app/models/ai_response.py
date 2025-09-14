from pydantic import BaseModel

class AIResponse(BaseModel):
    """Represents the autopilot recommendation produced by the AI service."""
    action: str
    confidence: float
