import serial
import tkinter as tk
from serial.tools import list_ports

state = False
fontFamily = "Consolas"
fontHeading = (fontFamily, 16)
fontBody = (fontFamily, 12)
fontNote = (fontFamily, 10)

def findArduinoPort():
    ports = list_ports.grep("Arduino")
    for p in ports:
        return p.name
    return "No port found"

arduino = serial.Serial()
arduino.baudrate = 9600
arduino.port = findArduinoPort()
if arduino.port == "No port found":
    print("Couldn't find Arduino port :(")
    exit()
arduino.open()

window = tk.Tk()
window.title("Magnetometar")
window.iconbitmap("magnet-simple.ico")
window.geometry("384x216")
window.resizable(width=False, height=False)

titleLabel = tk.Label(text="Magnetometar", font=fontHeading)
titleLabel.pack(pady=(18, 8))

maxMeasurementGauss = 0.0
currentMeasurementGauss = 0.0

maxMeasurementString = tk.StringVar()
currentMeasurementString = tk.StringVar()

maxMeasurementString.set("Maksimum: {B: 8.2f} G".format(B=maxMeasurementGauss))
maxMeasurementLabel = tk.Label(textvariable=maxMeasurementString, font=fontBody)
currentMeasurementString.set("Trenutno: {B: 8.2f} G".format(B=currentMeasurementGauss))
currentMeasurementLabel = tk.Label(textvariable=currentMeasurementString, font=fontBody)
currentMeasurementLabel.pack(pady=(10, 2))
maxMeasurementLabel.pack(pady=(2, 20))


def loop():
    global state, arduino, currentMeasurementGauss, currentMeasurementLabel, maxMeasurementGauss, maxMeasurementLabel, window
    line = arduino.readline().decode()
    if (state):
        if (line[0:2] == "G:"):
            currentMeasurementGauss = float(line[3:])
            # print(currentMeasurementGauss)
            currentMeasurementString.set("Trenutno: {B: 8.2f} G".format(B=currentMeasurementGauss))
            if (abs(currentMeasurementGauss) > abs(maxMeasurementGauss)):
                maxMeasurementGauss = currentMeasurementGauss
                maxMeasurementString.set("Maksimum: {B: 8.2f} G".format(B=maxMeasurementGauss))
        window.update()
    
    window.after(10, loop)


buttons = tk.Frame()


def play_pause():
    global state
    state = not state
    if (state):
        playPauseButton["text"] = "Pauza"
        # resetButton["state"] = "disabled"
    else:
        playPauseButton["text"] = "Početak"
        # resetButton["state"] = "normal"


playPauseButton = tk.Button(buttons, text="Početak", command=play_pause, font=fontBody)
playPauseButton.pack(padx=(0, 2), side=tk.LEFT)


def reset():
    global maxMeasurementGauss, currentMeasurementGauss, currentMeasurementString, maxMeasurementString, resetButton, window
    maxMeasurementGauss = 0.0
    currentMeasurementGauss = 0.0
    currentMeasurementString.set("Trenutno: {B: 8.2f} G".format(B=currentMeasurementGauss))
    maxMeasurementString.set("Maksimum: {B: 8.2f} G".format(B=maxMeasurementGauss))
    # resetButton["state"] = "disabled"
    window.update()


resetButton = tk.Button(buttons, text="Resetiraj", command=reset, font=fontBody)
resetButton.pack(padx=(2, 0), side=tk.LEFT)

buttons.pack()

conversionLabel = tk.Label(text="1 G = 0.00001 T", font=fontNote)
conversionLabel.pack(pady=(10, 0))

window.after(10, loop)

window.mainloop()
