#include "cGPIOHandler.h"
#include <unistd.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
using namespace std;


/* constructor and destructor functions */
cGPIOHandler::cGPIOHandler(int gpio_pin, bool bReadMode)
{
	m_GPIONumber = gpio_pin;
	vInitGPIO(bReadMode);
}

cGPIOHandler::~cGPIOHandler()
{
	vCloseRead();
}

void cGPIOHandler::vInitGPIO(bool bReadMode)
{
    int fd;
	char buf[50];
	//exporting gpio pin
	fd = open("/sys/class/gpio/export", O_WRONLY);
	sprintf(buf, "%d", m_GPIONumber);
	write(fd, buf, strlen(buf));
	close(fd);

	sprintf(buf, "/sys/class/gpio/gpio%d/direction", m_GPIONumber);
	fd = open(buf, O_WRONLY);
	if(bReadMode)
	{
		//write the direction now

		write(fd, "in", 2);
		close(fd);
		//get the file descriptor
		//sprintf(buf, "/sys/class/gpio/gpio%d/value", m_GPIONumber);
		//m_fd=open(buf,O_RDONLY);
	}
	else
	{
		//write the direction now
		write(fd, "out", 3);
		close(fd);

		//get the file descriptor

	}
}

//note: thread needs to be created outside this scope(if needed)
bool cGPIOHandler::bReadValue()
{
	char read_value;
	char buf[50];
	bool return_value=false;

	sprintf(buf, "/sys/class/gpio/gpio%d/value", m_GPIONumber);
	int fd=open(buf,O_RDONLY);
	//now read the gpio pin
	lseek(fd, 0, SEEK_SET);
	read(fd, &read_value, 1);

	if(read_value == '1')
	{
		return_value = true;
		//cout<<"R1"<<endl;
	}
	else if(read_value == '0')
	{
		return_value=false;
		//cout<<"R0"<<endl;
	}
	close(fd);
	return return_value;
}


void cGPIOHandler::vWritePin(bool value)
{
	char buf[50];
	sprintf(buf, "/sys/class/gpio/gpio%d/value", m_GPIONumber);
	int fd=open(buf,O_WRONLY);
	if(value == true)
	{
		write(fd,"1",1);
	}
	else
	{
		write(fd,"0",1);
	}
	close(fd);
}

void cGPIOHandler::vCloseRead()
{
    int fd;
	char buf[50];

	//exporting gpio pin
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	sprintf(buf, "%d", m_GPIONumber);
	write(fd, buf, strlen(buf));
	close(fd);
}

