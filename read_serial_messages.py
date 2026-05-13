import serial
import time
import serial.tools.list_ports

ports = serial.tools.list_ports.comports()
for port in ports:
    print(port.device)


# Configura el puerto serie
ser = serial.Serial(
    port='/dev/ttyACM0',  # Reemplaza con el puerto de tu microcontrolador (en Linux puede ser algo como '/dev/ttyUSB0')
    baudrate=115200,  # Configura la velocidad en baudios según tu microcontrolador
    timeout=2  # Tiempo de espera para lectura en segundos
)

#try:
    # Leer datos continuamente
#    while True:
#        if ser.in_waiting > 0:  # Comprueba si hay datos disponibles
#            data = ser.readline().decode('utf-8').strip()  # Lee una línea completa
#            print(f'Datos recibidos: {data}')
# except KeyboardInterrupt:
#    print("Saliendo del programa...")
# finally:
#    ser.close()  # Asegúrate de cerrar el puerto serie

while True:
    ser.write(b'cnpos0.00vel0.00\n')
    time.sleep(0.1)
    data = ser.readline().decode('utf-8') # Lee una línea completa
    print(f'Datos recibidos: {data}')
    time.sleep(0.1)

