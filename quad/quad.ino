//neutral accelerometer/gyro positions
#define X_ZERO 332
#define Y_ZERO 324
#define Z_ZERO 396
#define PITCH_ZERO 249
#define ROLL_ZERO 249
#define YAW_ZERO 248

#define GYRO_CON 1.47
#define ACCEL_CON 0.93

#define TIME_CON 0.02
#define SEN_CON 0.95

//motor speed vars
int speeds[4];

//gyro inputs - current tilt vars
float pitch, roll, yaw;
int pitchzero, rollzero;
//accelerometer inputs - current acceleration vars
float xin, yin, zin;

//human inputs - control info vars
float pitchin, rollin, yawin, zhuman;

//random other vars
float xaverage=0, yaverage=0;
int y=0;
int blah;

//proportionality constants
float p=2.5; // P proportionality constant
float d=0.5; // D proportionality constant

void setup() {
    zhuman=0;
    rollin=0;
    Serial.begin(9600);
    for(int x=6; x<10; x++) {
        pinMode(x, OUTPUT);
    }

    //send upper bound for human inputs to the motor speed controllers
    for(int x=6; x<10; x++) {
        pulsout(x,2000);
    }
    delay(5000);

    //get zeros for pitch and roll human inputs
    for(int x=0; x<10; x++) {
        y=y+analogRead(3);
    }
    pitchzero=y/10;
    y=0;
    for(int x=0; x<10; x++) {
        y=y+analogRead(4);
    }
    rollzero=y/10;
}

void loop () {
    //accelerometer and gyro inputs ranged -232 to 232?
    xin=(analogRead(0)-X_ZERO)*ACCEL_CON;
    yin=(analogRead(1)-Y_ZERO)*ACCEL_CON;
    zin=(analogRead(2)-Z_ZERO)*ACCEL_CON;
    pitch=(pitchzero-analogRead(3))*GYRO_CON;
    roll=(rollzero-analogRead(4))*GYRO_CON;
    yaw=(analogRead(5)-YAW_ZERO)*GYRO_CON;

    //get human inputs through radio here range of -30 to 30 except for zhuman which has an ideal range of 1000-2000, only 2 pulses per loop
    if(blah==0) {
        yawin=0.06*((signed int) pulseIn(2,HIGH)-1500);
        pitchin=0.06*((signed int) pulseIn(3,HIGH)-1500);
        blah=1;
    }
    else {
        zhuman=(signed int) pulseIn(4,HIGH);
        rollin=0.06*((signed int) pulseIn(5,HIGH)-1400); //1400 instead of 1500 is to correct for the underpowered motor #4 by trimming it in code
        blah=0;
    }

    //averaging, etc.
    xaverage= SEN_CON *( xaverage + TIME_CON * pitch) + ( 1 - SEN_CON ) * xin;
    yaverage= SEN_CON *( yaverage + TIME_CON * roll) + ( 1 - SEN_CON ) * yin;

    //calculate the motor speeds
    if(zhuman<1150) {
        for(int x=0; x<4; x++) {
            speeds[x]=zhuman;
        }
    }
    else {
        if(zhuman > 1450) {
            zhuman = 1450;
        }
        speeds[0] = zhuman - p*(xaverage - pitchin) - p*(yawin) - d*pitch;
        speeds[1] = zhuman - p*(pitchin - xaverage) - p*(yawin) + d*pitch;
        speeds[2] = zhuman - p*(yaverage - rollin) + p*(yawin) - d*roll;
        speeds[3] = zhuman - p*(rollin - yaverage) + p*(yawin) + d*roll;
    }
    //set the upper and lower bounds for motor speeds (1000=no speed, 1600=upper speed limit, 2000=maximum possible speed)
    for(int x=0; x<4; x++) {
        //speed limit between 1000 and 1600
        if(speeds[x]<1000) {
            speeds[x]=1000;
        }
        if(speeds[x]>1600) {
            speeds[x]=1600;
        }
    }

    //pulsouts to motor speed controllers
    for(int x=0; x<4; x++) {
        pulsout(x+6,speeds[x]);
    }
}
void pulsout (int pin, int duration) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(duration);
    digitalWrite(pin, LOW);
}
