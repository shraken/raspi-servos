#include <SPI.h>
#include <Servo.h>
#include <string.h>

#define MAX_BUF_LEN 255

Servo servo_horiz;
Servo servo_vert;

char buf[MAX_BUF_LEN + 1];
volatile byte pos;
volatile byte process_it;

// twelve servo objects can be created on most boards

void setup() {
  Serial.begin(115200);

  // turn on SPI in slave mode
  SPCR |= bit (SPE);

  // have to send on master in, *slave out*
  pinMode (MISO, OUTPUT);

  // get ready for an interrupt
  pos = 0;   // buffer empty
  process_it = false;

  // now turn on interrupts
  SPI.attachInterrupt();

  // attach the horizontal and vertical servo to pins
  servo_horiz.attach(8);
  servo_vert.attach(9);

  Serial.println("raspi_servo setup complete");
}

void servo_action(int id, int value)
{
  switch (id) {
    case 0:
      servo_horiz.write(value);
      break;

    case 1:
      servo_vert.write(value);
      break;
    
    default:
      break;
  }
}

void process() {
  char *command;
  int servo_id;
  int servo_value;
  int count = 1;
  
  Serial.println("raspi_servo process");
  command = strtok(buf, ":");

  while (command != NULL) {
    if ((count%2) == 1) {
      servo_id = atoi(command);
    } else {
      servo_value = atoi(command);

      Serial.print("raspi_servo setting servo id ");
      Serial.print(servo_id, DEC);
      Serial.print(" with value ");
      Serial.print(servo_value, DEC);
      Serial.println("");

      servo_action(servo_id, servo_value);
    }

    count++;
    command = strtok(NULL, ":");
  }
}

void dump_buffer()
{
  int i;

  for (i = 0; i < MAX_BUF_LEN; i++) {
    Serial.print(buf[i], DEC);
    Serial.print(":");
  }
  Serial.println("");
}

void loop() {
  int local_pos = 0;

  /*
  Serial.println("SPI buffer pos");
  Serial.print(pos, DEC);
  Serial.println("");
  Serial.print("process_it = ");
  Serial.print(process_it, DEC);
  Serial.println("");
  */
  
  if (process_it)
  {
    buf[pos] = 0;
    pos = 0;
    process_it = false;

    process();
  }  // end of flag set
}

// SPI interrupt routine
ISR (SPI_STC_vect)
{
  byte c = SPDR;  // grab byte from SPI Data Register
  
  // add to buffer if room
  if (pos < sizeof buf)
  {
    buf[pos++] = c;

    // example: newline means time to process buffer
    if (c == '\n')
      process_it = true;

  }  // end of room available
}  // end of interrupt routine SPI_STC_vect
