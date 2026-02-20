#!/usr/bin/env python3
"""
Capture screenshots of the UI prototype for documentation.
Requires: pip install playwright && playwright install chromium
Run: cd ui-prototype && python3 -m http.server 9999 &
     python3 capture_screenshots.py
"""
import asyncio
import os
import subprocess
import sys

try:
    from playwright.sync_api import sync_playwright
except ImportError:
    print("Install: pip install playwright && playwright install chromium")
    sys.exit(1)

OUT_DIR = os.path.join(os.path.dirname(__file__), "..", "docs", "images")
URL = "http://localhost:9999"

def capture():
    os.makedirs(OUT_DIR, exist_ok=True)
    with sync_playwright() as p:
        browser = p.chromium.launch()
        page = browser.new_page(viewport={"width": 1920, "height": 1080})
        try:
            page.goto(URL, wait_until="networkidle", timeout=10000)
        except Exception as e:
            print(f"Could not connect to {URL}. Start server first: python3 -m http.server 9999")
            browser.close()
            sys.exit(1)

        page.screenshot(path=os.path.join(OUT_DIR, "ui-initial-netflix-focused.png"))
        print("Saved: ui-initial-netflix-focused.png")

        page.keyboard.press("ArrowRight")
        page.wait_for_timeout(300)
        page.screenshot(path=os.path.join(OUT_DIR, "ui-focus-prime-video.png"))
        print("Saved: ui-focus-prime-video.png")

        page.keyboard.press("KeyS")
        page.wait_for_timeout(400)
        page.screenshot(path=os.path.join(OUT_DIR, "ui-settings-panel-open.png"))
        print("Saved: ui-settings-panel-open.png")

        browser.close()
    print(f"Screenshots saved to {OUT_DIR}")

if __name__ == "__main__":
    capture()
