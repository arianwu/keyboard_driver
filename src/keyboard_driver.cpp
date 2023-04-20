#include "keyboard_driver/keyboard_driver.h"

KeyboardDriver::KeyboardDriver()
{
  cmd_pub_ =  nh_.advertise<std_msgs::String>("keys", 10);
}

void KeyboardDriver::keyLoop()
{
  char c;
  bool dirty=false;

  // get the console in raw mode
  tcgetattr(kfd, &cooked);
  memcpy(&raw, &cooked, sizeof(struct termios));
  raw.c_lflag &=~ (ICANON | ECHO);
  // Setting a new line, then end of file
  raw.c_cc[VEOL] = 1;
  raw.c_cc[VEOF] = 2;
  tcsetattr(kfd, TCSANOW, &raw);

  sleep(2);

  for(;;)
  {
    std_msgs::String msg;
    std::stringstream ss;

    // get the next event from the keyboard
    if(read(kfd, &c, 1) < 0)
    {
      perror("read():");
      exit(-1);
    }

    ROS_DEBUG("value: 0x%02X\n", c);

    msg.data = c;
    ROS_INFO("%s", msg.data.c_str());
    cmd_pub_.publish(msg);
    ros::spinOnce();
  }

  return;
}

void quit(int sig)
{
  tcsetattr(kfd, TCSANOW, &cooked);
  ros::shutdown();
  exit(0);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "keyboard_driver");
  KeyboardDriver allegro_hand_keyboard_cmd;

  signal(SIGINT,quit);

  allegro_hand_keyboard_cmd.keyLoop();

  return(0);
}

