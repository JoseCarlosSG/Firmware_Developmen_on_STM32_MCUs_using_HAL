import serial
import serial.tools.list_ports

ports = serial.tools.list_ports.comports()
for port in ports:
    print(port.device)


# Configura el puerto serie
ser = serial.Serial(
    port='/dev/ttyACM0',    # Reemplaza con el puerto de tu microcontrolador (en Linux puede ser algo como '/dev/ttyUSB0')
    baudrate=115200,        # Configura la velocidad en baudios según tu microcontrolador
    timeout=2               # Tiempo de espera para lectura en segundos
)

# Mensaje a enviar
mensaje = "c0x150.5v20.3\n"

# Enviar mensaje
#ser.write(mensaje.encode())

print(mensaje)