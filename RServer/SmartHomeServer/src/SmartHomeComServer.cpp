#include <stdio.h>         //��׼�����������
#include <stdlib.h>        //��׼�����ⶨ��
#include <unistd.h>       //Unix��׼��������
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>          //�ļ����ƶ���
#include <termios.h>     //POSIX�жϿ��ƶ���
#include <errno.h>        //����Ŷ���
#include "shp.h"

#define FALSE 	0
#define TRUE	1

int speed_arr[] = { B38400, B19200, B9600, B4800, B2400, B1200, B300, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = {38400, 19200, 9600, 4800, 2400, 1200, 300, 38400, 19200, 9600, 4800, 2400, 1200, 300, };

void SetSpeed(int fd, int speed)
{
    int i;
    struct termios opt;    //����termios�ṹ

    if(tcgetattr(fd, &opt) != 0)
    {
        DBG("tcgetattr fd");
        return;
    }
    for(i = 0; i < sizeof(speed_arr) / sizeof(int); i++)
    {
        if(speed == name_arr[i])
        {
            tcflush(fd, TCIOFLUSH);
            cfsetispeed(&opt, speed_arr[i]);
            cfsetospeed(&opt, speed_arr[i]);
            if(tcsetattr(fd, TCSANOW, &opt) != 0)
            {
                DBG("tcsetattr fd");
                return;
            }
            tcflush(fd, TCIOFLUSH);
        }
    }
}

int SetParity(int fd, int databits, int stopbits, int parity)
{
    struct termios opt;
    if(tcgetattr(fd, &opt) != 0)
    {
        DBG("tcgetattr fd");
        return FALSE;
    }
   opt.c_cflag |= (CLOCAL | CREAD);        //һ������õı�־

    switch(databits)        //��������λ��
    {
    case 7:
        opt.c_cflag &= ~CSIZE;
        opt.c_cflag |= CS7;
        break;
    case 8:
        opt.c_cflag &= ~CSIZE;
        opt.c_cflag |= CS8;
        break;
    default:
        fprintf(stderr, "Unsupported data size.\n");
        return FALSE;
    }

    switch(parity)            //����У��λ
    {
    case 'n':
    case 'N':
        opt.c_cflag &= ~PARENB;        //���У��λ
        opt.c_iflag &= ~INPCK;        //enable parity checking
        break;
    case 'o':
    case 'O':
        opt.c_cflag |= PARENB;        //enable parity
        opt.c_cflag |= PARODD;        //��У��
        opt.c_iflag |= INPCK;            //disable parity checking
        break;
    case 'e':
    case 'E':
        opt.c_cflag |= PARENB;        //enable parity
        opt.c_cflag &= ~PARODD;        //żУ��
        opt.c_iflag |= INPCK;            //disable pairty checking
        break;
    case 's':
    case 'S':
        opt.c_cflag &= ~PARENB;        //���У��λ
        opt.c_cflag &= ~CSTOPB;        //??????????????
        opt.c_iflag |= INPCK;            //disable pairty checking
        break;
    default:
        fprintf(stderr, "Unsupported parity.\n");
        return FALSE;    
    }

    switch(stopbits)        //����ֹͣλ
    {
    case 1:
        opt.c_cflag &= ~CSTOPB;
        break;
    case 2:
        opt.c_cflag |= CSTOPB;
        break;
    default:
        fprintf(stderr, "Unsupported stopbits.\n");
        return FALSE;
    }

    opt.c_cflag |= (CLOCAL | CREAD);

    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
 
    opt.c_oflag &= ~OPOST;
    opt.c_oflag &= ~(ONLCR | OCRNL);    //��ӵ�
 
    opt.c_iflag &= ~(ICRNL | INLCR);
    opt.c_iflag &= ~(IXON | IXOFF | IXANY);    //��ӵ�

    tcflush(fd, TCIFLUSH);
    opt.c_cc[VTIME] = 1;        //���ó�ʱΪ15sec
    opt.c_cc[VMIN] = 120;        //Update the opt and do it now
    if(tcsetattr(fd, TCSANOW, &opt) != 0)
    {
        DBG("tcsetattr fd");
        return FALSE;
    }

    return TRUE;
}

