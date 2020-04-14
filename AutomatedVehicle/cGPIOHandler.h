
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <cstring>
#ifndef CGPIOHANDLER_H_
#define CGPIOHANDLER_H_
class cGPIOHandler
{
	public:
		cGPIOHandler(int,bool); //constructor
		~cGPIOHandler();

		bool bReadValue();

		void vWritePin(bool value);


		void vInitGPIO(bool);

		void vCloseRead();

	private:

		int m_GPIONumber;

		int m_fd;


};
#endif //CGPIOHANDLER_H_
