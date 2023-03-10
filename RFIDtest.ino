#include <SoftwareSerial.h>

#define rxPin 7
#define txPin 6
SoftwareSerial rfid = SoftwareSerial( rxPin, txPin );


int incomingByte = 0; // To store incoming serial data

void setup() {

  Serial.begin(115200); // Serial port for connection to host
  rfid.begin(115200); // Serial port for connection to RFID module
  Serial.println("RFID Reader Initialized");
}




void loop() {

byte i = 0;
byte val = 0;
byte checksum = 0;
byte bytesRead = 0;
byte tempByte = 0;
byte tagBytes[6]; // "Unique" tags are only 5 bytes but we need an extra byte for the checksum
char tagValue[10];

// Read from the RFID module. Because this connection uses SoftwareSerial
// there is no equivalent to the Serial.available() function, so at this
// point the program blocks while waiting for a value from the module
if((val = rfid.read()) == 2) { // Check for header
bytesRead = 0;
while (bytesRead < 12) { // Read 10 digit code + 2 digit checksum
val = rfid.read();

// Append the first 10 bytes (0 to 9) to the raw tag value
if (bytesRead < 10)
{
tagValue[bytesRead] = val;
}

// Check if this is a header or stop byte before the 10 digit reading is complete
if((val == 0x0D)||(val == 0x0A)||(val == 0x03)||(val == 0x02)) {
break; // Stop reading
}

// Ascii/Hex conversion:
if ((val >= '0') && (val <= '9')) {
val = val - '0';
}
else if ((val >= 'A') && (val <= 'F')) {
val = 10 + val - 'A';
}

// Every two hex-digits, add a byte to the code:
if (bytesRead & 1 == 1) {
// Make space for this hex-digit by shifting the previous digit 4 bits to the left
tagBytes[bytesRead >> 1] = (val | (tempByte << 4));

if (bytesRead >> 1 != 5) { // If we're at the checksum byte,
checksum ^= tagBytes[bytesRead >> 1]; // Calculate the checksum... (XOR)
};
} else {
tempByte = val; // Store the first hex digit first
};

bytesRead++; // Ready to read next digit
}


if (bytesRead == 12) { // 12 digit read is complete
tagValue[10] = '\0'; // Null-terminate the string

Serial.print("Tag read: ");
for (i=0; i<5; i++) {
// Add a leading 0 to pad out values below 16
if (tagBytes[i] < 16) {
Serial.print("0");
}
Serial.print(tagBytes[i], HEX);
}
Serial.println();

Serial.print("Checksum: ");
Serial.print(tagBytes[5], HEX);
Serial.println(tagBytes[5] == checksum ? " -- passed." : " -- error.");

// Show the raw tag value
Serial.print("VALUE: ");
Serial.println(tagValue);

Serial.println(); // Blank separator line in output
}

bytesRead = 0;
}
  
}
