/*
 * Software License Agreement (BSD License)
 *
 * Copyright (c) 2012, Haikal Pribadi <haikal.pribadi@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *  * Neither the name of the Haikal Pribadi nor the names of other
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _EDDIE_H
#define	_EDDIE_H
//#include <stdlib.h>
//#include <stdio.h>
//#include <unistd.h>
#include <ros/ros.h>
#include <fcntl.h>
#include <termios.h>
#include <semaphore.h>
#include <string>
#include <sstream>
#include <map>
#include <parallax_eddie_robot/Ping.h>
#include <parallax_eddie_robot/ADC.h>
#include <parallax_eddie_robot/Accelerate.h>
#include <parallax_eddie_robot/DriveWithDistance.h>
#include <parallax_eddie_robot/DriveWithPower.h>
#include <parallax_eddie_robot/DriveWithSpeed.h>
#include <parallax_eddie_robot/GetDistance.h>
#include <parallax_eddie_robot/GetHeading.h>
#include <parallax_eddie_robot/GetSpeed.h>
#include <parallax_eddie_robot/ResetEncoder.h>
#include <parallax_eddie_robot/Rotate.h>
#include <parallax_eddie_robot/StopAtDistance.h>
#include <parallax_eddie_robot/DriveWithDistance.h>

class Eddie {
public:
    Eddie();
    virtual ~Eddie();

    //==============================================//
    //PARALLAX EDDIE CONTROL BOARD ATTRIBUTES       //
    //TO DO :: IMPLEMENT ALL THESE AS ROS PARAMETERS//
    //==============================================//

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

    //FW returns values representing 1/819v for ADC pins: 1/819
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

    //Parallax TRVL command may travel a distance in the range of -32767 to 32767
    //Parallax GOSPD command may travel a speed in the range of -32767 to 32767
    const int TRAVEL_SPEED_MAX_FORWARD;

    //Parallax TRVL command may travel a distance in the range of -32767 to 32767
    //Parallax GOSPD command may travel a speed in the range of -32767 to 32767
    const int TRAVEL_SPEED_MAX_REVERSE;

    //Parallax TRVL comman may travel at a speed up to 65535
    const int TRAVEL_MAX_SPEED;

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

    //Zeros out the internal registers where encoder ticks are accumulated: "RST"
    const std::string RESET_ENCODER_TICKS_STRING;

    //Sets a velocity ramping value for drive system: "ACC"
    const std::string SET_RAMPING_VALUE_STRING;

    //Sends a full stop: "STOP 00"
    //const std::string SET_FULL_STOP_STRING;

    //Send a series of 3 carriage returns to reset the FW serial buffer: "\r\r\r"
    const std::string FLUSH_BUFFERS_STRING;

    parallax_eddie_robot::Ping getPingData();
    parallax_eddie_robot::ADC getADCData();

    void publishPingData();
    void publishADCData();

private:
    sem_t mutex;
    struct termios tio;
    int tty_fd;

    ros::NodeHandle node_handle_;
    ros::Publisher ping_pub_;
    ros::Publisher adc_pub_;
    ros::ServiceServer accelerate_srv_;
    ros::ServiceServer drive_with_distance_srv_;
    ros::ServiceServer drive_with_power_srv_;
    ros::ServiceServer drive_with_speed_srv_;
    ros::ServiceServer get_distance_srv_;
    ros::ServiceServer get_heading_srv_;
    ros::ServiceServer get_speed_srv_;
    ros::ServiceServer reset_encoder_srv_;
    ros::ServiceServer rotate_srv_;
    ros::ServiceServer stop_at_distance_srv_;

    void initialize(std::string port);
    std::string command(std::string str);
    std::string intToHexString(int num);
    std::string generateCommand(std::string str1);
    std::string generateCommand(std::string str1, int num1);
    std::string generateCommand(std::string str1, int num1, int num2);

    bool accelerate(parallax_eddie_robot::Accelerate::Request &req,
            parallax_eddie_robot::Accelerate::Response &res);
    bool driveWithDistance(parallax_eddie_robot::DriveWithDistance::Request &req,
            parallax_eddie_robot::DriveWithDistance::Response &res);
    bool driveWithPower(parallax_eddie_robot::DriveWithPower::Request &req,
            parallax_eddie_robot::DriveWithPower::Response &res);
    bool driveWithSpeed(parallax_eddie_robot::DriveWithSpeed::Request &req,
            parallax_eddie_robot::DriveWithSpeed::Response &res);
    bool getDistance(parallax_eddie_robot::GetDistance::Request &req,
            parallax_eddie_robot::GetDistance::Response &res);
    bool getHeading(parallax_eddie_robot::GetHeading::Request &req,
            parallax_eddie_robot::GetHeading::Response &res);
    bool GetSpeed(parallax_eddie_robot::GetSpeed::Request &req,
            parallax_eddie_robot::GetSpeed::Response &res);
    bool resetEncoder(parallax_eddie_robot::ResetEncoder::Request &req,
            parallax_eddie_robot::ResetEncoder::Response &res);
    bool rotate(parallax_eddie_robot::Rotate::Request &req,
            parallax_eddie_robot::Rotate::Response &res);
    bool stopAtDistance(parallax_eddie_robot::StopAtDistance::Request &req,
            parallax_eddie_robot::StopAtDistance::Response &res);
};

#endif	/* _EDDIE_H */

