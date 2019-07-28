#import serial               #import serial pacakge
from time import sleep
import webbrowser           #import package for opening link in browser
import sys                  #import system package
import requests
from bs4 import BeautifulSoup
import RPi.GPIO as GPIO


def GPS_Info():
    global NMEA_buff
    global lat_in_degrees
    global long_in_degrees
    nmea_time = []
    nmea_latitude = []
    nmea_longitude = []
    nmea_time = NMEA_buff[0]                    #extract time from GPGGA string
    nmea_latitude = NMEA_buff[1]                #extract latitude from GPGGA string
    nmea_longitude = NMEA_buff[3]               #extract longitude from GPGGA string
    
    print("NMEA Time: ", nmea_time,'\n')
    print ("NMEA Latitude:", nmea_latitude,"NMEA Longitude:", nmea_longitude,'\n')
    
    lat = float(nmea_latitude)                  #convert string into float for calculation
    longi = float(nmea_longitude)               #convertr string into float for calculation
    
    lat_in_degrees = convert_to_degrees(lat)    #get latitude in degree decimal format
    long_in_degrees = convert_to_degrees(longi) #get longitude in degree decimal format


#convert raw NMEA string into degree decimal format   
def convert_to_degrees(raw_value):
    decimal_value = raw_value/100.00
    degrees = int(decimal_value)
    mm_mmmm = (decimal_value - int(decimal_value))/0.6
    position = degrees + mm_mmmm
    position = "%.4f" %(position)
    return position
    

GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.OUT)
GPIO.setup(27, GPIO.OUT)
GPIO.output(17,GPIO.LOW)
GPIO.output(27,GPIO.LOW)
        
gpgga_info = "$GPGGA,"
ser = serial.Serial ("/dev/ttyS0",9600)              #Open port with baud rate
GPGGA_buffer = 0
NMEA_buff = 0
lat_in_degrees = 0
long_in_degrees = 0


while True:
    received_data = (str)(ser.readline())                   #read NMEA string received
    GPGGA_data_available = received_data.find(gpgga_info)   #check for NMEA GPGGA string                 

    if (GPGGA_data_available>0):
            GPGGA_buffer = received_data.split("$GPGGA,",1)[1]  #store data coming after "$GPGGA," string 
            NMEA_buff = (GPGGA_buffer.split(','))               #store comma separated data in buffer
            GPS_Info()                                          #get time, latitude, longitude
 
    print("lat in degrees:", lat_in_degrees," long in degree: ", long_in_degrees, '\n')
            
    lat = 76.61751
    lng = 9.31477
    N = 0.001
    a = lat - (0.009 * N)
    b = lat + (0.009 * N)
    c = lng - (0.009 * N)
    d = lng + (0.009 * N)

    r = requests.get('http://www.overpass-api.de/api/xapi?*[maxspeed=*][bbox={},{},{},{}]'.format(a,c,b,d) )

    y=BeautifulSoup(r.text)

    for i in  y.findAll("way")[0].findAll("tag"):

        if i["k"] == "maxspeed":

            limit = i["v"]
            print ("Maximum Permissible Speed is {}".format(i["v"]))

    print (limit)

    if (limit==20):
        GPIO.output(17,GPIO.LOW)
        GPIO.output(27,GPIO.HIGH)

    if (limit==30):
        GPIO.output(17,GPIO.HIGH)
        GPIO.output(27,GPIO.LOW)

    if (limit==40):
        GPIO.output(17,GPIO.HIGH)
        GPIO.output(27,GPIO.HIGH)
