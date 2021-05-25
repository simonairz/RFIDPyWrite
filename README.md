# RFIDPyWrite
Simonair Technologies RFID Communication Standards Between Python and Arduino To Write Data to  1k MIFARE Card Which Contains Information About Students to Help: Communications, Keep Standards Between Platforms, to Create a Data Structure That is Easily Expandable, to Create Addition Places for Better Interoperability, and to Show How Data is Organised in the 1k MIFARE Card; To Help Organize the Data About Students to Help With Readability and to Shorten the Processes of Learning the Code and Data Structure.

Aka Simonair RFID Communication Standard

By: Simon Airam Reinoso Navarro
Memory organization(sector number, block number)
//Access level and grade level
[[1, 2]
//First name | number of bytes = 48, characters = 48
[0, 1, 2]
//Last name | number of bytes = 48, characters = 48
[0, 1, 2]
//ID number | number of bytes = 48, characters = 48
[0, 1, 2]

Send order
sector
block address
access level
grade level
first name
last name
id number

How is each piece of data sent
first a data request is sent from the arduino
Serial.print(“Write request”)
Data is then read from python code
The python code then sends the data that needs to be sent which is shown from the send order above


