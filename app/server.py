import os

import serial
from fastapi import FastAPI
from fastapi.responses import FileResponse


BASE_DIR = os.path.dirname(os.path.abspath(__file__))
STATIC_DIR = os.path.join(BASE_DIR, "static")

arduino = serial.Serial("/dev/ttyACM0", 9600, timeout=1)
app = FastAPI()


@app.get("/")
def read_root():
    return FileResponse(os.path.join(STATIC_DIR, "index.html"))


@app.post("/led/{state}")
def control_led(state: str):
    if state == "on":
        arduino.write(b"1")
        return {"status": "LED turned ON"}
    elif state == "off":
        arduino.write(b"0")
        return {"status": "LED turned OFF"}
    return {"error": "Invalid command"}
