#include <stdio.h>
#include <stdint.h>
#include "eecs388_lib.h"
#include "metal/i2c.h"

struct metal_i2c *i2c;
uint8_t bufWrite[5];
uint8_t bufRead[1];

// The entire setup sequence
// Do not modify the following function!

void set_up_I2C(){
    uint8_t oldMode;
    uint8_t newMode;
    _Bool success;

    i2c = metal_i2c_get_device(0);

    if(i2c == NULL)
        printf("Connection Unsuccessful\n");
    else
        printf("Connection Successful\n");
    
    //Setup Sequence
    metal_i2c_init(i2c,I2C_BAUDRATE,METAL_I2C_MASTER);

    bufWrite[0] = PCA9685_MODE1;
    bufWrite[1] = MODE1_RESTART;
    printf("%d\n",bufWrite[0]);

    success = metal_i2c_write(i2c,PCA9685_I2C_ADDRESS,2,bufWrite,METAL_I2C_STOP_DISABLE);//reset
    delay(100);
    printf("resetting PCA9685 control 1\n");

    //Initial Read of control 1
    bufWrite[0] = PCA9685_MODE1;//Address
    success = metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,1,bufRead,1);//initial read
    printf("Read success: %d and control value is: %d\n", success, bufWrite[0]);
    
    //Configuring Control 1
    oldMode = bufRead[0];
    newMode = (oldMode & ~MODE1_RESTART) | MODE1_SLEEP;
    printf("sleep setting is %d\n", newMode);
    bufWrite[0] = PCA9685_MODE1;//address
    bufWrite[1] = newMode;//writing to register
    success = metal_i2c_write(i2c,PCA9685_I2C_ADDRESS,2,bufWrite,METAL_I2C_STOP_DISABLE);//sleep
    bufWrite[0] = PCA9685_PRESCALE;//Setting PWM prescale
    bufWrite[1] = 0x79;
    success = metal_i2c_write(i2c,PCA9685_I2C_ADDRESS,2,bufWrite,METAL_I2C_STOP_DISABLE);//sets prescale
    bufWrite[0] = PCA9685_MODE1;
    bufWrite[1] = 0x01 | MODE1_AI | MODE1_RESTART;
    printf("on setting is %d\n", bufWrite[1]);
    success = metal_i2c_write(i2c,PCA9685_I2C_ADDRESS,2,bufWrite,METAL_I2C_STOP_DISABLE);//awake
    delay(100);
    printf("Setting the control register\n");
    bufWrite[0] = PCA9685_MODE1;
    success = metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,1,bufRead,1);//initial read
    printf("Set register is %d\n",bufRead[0]);

} 


void breakup(int bigNum, uint8_t* low, uint8_t* high){
    *low = bigNum & 0x00FF;
    *high = bigNum >> 8;
}

void steering(int angle){
    int cycle = getServoCycle(angle);

    bufWrite[0] = PCA9685_LED0_ON_L + 4; // or add 4 for LED 1
    bufWrite[1] = 0;
    bufWrite[2] = 0;

    breakup(cycle, &bufWrite[3], &bufWrite[4]);
    printf(cycle);
    metal_i2c_transfer(i2c, PCA9685_I2C_ADDRESS, bufWrite, 5, bufRead, 1);
}

void stopMotor(){
    i2c = metal_i2c_get_device(0);

    bufWrite[0] = PCA9685_LED0_ON_L;
    bufWrite[1] = 0;
    bufWrite[2] = 0;

    breakup(280, &bufWrite[3], &bufWrite[4]);
    metal_i2c_transfer(i2c, PCA9685_I2C_ADDRESS, bufWrite, 5, bufRead, 1); 
}

void driveForward(uint8_t speedFlag){
    /*
        Write Task 4 code here
    */
}

void driveReverse(uint8_t speedFlag){
    /*
        Write task 5 code here
    */
}


int main()
{
    set_up_I2C();
    
    delay(2000);
    steering(45); // Test added by yara
    delay(2000);
    steering(-45); // Test added by yara
    delay(2000);
    steering(0); // Test added by yara
    /*
        Add function calls here to complete task 6
    */
    stopMotor();
}


