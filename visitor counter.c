# include <lpc214x.h>
# define RS 9
# define EN 10
# include "delay.h"
void nobody(void);
void command(char);
void data(char);
void init(void);
void string(char*);
void number(unsigned int);
char pin[]={16,21,23,26};
char value[]={0x10,0x20,0x40,0x80};
int t=0;
int main()
{
	IO1DIR=1<<16;
	init();
	command(0x80);
	string("BIDRECTIONAL");
	command(0xC0);
	string("Visitor Counter");
	delay(500);
	command(0x01);
	while(1)
	{ 
			if(IO1PIN &(1<<31))
			{
				while(!(IO1PIN&(1<<17)));
					t++;
				while(IO1PIN&(1<<17));
				command(0x80);
				IO1SET=1<<16;
				string("Person =");
				number(t);
				command(0xc0);
				string("!Lights on");		 				
			}
			else if(IO1PIN&(1<<17))
			{
				while(!(IO1PIN&(1<<31)));
				if(t!=0)
				{
				t--;
			  IO1SET=1<<16;
				}
				while(IO1PIN&(1<<31));
				command(0x80);
				string("Person =");
				number(t);
				command(0xc0);
				string("!Lights on");
	     if(t==0)
			 {
				IO1CLR=1<<16;
				 command(0x80);
				string("Person =");
				number(t);
				command(0xc0);
				string("lights off");
			 }					
			}
	}
}
void command(char x)
{
	int i;
	char y;
	    
	for(i=0;i<=3;i++)
	{
		y=x & value[i];
		if(y!=0)
		{
			IO0SET=1<<pin[i];
		}
		else
			{
				IO0CLR=1<<pin[i];
		  }
			
	}
    	IO0CLR=1<<RS;
			IO0SET=1<<EN;
			delay(10);
			IO0CLR=1<<EN;
		for(i=0;i<=3;i++)
	{
		y=(x<<4) & value[i];// shifting lower to upper nibble
		if(y!=0)
		{
			IO0SET=1<<pin[i];
		}
		else
			{
				IO0CLR=1<<pin[i];
		  }
	}
	    IO0CLR=1<<RS;
			IO0SET=1<<EN;
			delay(10);
			IO0CLR=1<<EN;
	
}
void data(char x)
{
	int i;
	char y;
	   
	for(i=0;i<=3;i++)
	{
		y=x & value[i];
		if(y!=0)
		{
			IO0SET=1<<pin[i];
		}
		else
			{
				IO0CLR=1<<pin[i];
		  }
			
	}
    	IO0SET=1<<RS;
			IO0SET=1<<EN;
			delay(10);
			IO0CLR=1<<EN;
		for(i=0;i<=3;i++)
	{
		y=(x<<4) & value[i];// shifting lower to upper nibble
		if(y!=0)
		{
			IO0SET=1<<pin[i];
		}
		else
			{
				IO0CLR=1<<pin[i];
		  }
	}
	    IO0SET=1<<RS;
			IO0SET=1<<EN;
			delay(10);
			IO0CLR=1<<EN;
}
void init(void)
{
	IO0DIR=1<<RS|1<<EN|1<<16|1<<21|1<<23|1<<26;
	command(0x02);// return home
	command(0x28);// 4 bit mode
	command(0x06);
	command(0x0E);
	command(0x01);
	command(0x80);
}
void string(char *p)
{
	while(*p!='\0')
	{
		data(*p);
		p++;
	}
}
void number(unsigned int x)
{
	unsigned int m=0,i,d=1, n;
	if(x==0)
	{	
	data(48);
	}
	
	else
	{
		n=x;
		while(x!=0)
		{
			x=x/10;
			m++;
		}
		
		for(i=0;i<m-1;i++)
		{
			d=d*10;
		}
		
		for(i=0;i<m;i++)
		{
			data((n/d)+48);
			n=n%d;
			d=d/10;
			
		}
		
	}
}