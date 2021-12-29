unsigned char StopReadCode[5] = {0xa0,0x03,0xA8,0x00,0xB5};//Stop reading the label code
unsigned char ResetCode[5]={0xa0,0x03,0x65,0x00,0xf8};//Reset code
unsigned char StopReadCodeCB[6]={0xe4,0x04,0xa8,0x00,0x00,0x74};//Stop reading code success and return the value
unsigned char ResetCodeCB[6]={0xe4,0x04,0x65,0x00,0x00,0xb3};//Reset code success and return the value
unsigned char ResetBuzzer[6]={0xa0, 0x04, 0xb0, 0x00, 0x00, 0xac}; //Close beep sound when reading tag
unsigned char SetBaudRate[6]={0xa0, 0x04, 0xa9, 0x00, 0x00, 0xb3}; //Set baud rate to 9600

byte GetVersion[5]={0xa0, 0x03, 0x6a, 0x00}; //Get software version
byte WriteTag[10]={0xA0, 0x08, 0x9C, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00}; //Write 2 words(1234 5678)in EPC area address 4,5

byte data;
byte lastData;


int SKIP_DURATION = 10;
unsigned long lastDetected = 0;
bool firstSkip = true;

String command = "";
String lastCharOnCommand = "";


// CheckSum8 2s Complement
// https://stackoverflow.com/questions/12238528/checksum-calculation-two-s-complement-sum-of-all-bytes#:~:text=The%20checksum%20consists%20of%20a,most%20significant%20bit%20is%20ignored.
byte * generateCheckSum(byte *rawData, int len){
	len--;
	int sum = 0;

	for(int i = 0; i < len; i++){
		sum += rawData[i];
	}

	sum %= 256;

	byte ch = sum;

	byte twoscompl = (~ch) + 1;
	rawData[len] = twoscompl;

	return rawData;
	
}

//string to byte
byte stringToByte(char *s) 
{
  int x;
  for(;;) {
    char c = *s;
    if (c >= '0' && c <= '9') {
      x *= 16;
      x += c - '0'; 
    }
    else if (c >= 'A' && c <= 'F') {
      x *= 16;
      x += (c - 'A') + 10; 
    }
    else break;
    s++;
  }
  return (byte) x;
}

void setup(){
	Serial1.begin(9600); // PIN 18 as tx, PIN 19 as rx
	Serial.begin(9600);

	Serial1.write(SetBaudRate, 5);
	Serial1.write(ResetBuzzer, 6);
}
 

void loop(){

	// create new line after some time to distinguish between message from rfid
	if(millis() > lastDetected + SKIP_DURATION && firstSkip){
		Serial.println("");
		firstSkip = false;
	}
	
	// wait till message rfid existed
	while(Serial1.available()){
		data=Serial1.read();

		Serial.print(data, HEX);
		Serial.print(" ");

		// only when read tag
		/*if(data == 0xFF && lastData == 0xEC){
			Serial.println("");
		}*/

		lastDetected = millis();
		firstSkip = true;
		//lastData = data;
	}

	// wait till message from computer existed
	while(Serial.available()){
		data=Serial.read();
		lastCharOnCommand = (char) data;
		command += lastCharOnCommand;
	}

	// check and run command
	if(lastCharOnCommand == ";"){
		// remove new line and trailing whitespace
		command.trim();
		command.replace("\r\n", ""); // for windows
		command.replace("\n", ""); // for unix

		command.toUpperCase();

		Serial.print("Command : ");
		Serial.println(command);

		// check rfid software version
		if(command == "VERSION;"){
			generateCheckSum(GetVersion, sizeof(GetVersion));
			Serial1.write(GetVersion, sizeof(GetVersion));
		}

		// write tag based on inputed value
		// new tag's id have to be hexa like 1a1a1a1a or 000000
		if(command.substring(0, 10) == "WRITE_TAG:" && command.substring(10, 18).length() == 8){

			// get new tag's id
			String rawNewTag;
			char rawNewTagInChar[3];
			 // convert char to hexa, by split them to 4 part
			for(int i = 0; i < 4; i++){
				// convert string to char
				rawNewTag = command.substring(10 + 2*i, 12 + 2*i);
				rawNewTag.toCharArray(rawNewTagInChar, 3);


				// convert char to byte
				WriteTag[5 + 1*i] = stringToByte(rawNewTagInChar);
			}

			
			generateCheckSum(WriteTag, sizeof(WriteTag));
			Serial1.write(WriteTag, sizeof(WriteTag));

		}

		command = "";
		lastCharOnCommand = "";
	}
}

