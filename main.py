#!/usr/bin/env python
import serial
import logging
from telegram.ext import Updater, CommandHandler, MessageHandler, Filters
from influxdb import InfluxDBClient
#####
import os
import datetime
import sys
import time
import subprocess
####

#Logging to Grafana
client = InfluxDBClient(host='192.168.0.183', port=8086, username='grafana', password='1954', database='home')
logging.basicConfig(format='%(asctime)s - %(name)s - %(levelname)s - %(message)s', level=logging.INFO)

logger = logging.getLogger(__name__)

publishedData = [0, 0] #Temperature + Humidity

def changeLED(x):
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser.flush()
    if x==1:
        ser.write("LED_ON\n".encode('utf-8'))
        print("Turning on LED...")
    elif x==0:
        ser.write("LED_OFF\n".encode('utf-8'))
        print("Turning off LED...")


def setAlarm(x):
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser.flush()
    if x==1:
        ser.write("ALARM_ON\n".encode('utf-8'))
        print("ALARM has been turned ON!")
    elif x==0:
        ser.write("ALARM_OFF\n".encode('utf-8'))
        print("ALARM has been turned OFF!")

def start(update, context):
    update.message.reply_text("Hi, I'm your IoT bot!\n\n Control lights with: \n\t/lighton & /lightoff \n\n Control alarm: \n\t/alarmon & /alarmoff")

def help(update, context):
    update.message.reply_text('Current commands: \n /lighton \n /lightoff \n /temp \n /alarmon \n /alarmoff')

def error(update, context):
    logger.warning('Update "%s" caused error "%s"', update, context.error)

def arduino(update, context):
    f=open("testfile.txt", "r")
    update.message.reply_text(f.read())
    f.close()

def light_on(update, context):
    changeLED(1)
    update.message.reply_text("The lights have been turned ON!")

def light_off(update, context):
    changeLED(0)
    update.message.reply_text("The lights have been turned OFF!")

def alarm_on(update, context):
    setAlarm(1)
    update.message.reply_text("You alarm system has been activated!")

def alarm_off(update, context):
    setAlarm(0)
    update.message.reply_text("You alarm system has been deactivated!")
	
def temp(update, context):
    update.message.reply_text("Temp: " + str(publishedData[0]) + "\nHum: " + str(publishedData[1]))


def main():
    # --- TELEGRAM BOT SECTION ---
    updater = Updater("2119154414:AAGDm4oNWSLkx6btm_qgJzB7wcj5reQfHWg", use_context=True)

    # Get the dispatcher to register handlers
    dp = updater.dispatcher

    # on different commands - answer in Telegram
    dp.add_handler(CommandHandler("start", start))
    dp.add_handler(CommandHandler("help", help))
    dp.add_handler(CommandHandler("arduino", arduino))
    dp.add_handler(CommandHandler("lighton", light_on))
    dp.add_handler(CommandHandler("lightoff", light_off))
    dp.add_handler(CommandHandler("alarmon", alarm_on))
    dp.add_handler(CommandHandler("alarmoff", alarm_off))
    dp.add_handler(CommandHandler("temp", temp))


    # log all errors
    dp.add_error_handler(error)

    # Start the Bot
    updater.start_polling()

    # Run the bot until you press Ctrl-C or the process receives SIGINT,
    # SIGTERM or SIGABRT. This should be used most of the time, since
    # start_polling() is non-blocking and will stop the bot gracefully.
    #updater.idle()

    # --- ARDUINO-RASPBERRY SERIAL COMMUNICATION SECTION --
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser.reset_input_buffer()

    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            if line == "MOTION_DETECTED":
                #Send msg using telegram bot
                updater.dispatcher.bot.sendMessage(chat_id='960135971', text='\uD83D\uDEA8 Someone has entered your house!')
                updater.dispatcher.bot.sendMessage(chat_id='183831737', text='\uD83D\uDEA8 Someone has entered your house!')

		# read the absolute path
                script_dir = os.path.dirname(__file__)
		# call the .sh to capture the image
                os.system('./webcam.sh')
		#get the date and time, set the date and time as a filename.
                currentdate = datetime.datetime.now().strftime("%Y-%m-%d_%H%M")
		# create the real path
                rel_path = currentdate +".jpg"
		#  join the absolute path and created file name
                abs_file_path = os.path.join(script_dir, rel_path)
                print(abs_file_path)
                updater.dispatcher.bot.send_photo(chat_id='960135971', photo=open(abs_file_path, 'rb'))
                updater.dispatcher.bot.send_photo(chat_id='183831737', photo=open(abs_file_path, 'rb'))
            else:
                f=open("testfile.txt", "w+")
                f.write(line)
                f.close()
                #Get
                first_chars = line[0:3]
                if first_chars == "HUM":
                    publishedData[1] = int(line[3:5])
                    json_hum = [
                        {
                            "measurement": "humidity",
                            "fields":{
                                "value": int(line[3:5])
			    }
                         }
                    ]
                    client.write_points(json_hum)
                elif first_chars == "TEM":
                    publishedData[0] = int(line[3:5])
                    json_temp = [
                        {
                            "measurement": "temperature",
                            "fields":{
                                "value": int(line[3:5])
                            }
                         }
                    ]
                    client.write_points(json_temp)
                elif first_chars == "HIC":
                    json_hic = [
                        {
                            "measurement": "heatindex",
                            "fields":{
                                "value": int(line[3:5])
                            }
                            }
                    ]
                    client.write_points(json_hic)
                else:
                    print(line)
if __name__ == '__main__':
    main()
