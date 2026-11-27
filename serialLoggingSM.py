#!/usr/bin/env python3
""

import csv
import os

import serial

output_csv_filename = "datos.csv"


def main():
    """Launcher"""
    filename = os.path.basename(__file__)
    print(f"Start of {filename} \n")

    # USB port for MCU
    ser = serial.Serial(port="/dev/ttyACM0", baudrate=115200)
    # Clear serial I/O
    ser.flush()

    # check if CSV file exist. If not, then create it
    file_exists = os.path.exists(output_csv_filename)

    if file_exists:
        print("CSV file already exists, so appending to file")
        data_file = open(output_csv_filename, "a+", newline="", encoding="UTF8")
        csv_writer = csv.writer(data_file)
    else:
        print("CSV file does not exist, so creating new file")
        data_file = open(output_csv_filename, "w", newline="", encoding="UTF8")
        csv_writer = csv.writer(data_file)
        header = [
            "sample_num",
            "timestamp_ms",
            "delta_pressure_Pa",
            "flow_L_s",
            "temperature_C",
            "humidity_RH",
            "pressure_atm",
        ]
        csv_writer.writerow(header)

    try:
        # obtain data from device over USB
        print("Collecting data from device. Press CTR-C to terminate program.")
        while 1:
            data_read = ser.readline()
            data_str = data_read.decode("utf-8", "ignore").strip()
            if data_str:
                data_row = data_str.split(",")
                print(f"Data Received: {data_row}")
                # write data to CSV file
                csv_writer.writerow(data_row)

        data_file.close()

    except KeyboardInterrupt:
        print("Keyboard interrupt occurred")
        data_file.close()
        ser.close()
        print("End of", filename)


if __name__ == "__main__":
    main()