/* Defining the breakup function */
/*
    -Task 1: breaking 12 bit into two 8-bit
    Define the function created that recieves a 12 bit number,
    0 to 4096 and breaks it up into two 8 bit numbers.

    -Assign these values to a referenced value handed into
    the function. 

    Example of `breakup` being called from main: 

    uint8_t variable1;
    uint8_t variable2;
    int numToBreakup = 2000;
    breakup(numToBreakup, &variable1, &variable2);

    // After breakup returns, the three variable values
    // numToBreakup == 2000 (not modified)
    // variable1 == the least significant byte of 2000       == 1101 0000
    // variable2 == the next least significant byte of 2000  == 0000 0111
*/    


/* Changing Steering Heading */
/*
    -Task 2: using getServoCycle(), bufWrite, bufRead, 
    breakup(), and and metal_i2c_transfer(), implement 
    the function defined above to control the servo
    by sending it an angle ranging from -45 to 45.

    -Use the getServoCycle function to get the value to 
    breakup. 

    ex: 
    int valToBreak = getServoCycle(45);
    // getServoCycle returns the PWM mapping of steering angle -> duty cycle
    // the above line sets valToBreak to 355
    
    -Note: the motor's speed controller is either 
    LED0 or LED1 depending on where its plugged into 
    the board. 
        If it's LED0, use PCA9685_LED0_ON_L
        If it's LED1, use PCA9685_LED1_ON_L

    -Example use from main function:
    steering(0); -> driving angle forward
*/


/* Motor config/stop. This will cause a second beep upon completion */
/*
    -Task 3: using bufWrite, bufRead, breakup(), and
    and metal_i2c_transfer(), implement the function made
    above. This function configures the motor by 
    writing a value of 280 to the motor.

    -After writing 280, include a 2 second delay in order to calibrate

    -Note: the motor's speed controller is either 
    LED0 or LED1 depending on where its plugged into 
    the board. 
        If it's LED0, use PCA9685_LED0_ON_L
        If it's LED1, use PCA9685_LED1_ON_L

    -Example use from main function:
    stopMotor();
*/


/*
############################################################
    ATTENTION: The following section will cause the        
    wheels to move. Confirm that the robot is              
    propped up to avoid it driving away, as well as         
    that nothing is touching the wheels and can get 
    caught in them.

    If anything goes wrong, unplug the hifive board from
    the computer to stop the motors from moving.
    
    Avoid sticking your hand inside the 
    car while its wheels are spinning.
#############################################################
*/


/* Motor Forward */
/*
    -Task 4: using bufWrite, bufRead, breakup(), and
    and metal_i2c_transfer(), implement the function
    made above to drive the motor forward. The given
    speedFlag will alter the motor speed as follows:
    
    speedFlag = 1 -> value to breakup = 303 
    speedFlag = 2 -> value to breakup = 305(Optional)
    speedFlag = 3 -> value to breakup = 307(Optional)

    -note 1: the motor's speed controller is either 
    LED0 or LED1 depending on where its plugged into 
    the board.

    -Example use from main function:
    driveForward(1);
*/


/* Motor Reverse */
/*
    -Task 5: using bufWrite, bufRead, breakup(), and
    and metal_i2c_transfer(), implement the function
    made above to Drive the motor backward. The given
    speedFlag will alter the motor speed as follows:
    
    speedFlag = 1 -> value to breakup = 257 
    speedFlag = 2 -> value to breakup = 255(Optional)
    speedFlag = 3 -> value to breakup = 253(Optional)

    -note 1: the motor's speed controller is either 
    LED0 or LED1 depending on where its plugged into 
    the board.

    -Example use from main function:
    driveReverse(1);
*/


/* Fully Controlling the PCA9685 */
/*
    -Task 6: using previously defined functions, 
    perform the following sequence of actions
    
    -Configure the motors (wait for 2 seconds)
    -Set the steering heading to 0 degrees 
    -Drive forward (wait for 2 seconds)
    -Change the steering heading to 20 degrees (wait for 2 seconds)
    -Stop the motor (wait for 2 seconds)
    -Drive forward (wait for 2 seconds)
    -Set steering heading to 0 degrees (wait for 2 seconds)
    -Stop the motor
*/
