/* 
 * File:   parallax_board.h
 * Author: Haikal Pribadi (haikal.pribadi@gmail.com)
 *
 * Created on February 19, 2012, 5:34 PM
 */

#ifndef _PARALLAXBOARD_H
#define	_PARALLAXBOARD_H
//#include <stdlib.h>
//#include <stdio.h>
//#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string>
#include <map>
#include "parallax_eddie_robot/Ping.h"
#include "parallax_eddie_robot/ADC.h"

class ParallaxBoard {
public:
    ParallaxBoard(std::string port="/dev/ttyUSB0");
    ParallaxBoard(const ParallaxBoard& orig);
    virtual ~ParallaxBoard();

    //=======================================//
    //PARALLAX EDDIE CONTROL BOARD ATTRIBUTES//
    //=======================================//

    //Number of potential GPIO pins available: 10
    const int GPIO_COUNT;

    //Number of ADC pins availabe: 8
    const int ADC_PIN_COUNT;

    //Number of potential digital pins available: 10
    const int DIGITAL_PIN_COUNT;

    //Number of switchable power ports: 3
    const int AUXILIARY_POWER_RELAY_COUNT;

    //Serial port buffer size: 256
    const int PARALLAX_MAX_BUFFER;

    //There are encoders for each wheel: 2
    const int ENCODER_COUNT;

    //FW returns values representing 1/892.v for ADC pins: 1/2819.2
    const double ADC_VOLTAGE_MULTIPLIER;

    //FW returns a battery voltage diveded: 3.21
    const double BATTERY_VOLTAGE_DIVIDER;

    //Parallax motor power levels go from -128 (full reverse) to 127 (full forward): 0
    const signed char MOTOR_POWER_STOP;

    //Parallax motor power levels go from -128 (full reverse) to 127 (full forward): 127
    const signed char MOTOR_POWER_MAX_FORWARD;

    //Parallax motor power levels go from -128 (full reverse) to 127 (full forward): -127
    //But -128 is clipped to -127 to maintain symmetry
    const signed char MOTOR_POWER_MAX_REVERSE;

    //3.3v Solid State Relay is located on GPIO pin 11: 16
    const unsigned char RELAY_33V_PIN_NUMBER;

    //5v Solid State Relay is located on GPIO pin 12: 17
    const unsigned char RELAY_5V_PIN_NUMBER;

    //12v Solid State Relay is located on GPIO pin 13: 18
    const unsigned char RELAY_12V_PIN_NUMBER;

    //Packet terminator as byte: '\r'
    const unsigned char PACKET_TERMINATOR;

    //Parameter delimiter, a space character: ' '
    const unsigned char PARAMETER_DELIMITER;

    //Response from FW in the case of a problem: "ERROR"
    const std::string ERROR;

    //Default wheel radius in meters: 0.0762
    const double DEFAULT_WHEEL_RADIUS;

    //Default ticks per revolution: 36
    const int DEFAULT_TICKS_PER_REVOLUTION;

    //=====================================================//
    //PARALLAX EDDIE CONTROL BOARD FIRMWARE COMMAND STRINGS//
    //=====================================================//

    //Returns 16 bits representing the Firmware version: "VER"
    const std::string GET_VERSION_STRING;

    //Sets specific GPIO pins to output using a 20 bit mask: "OUT"
    const std::string SET_GPIO_DIRECTION_OUT_STRING;

    //Sets specific GPIO pins to input using a 20 bit mask: "IN"
    const std::string SET_GPIO_DIRECTION_IN_STRING;

    //Sets specific GPIO output pins to high using a 20 bit mask: "HIGH"
    const std::string SET_GPIO_STATE_HIGH_STRING;

    //Sets specific GPIO output pins to low using a 20 bit mask: "LOW"
    const std::string SET_GPIO_STATE_LOW_STRING;

    //Return 20 bits representing a high/low state of all GPIO pins: "READ"
    const std::string GET_GPIO_STATE_STRING;

    //Return the ADC values as 8 separate 12 bits words: "ADC"
    const std::string GET_ADC_VALUE_STRING;

    //Return the digital PING values as 10|N separate 16 bits words: "PING"
    const std::string GET_PING_VALUE_STRING;

    //Sets drive power using 8 bits (signed) for each wheel -128|-127 is full reverse, 127 is full forward: "GO"
    const std::string SET_DRIVE_POWER_STRING;

    //Sets drive speed using 16 bits (signed) for each wheel, ? full reverse, ? full forward: "GOSPD"
    const std::string SET_DRIVE_SPEED_STRING;

    //Sets drive distance and speed using 16 bits (signed) value each, respectively: "TRVL"
    const std::string SET_DRIVE_DISTANCE_STRING;

    //Sets a gradual slow down to stop distance using 16 bits: "STOP"
    const std::string SET_STOP_DISTANCE_STRING;

    //Sends a rotate in place degrees and speed using 16 bits and 16 bits, respectively: "TURN"
    const std::string SET_ROTATE_STRING;

    //Returns the current speed at 8 bits: "SPD"
    const std::string GET_CURRENT_SPEED_STRING;

    //Returns the current heading, relative to start or last resetted, as 16 bits: "HEAD"
    const std::string GET_CURRENT_HEADING_STRING;

    //Returns the left and right encoder ticks as a pair of signed 16 bits: "DIST"
    const std::string GET_ENCODER_TICKS_STRING;

    //Zeros out the internal registers where encoder ticks are accumulated: "DIST"
    const std::string RESET_ENCODER_TICKS_STRING;

    //Sets a velocity ramping value for drive system: "ACC"
    const std::string SET_RAMPING_VALUE_STRING;

    //Sends a full stop: "STOP 00"
    //const std::string SET_FULL_STOP_STRING;

    //Send a series of 3 carriage returns to reset the FW serial buffer: "\r\r\r"
    const std::string FLUSH_BUFFERS_STRING;

    parallax_eddie_robot::Ping getPingData();
    parallax_eddie_robot::ADC getADCData();

private:
    //Associative lookups for command string -> packet bytes (char)
    std::map<std::string, unsigned char[6] > command_set_map_;
    struct termios tio;
    //struct termios stdio;
    int tty_fd;
    //fd_set rdset;

    void initialize(std::string port);
    std::string command(std::string str);

};

#endif	/* _PARALLAXBOARD_H */

