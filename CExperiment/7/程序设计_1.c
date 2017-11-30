#include<stdio.h>

typedef struct bits{
	unsigned char identity;
    int bit0;
    int bit1;
    int bit2;
    int bit3;
    int bit4;
    int bit5;
    int bit6;
    int bit7;
}Bits;

void f0(struct bits b);
void f1(struct bits b);
void f2(struct bits b);
void f3(struct bits b);
void f4(struct bits b);
void f5(struct bits b);
void f6(struct bits b);
void f7(struct bits b);

int main(void){
	Bits init;
	init.identity = 0xAA;
	init.bit0 = (init.identity&(0x01<<0))>>0;
	init.bit1 = (init.identity&(0x01<<1))>>1;
	init.bit2 = (init.identity&(0x01<<2))>>2;
	init.bit3 = (init.identity&(0x01<<3))>>3;
	init.bit4 = (init.identity&(0x01<<4))>>4;
	init.bit5 = (init.identity&(0x01<<5))>>5;
	init.bit6 = (init.identity&(0x01<<6))>>6;
	init.bit7 = (init.identity&(0x01<<7))>>7;


	void (* p_fun[8])(struct bits);
	p_fun[0]=f0;
	p_fun[1]=f1;
	p_fun[2]=f2;
	p_fun[3]=f3;
	p_fun[4]=f4;
	p_fun[5]=f5;
	p_fun[6]=f6;
	p_fun[7]=f7;

	(init.bit0)?p_fun[0](init):0;
	(init.bit1)?p_fun[1](init):0;
	(init.bit2)?p_fun[2](init):0;
	(init.bit3)?p_fun[3](init):0;
	(init.bit4)?p_fun[4](init):0;
	(init.bit5)?p_fun[5](init):0;
	(init.bit6)?p_fun[6](init):0;
	(init.bit7)?p_fun[7](init):0;

	return 0;
}

void f0(struct bits b)
{
printf("the function 0 is called! %d\n",b.bit0);
}
void f1(struct bits b)
{
printf("the function 1 is called! %d\n",b.bit1);
}void f2(struct bits b)
{
printf("the function 2 is called! %d\n",b.bit2);
}void f3(struct bits b)
{
printf("the function 3 is called! %d\n",b.bit3);
}void f4(struct bits b)
{
printf("the function 4 is called! %d\n",b.bit4);
}void f5(struct bits b)
{
printf("the function 5 is called! %d\n",b.bit5);
}void f6(struct bits b)
{
printf("the function 6 is called! %d\n",b.bit6);
}void f7(struct bits b)
{
printf("the function 7 is called! %d\n",b.bit7);
}
