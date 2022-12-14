#pragma GCC optimize ("-Ofast")
#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)
typedef uint16_t uintDiff;
#else
typedef uint32_t uintDiff;
#endif

#include "uniqueID.h"
#include "sha1.h"


String lastblockhash = "";
String newblockhash = "";
String DUCOID = "";
uintDiff difficulty = 0;
uintDiff ducos1result = 0;

const uint16_t job_maxsize = 104;  
uint8_t job[job_maxsize];
Sha1Wrapper Sha1_base;

void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  DUCOID = get_DUCOID();
  Serial.begin(115200);
  Serial.setTimeout(10000);
  while (!Serial)
    ; 
  Serial.flush();
}


uintDiff ducos1a(String lastblockhash, String newblockhash,
                 uintDiff difficulty) {
  newblockhash.toUpperCase();
  const char *c = newblockhash.c_str();
  uint8_t final_len = newblockhash.length() >> 1;
  for (uint8_t i = 0, j = 0; j < final_len; i += 2, j++)
    job[j] = ((((c[i] & 0x1F) + 9) % 25) << 4) + ((c[i + 1] & 0x1F) + 9) % 25;

    
  #if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)
    
    if (difficulty > 655) return 0;
  #endif
  Sha1_base.init();
  Sha1_base.print(lastblockhash);
  for (uintDiff ducos1res = 0; ducos1res < difficulty * 100 + 1; ducos1res++) {
    Sha1 = Sha1_base;
    Sha1.print(String(ducos1res));
    
    uint8_t *hash_bytes = Sha1.result();
    if (memcmp(hash_bytes, job, SHA1_HASH_LEN * sizeof(char)) == 0) {
      
      return ducos1res;
    }
  }
  return 0;
}

String get_DUCOID() {
  String ID = "DUCOID";
  char buff[4];
  for (size_t i = 0; i < 8; i++) {
    sprintf(buff, "%02X", (uint8_t)UniqueID8[i]);
    ID += buff;
  }
  return ID;
}

void loop() {
  
  if (Serial.available() > 0) {
    memset(job, 0, job_maxsize);
    lastblockhash = Serial.readStringUntil(',');
    newblockhash = Serial.readStringUntil(',');
    difficulty = strtoul(Serial.readStringUntil(',').c_str(), NULL, 10);
    while (Serial.available()) Serial.read();
    #if defined(ARDUINO_ARCH_AVR)
        PORTB = PORTB | B00100000;
    #else
        digitalWrite(LED_BUILTIN, LOW);
    #endif
    uint32_t startTime = micros();
    ducos1result = ducos1a(lastblockhash, newblockhash, difficulty);
    uint32_t elapsedTime = micros() - startTime;
    #if defined(ARDUINO_ARCH_AVR)
        PORTB = PORTB & B11011111;
    #else
        digitalWrite(LED_BUILTIN, HIGH);
    #endif
    while (Serial.available()) Serial.read();
    Serial.print(String(ducos1result, 2) 
                 + "," 
                 + String(elapsedTime, 2) 
                 + "," 
                 + String(DUCOID) 
                 + "\n");
  }
}
