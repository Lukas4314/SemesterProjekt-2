import serial
import matplotlib.pyplot as plt

# Åbn seriel port
ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)

# Aktiver interaktivt plot
plt.ion()
fig, ax = plt.subplots()

data = [0] * 100  # Start med dummy-værdier
line, = ax.plot(data)
ax.set_ylim(0, 4095)  # Hvis du bruger 12-bit ADC på Pico

while True:
    try:
        line_raw = ser.readline().decode('utf-8', errors='ignore').strip()
        print(f"Modtog: '{line_raw}'")  # <--- denne linje viser præcist, hvad Pico sender

        if line_raw.isdigit():
            val = int(line_raw)
            data.append(val)
            data.pop(0)
            line.set_ydata(data)
            line.set_xdata(range(len(data)))
            ax.relim()
            ax.autoscale_view()
            plt.pause(0.01)  # Pause for at opdatere grafen
    except Exception as e:
        print("Fejl:", e)

