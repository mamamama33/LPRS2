  typedef enum
  {
    HIGH_POWER,
    LOW_POWER
  } powerMode;
  enum
  {
    ON = 0,
    OFF = 1
  };

  /* Configuration structure for LORA device. */
  struct
  {
      powerMode power = LOW_POWER;
      uint8_t HighPowerAmpEna = 6; //
      long FREQUENCY = 480E6;     // LoRa frequency.
      int MAGIC_NUMBER = 0X56;    // LoRa magic number.
    
  } LoRaConfig;
  struct{

      int ss=10;
      int rst=15;
      int dio0=16;

  }Atmega;

  //DIO0 na atmega je PD2 fizicki 4. pin a u ARDUINU 2
  //NSS  na atmega je PB2 fizicki 16. pin a u ARDUINU 10
  //RST na atemga je PD5 a fizicki pin a ARDUINU 5