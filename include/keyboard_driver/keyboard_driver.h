#include <ros/ros.h>

#include "std_msgs/String.h"

#include <iostream>
#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>

#include "keyboard_driver/virtualkey_codes.h"

#define DOF_JOINTS 16

using namespace std;

int kfd = 0;
struct termios cooked, raw;
class KeyboardDriver
{
public:
  KeyboardDriver();
  void keyLoop();

private:
  ros::NodeHandle nh_;
  ros::Publisher cmd_pub_;
};