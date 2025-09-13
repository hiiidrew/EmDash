#!/usr/bin/env python3
"""Run build and test scripts for different components of the project.

This wrapper simplifies local testing by delegating to the shell scripts
already present in the ``scripts/test`` directory. It prints the stdout
of each script and reports any failures at the end.
"""

import argparse
import subprocess
import sys
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
TEST_DIR = SCRIPT_DIR / "test"

STEPS = {
    "embedded": TEST_DIR / "test_embedded.sh",
    "gui": TEST_DIR / "test_gui.sh",
    "ai_backend": TEST_DIR / "test_ai_backend.sh",
}


def run_step(step: str) -> int:
    script = STEPS[step]
    if not script.exists():
        print(f"Skipping {step}: {script} not found")
        return 0
    print(f"== Running {script} ==")
    result = subprocess.run(["bash", str(script)], capture_output=True, text=True)
    if result.stdout:
        print(result.stdout)
    if result.returncode != 0:
        if result.stderr:
            print(result.stderr, file=sys.stderr)
        print(f"Step {step} failed with code {result.returncode}", file=sys.stderr)
    return result.returncode


def main() -> int:
    parser = argparse.ArgumentParser(description="Run project test pipeline")
    parser.add_argument(
        "--component",
        choices=list(STEPS.keys()) + ["all"],
        default="all",
        help="Component to test (default: all)",
    )
    args = parser.parse_args()

    steps = STEPS.keys() if args.component == "all" else [args.component]
    failures = []
    for step in steps:
        if run_step(step) != 0:
            failures.append(step)

    if failures:
        print("Failed components: " + ", ".join(failures), file=sys.stderr)
        return 1
    print("All selected components passed their tests")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
