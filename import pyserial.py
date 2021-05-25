import serial
import codecs



FIRST_NAME = "Simon Airam"
LAST_NAME = "Navarro"
ID_NUMBER = "221188"
GRADE_LEVEL = "11"
ACCESS_LEVEL = "1"

FullMessagePreEncoded = [ACCESS_LEVEL, GRADE_LEVEL, FIRST_NAME, LAST_NAME, ID_NUMBER ]


#A test variable, Idk why I left it in here but just in case 
START_MESSAGE = "Start".encode('utf-8')


#Defines the serial port for communications between the arduino and the laptop
SerialPort = serial.Serial(port = "COM3", baudrate=115200, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)

#Its the main method, search it up
def main():
    
    
    #test code
    # HEX_FIRST_NAME_NP = FIRST_NAME.encode('utf-8').hex()
    # index = 0
    # HEX_FINAL = ""
    # while index < len(HEX_FIRST_NAME_NP):
    #     HEX_FINAL += "0x" + HEX_FIRST_NAME_NP[index:index+2] + " "
    #     index += 2

    # print("not processed hex - " + HEX_FIRST_NAME_NP)
    # print("processed - " + HEX_FINAL)

    print( encodeCombined(FullMessagePreEncoded))    

#runs code in a loop
def loop():
    while(1):
        # Wait until there is data waiting in the serial buffer
        if(serialPort.in_waiting > 0):

            # Read data out of the buffer until a carraige return / new line is found
            serialString = serialPort.readline()

            # Print the contents of the serial data
            print(serialString.decode('Ascii'))

            # Tell the device connected over the serial port that we recevied the data!
            # The b at the beginning is used to indicate bytes!
            serialPort.write(b"Thank you for sending data \r\n")

    
#encodes the string into hex
def encodeStrToHex(str):
    encodedString = str.encode('utf-8').hex()
    index = 0
    finalResult = ""
    while index < len(encodedString):
        finalResult += "0x" + encodedString[index:index+2] + ", "
        index += 2
    return finalResult

#check for a write request from the arduino, returns true if arduino request for raw data
def checkForWrite():
    if(serialPort.in_waiting > 12):
        if(serialPort.ReadLine() == "Write Request"):
            return True

#Encodes an entire array into a string ASCII
def encodeCombined(array):
    fullEncodedMessage = ""
    index = 0
    for x in array:
        #puts in an ASCII start text
        fullEncodedMessage += "0x02, "

        #puts the encoded message for each of the item in the array
        fullEncodedMessage += encodeStrToHex(x)

        #puts in an ASCII end text
        fullEncodedMessage += "0x03, "


        
        #Adds in spaces where need be and formats it when need be
        if(index == 1 or index == 2):
            for x in range(0, 16 - len(array)):
                fullEncodedMessage += "0x20, "
        elif(index == 3):
            for x in range(0, 48 - len(array)):
                fullEncodedMessage += "0x20, "
        elif(index == 4):
            for x in range(0, 47 - len(array)):
                fullEncodedMessage += "0x20, "
            fullEncodedMessage += "0x20 "

        print(index)
        index+=1
    return fullEncodedMessage


main()