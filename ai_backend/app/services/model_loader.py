"""Utility for loading the lightweight autopilot model.

The model is represented by a simple linear combination of features.  Coefficients
are stored in ``ai_backend/models/metadata.json`` so the service can be extended
or retrained without code changes.
"""
from __future__ import annotations

import json
from dataclasses import dataclass
from pathlib import Path
from typing import Dict


@dataclass
class LinearModel:
    """A tiny linear model used for autopilot recommendations."""

    weights: Dict[str, float]
    bias: float

    def evaluate(self, features: Dict[str, float]) -> float:
        """Return the raw score for the provided feature mapping."""
        total = self.bias
        for name, weight in self.weights.items():
            total += weight * features.get(name, 0.0)
        return total


def load_model(model_dir: Path | None = None) -> LinearModel:
    """Load model weights and bias from ``metadata.json``.

    Parameters
    ----------
    model_dir: Path | None
        Directory containing ``metadata.json``.  Defaults to the ``models``
        folder at the project root.
    """
    if model_dir is None:
        model_dir = Path(__file__).resolve().parents[2] / "models"
    metadata_file = model_dir / "metadata.json"
    with metadata_file.open() as f:
        meta = json.load(f)
    return LinearModel(weights=meta["weights"], bias=meta["bias"])
