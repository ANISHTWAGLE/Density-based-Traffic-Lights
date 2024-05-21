import subprocess
import shlex
import serial
import threading

def read_output(process, ser):
    while True:
        line = process.stdout.readline().strip()
        if not line:
            break
        if ('2' in line or '3' in line) and 'cup' in line:
            ser.write(b'2')  # Send '2' to Arduino for 2 or 3 persons
        elif 'cup' in line:
            ser.write(b'1')  # Send '1' to Arduino for 1 person
        else:
            ser.write(b'0')  # Send '0' to Arduino if no person detected

def start_process(command, ser):
    process = subprocess.Popen(shlex.split(command), stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True, shell=True)
    read_thread = threading.Thread(target=read_output, args=(process, ser))
    read_thread.start()
    return process, read_thread

# Open serial port
ser = serial.Serial('COM3', 9600)  # Adjust the port and baud rate accordingly

try:
    process = None
    read_thread = None
    running = False

    while True:
        user_input = input("Press 'b' to start process, 'a' to terminate, or 'q' to quit: ").strip().lower()

        if user_input == 'b':
            if not running:
                process, read_thread = start_process('python detect.py --weights yolov5x.pt --source https://10.0.66.240:4343/video', ser)
                running = True
                print("Process started.")

        elif user_input == 'a':
            if running and process is not None:
                process.terminate()
                read_thread.join()  # Wait for read thread to finish
                process = None
                read_thread = None
                running = False
                print("Process terminated.")

        elif user_input == 'q':
            break

        if process and process.poll() is not None:
            print("Process finished.")
            running = False

except Exception as e:
    print("An error occurred:", e)

finally:
    ser.close()  # Close serial port when done
