# -*- coding: utf-8 -*-
"""
Created on Tue Mar 10 14:12:55 2020

@author: qinzhena
"""

import time
import serial

from datetime import datetime

#存档路径
path = r"C:\Users\sunqy\Desktop\data.txt"
#协议长度
data_length  = 9
#串口号
ser_num = 10 
ser_num = "com" + str(ser_num)
#波特率
Baud = 9600

ser = serial.Serial(ser_num, Baud, timeout=None)
flags = []

# 发送传感器的测温指令
cur_cmd = bytes([0xF4, 0xF5, 0x03, 0x02, 0x00, 0xEE])
ser.write(cur_cmd)

while True:
    file = open(path, 'a+')
    # 烟机状态
    test_flag = input("烟机状态（0关，1开）：")

    if test_flag == "0":
        # 30内
        for counter in range(30 * 4):
            time.sleep(0.24)
            current_time = datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]
            temp_data = []
            for i in range(data_length):
                data = ser.read()
                data_int = int.from_bytes(data, byteorder='big')
                temp_data.append(data_int)
            
            log_msg = current_time + " " + temp_data + " " + test_flag
            file.write(log_msg + '\n')

    elif test_flag == "1":
        while True:
            current_time = datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]
            temp_data = []
            for i in range(data_length):
                data = ser.read()
                data_int = int.from_bytes(data, byteorder='big')
                temp_data.append(data_int)
            
            log_msg = current_time + " " + temp_data + " " + test_flag
            file.write(log_msg + '\n')
            if input() == "1":
                break;
    else:
        break;

    file.close()
