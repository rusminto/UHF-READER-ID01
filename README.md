# UHF-READER-ID01
example of UHF READER ID01 DFRobot's implementation


There are three main function within this code :  
1. listen to response from command and read tag
2. read RFID's software version
3. write tag


### How to read tag ?
Just run this program on your arduino, and check is serial port correct.

### How to check software version ?
Insert command "version;" or "VERSION;" within ```arduino-cli monitor``` or your serial monitor

### How to write tag ?
![image](https://user-images.githubusercontent.com/43722553/147645736-2f59181a-f858-4d4a-8fb6-6b3c0a1216a4.png)  

- Put your tag on rfid.
- Insert command "write_tag:1a1a1a1a;" within ```arduino-cli monitor``` or your serial monitor, where "1a1a1a1a" is your new tag id.


References:
- https://www.scadacore.com/tools/programming-calculators/online-checksum-calculator/
- https://wiki.dfrobot.com/UHF_RFID_MODULE-UART__SKU_TEL0082_
- https://github.com/Arduinolibrary/DFRobot_ID10_UHF_RFID_Reader/blob/master/ID01%20UHF%20RFID%20Reader%20user%20manual.pdf?raw=true
