#include<reg51.h>
#define uchar unsigned char 
#define uint unsigned int

uchar code tableduan[]={0x02,0x9e,0x24,
0x0c,0x98,0x48,0x40,0x1e,0x00,0x08};

uchar ircode[4];
uchar irdata[33];

uchar irtime,numbit,read,start,stop;

void time0init()
{
	TMOD=0x02;
	TH0=0x00;
	TL0=0x00;
	ET0=1;
	EA=1;
	TR0=1;
}

void int1init()
{
	IT1=1;
	EX1=1;
	EA=1;
}

//void delay(uchar z)
//{
//	uint i,j;
//	for(i=z;i>0;i--)
//		for(j=10;j>0;j--);
//}

void ir_read()
{
	uchar i,j,value;
	uchar k=1;
	for(i=0;i<4;i++)
	{
		for(j=0;j<8;j++)
		{
			value=value>>1;
			if(irdata[k]>6)
			{
				value=value|0x80;
			}
			k++;	
		}
		ircode[i]=value;
	}
	read=1;
}

uchar translation()
{
	uchar num=0;
	switch(ircode[2])
	{
		case 0x0c:	num=1;	break;
		case 0x18:	num=2;	break;
		case 0x5e:	num=3;	break;
		case 0x08:	num=4;	break;
		case 0x1c:	num=5;	break;
		case 0x5a:	num=6;	break;
		case 0x42:	num=7;	break;
		case 0x52:	num=8;	break;
		case 0x4a:	num=9;	break;
		default: num=0; break;
	}
	return num;
}

void main()
{
	uchar a=0;
	time0init();
	int1init();
	while(1)
	{
		if(stop)
		{
			stop=0;
			ir_read();
		} 
		if(read)
		{
			read=0;
			a=translation();
		}	
		P2=tableduan[a];	
	}
}

void time0() interrupt 1
{
	irtime++;
}

void int1() interrupt 2
{
	if(start)
	{
		if(irtime>32)
		{
			numbit=0;
		}
		irdata[numbit]=irtime;
		irtime=0;
		numbit++;
		if(numbit==33)
		{
			numbit=0;
			stop=1;
		}
	}
	else
	{
		start=1;
		irtime=0;
	}
}