#include "menu.h"
#include "12864s.h"
#include "DS1302.h"
#include "bh1750.h"
#include "DS18B20.h"
#include "key.h"
#include "pwm.h"

#define LOCKED (1)
#define UNLOCKED (0)
#define ERROR_LOCKED (INT32_MIN)

menu_list m[6];//all menu list strust 
menu_list *g_menu=m;
int8_t m_name[7][16]={"����","�¶�","��ˮ","�ƹ�","����","ѭ��"};
int8_t  null[]="NULL";// null pointer to this 
int8_t _IsON[]="ON";
int8_t _IsOFF[]="OFF        ";
uint8_t ext[]="2102";//just for  test]
uint8_t g_current_key=0;

uint8_t r_guangzhao[3]={0,8,22};//enable start stop
uint8_t r_wendu[3]={0,8,30};

uint8_t r_huanshui[2]={0,1};//3
//uint8_t r_dengguang[3]={0,20};
uint8_t r_chongyang[5]={0,1};//5
uint8_t r_xunhuan[2]={0,1};//6

uint8_t g_menu_nest=0;

void __handle_key(void);


static void dis_temp(void);
static void dis_light(void);
static void dis_ext(void);


void menu_init(menu_list *m,\
				uint8_t id,\
				void *pre,\
				uint8_t lock,\
				int8_t *nam,\
				int32_t value,\
				void *ext_data,\
				void *next)

{
	m->id=id;
	m->prev_menu=(menu_list* )pre;
	m->lock=lock; 
	m->name=nam;
	m->val=value;
	m->ext_data = (uint8_t *)ext_data;
	m->next_menu=(menu_list* )next;
}

void menu_all_config(void)
{
	menu_init(&m[0],1,&m[5],0,m_name[0],1,r_guangzhao,&m[1]);
	menu_init(&m[1],2,&m[0],0,m_name[1],2,r_wendu,&m[2]);
	menu_init(&m[2],3,&m[1],0,m_name[2],3,r_huanshui,&m[3]);
	menu_init(&m[3],4,&m[2],0,m_name[3],0,NULL,&m[4]);
	menu_init(&m[4],5,&m[3],0,m_name[4],5,r_chongyang,&m[5]);
	menu_init(&m[5],6,&m[4],0,m_name[5],6,r_xunhuan,&m[0]);
	//g_menu=&m[1];
}

void menu_page_up(void)
{
	g_menu=g_menu->prev_menu;
}

void menu_page_down(void)
{
	g_menu=g_menu->next_menu;
}

void menu_lock(menu_list *p)
{
	p->lock=LOCKED;
}

void menu_unlock(menu_list *p)
{
	p->lock=UNLOCKED;
}

int32_t menu_set_val(menu_list *p,int32_t v)
{
	if(p->lock == LOCKED)
	{
		return ERROR_LOCKED; // if menu is lock ,then return 
	}
	else
	{
		p->val=v; // set val success 
		return v;
	}
}

void menu_display(void)
{
	
	//menu_dis_get_value();
	//printf("%d:%s-%d-ext-%s\r\n",g_menu->id,g_menu->name,g_menu->val,(uint8_t *)(g_menu->ext_data));
	
	LcdPrintString(1,1,g_menu->name);
  dis_time();
	dis_temp();
	dis_light();
	dis_ext();
	
	//
}


static void dis_temp(void)
{
	GetTemp();
	LcdPrintTemp(3,2,g_Temp[0]);
	LcdPrintTemp(3,6,g_Temp[1]);
}

static void dis_light(void)
{
	bh1750Start();
	bh1750ReadBuff(pBH1750_RAW);	
	g_Light=GetLight(pBH1750_RAW);
	LcdPrintlight(2,1,g_Light);
}

void dis_time(void)
{
	Ds1302ReadTime();
	TimeConvertToString();
	//printf("Now Date is :%s-%s\r\n",YearString,TimeString);
	LcdPrintString(1,4,YearString);
  LcdPrintString(2,5,TimeString);
}


void __handle_key(void)
{
	uint8_t key_tmp=0;
	key_tmp=GetKey();
	if(key_tmp!=255)
	{
		g_current_key=key_tmp;
	}
	
	if(g_current_key== KEY_PAGE_DOWN)// page down 
		{	
				g_current_key=0;
				g_menu_nest = 0;
				menu_page_down();
		}
		
	else if(g_current_key== KEY_OK)//ok
	{
		
		g_current_key=0;
		if( g_menu_nest== 0)
		{
			g_menu_nest++;
			printf("key ok nest %d \r\n",g_menu_nest);
		}
		else
		{
				switch(g_menu->id)
				{
					case (1):
					{
						if((*((uint8_t *)(g_menu->ext_data)+0)) == 0)
						{
							*(((uint8_t *)(g_menu->ext_data) + 0))=1;//open 
							m[3].val = 1;
						}
						else
						{
							*(((uint8_t *)(g_menu->ext_data) + 0))=0;//close
							m[3].val = 0;
						}
						printf("1 ON/OFF shift \r\n");
						break;
					}
					case (2):
					case (3):
					case (5):
					case (6):
					{
						if((*((uint8_t *)(g_menu->ext_data)+0)) == 0)
						{
							*(((uint8_t *)(g_menu->ext_data) + 0))=1;//open 
						}
						else
						{
							*(((uint8_t *)(g_menu->ext_data) + 0))=0;//close
						}
						printf("2,3,5,6 ON/OFF shift \r\n");
						break;
					}
					
					case (4):
					{
						if(g_menu->val ==0)
						{
							g_menu->val = 1;
							r_guangzhao[0] = 1;//open
						}
						else
						{
							g_menu->val = 0;
							r_guangzhao[0] = 0;//close 
						}
						
						printf("g-val is %d\r\n",g_menu->val);
						break;
					}
					
					default :break;
				}
		}
	}
	
	else if(g_current_key== KEY_INC)//+1
	{
		g_current_key=0;
		switch(g_menu->id)
				{
					case (1):
					{
						if((*((uint8_t *)(g_menu->ext_data)+0)) != 0)
						{
								(*((uint8_t *)(g_menu->ext_data)+1))++;
								if((*((uint8_t *)(g_menu->ext_data)+1)) == 24)(*((uint8_t *)(g_menu->ext_data)+1)) =0;
								printf("guangzhao inc\r\n");
						}
						break;
					}
					case (2):
					{
						if((*((uint8_t *)(g_menu->ext_data)+0)) != 0)
						{
								(*((uint8_t *)(g_menu->ext_data)+1))++;
								if((*((uint8_t *)(g_menu->ext_data)+1)) > 15)(*((uint8_t *)(g_menu->ext_data)+1)) =15;
								printf("wendu inc\r\n");
						}
						break;
					}
					
					case (3):
					case (5):
					case (6):
					{
						if((*((uint8_t *)(g_menu->ext_data)+0)) != 0)
						{
								(*((uint8_t *)(g_menu->ext_data)+1))++;
								if((*((uint8_t *)(g_menu->ext_data)+1)) > 12)(*((uint8_t *)(g_menu->ext_data)+1)) =12;
								printf("huanshui inc\r\n");
						}
						break;
					}
					default :break;
				}
	}
	
		else if(g_current_key == KEY_DEC)//-1
	{
		g_current_key=0;
		switch(g_menu->id)
				{
					case (1):
					{
						if((*((uint8_t *)(g_menu->ext_data)+0)) != 0)
						{
								if((*((uint8_t *)(g_menu->ext_data)+1)) == 0)(*((uint8_t *)(g_menu->ext_data)+1)) =24;
								(*((uint8_t *)(g_menu->ext_data)+1))--;
								printf("guangzhao dec\r\n");
						}
						break;
					}
					case (2):
					{
						if((*((uint8_t *)(g_menu->ext_data)+0)) != 0)
						{
								(*((uint8_t *)(g_menu->ext_data)+1))--;
								if((*((uint8_t *)(g_menu->ext_data)+1)) < 5)(*((uint8_t *)(g_menu->ext_data)+1)) =5;
								printf("wendu dec\r\n");
						}
						break;
					}
					case (3):
					case (5):
					case (6):
					{
						if((*((uint8_t *)(g_menu->ext_data)+0)) != 0)
						{
								(*((uint8_t *)(g_menu->ext_data)+1))--;
								if((*((uint8_t *)(g_menu->ext_data)+1)) <=1 )(*((uint8_t *)(g_menu->ext_data)+1)) =1;
								printf("haunshui dec\r\n");
						}
						break;
					}
					
					
					
					default :break;
				}
	}
	
	else if(g_current_key == KEY_INC_TEMP)
	{
		g_menu_nest = 0;
		g_current_key=0;
		g_menu=&m[1];
	}
	else if(g_current_key == KEY_EXCHEANGE_WATER)
	{
		g_menu_nest = 0;
		g_current_key=0;
		g_menu=&m[2];
	}
	else if(g_current_key == KEY_LIGHT)
	{
		g_menu_nest = 0;
		g_current_key=0;
		g_menu=&m[3];
	}
	
	else if(g_current_key== KEY_LOOP)
	{
		if(g_menu_nest == 0)
		{
			g_current_key=0;
			g_menu=&m[5];
		}
		else
		{
			g_current_key=0;
			switch(g_menu->id)
					{
						case (1):
						{
							if((*((uint8_t *)(g_menu->ext_data)+0)) != 0)
							{
									(*((uint8_t *)(g_menu->ext_data)+2))++;
									if((*((uint8_t *)(g_menu->ext_data)+2)) == 24)(*((uint8_t *)(g_menu->ext_data)+2)) =0;//time 
									printf("h inc\r\n");
							}
							break;
						}
						
						case (2):
						{
							if((*((uint8_t *)(g_menu->ext_data)+0)) != 0)
							{
									(*((uint8_t *)(g_menu->ext_data)+2))++;
									if((*((uint8_t *)(g_menu->ext_data)+2)) > 24)(*((uint8_t *)(g_menu->ext_data)+2)) =24;//temp
									printf("h inc\r\n");
							}
							break;
						}
						
						default :break;
					}
		}
	}
	
		else if(g_current_key== KEY_OXY)
	{
		if(g_menu_nest ==0 )
		{
			g_current_key=0;
			g_menu=&m[4];
		}
		else
		{
			g_current_key=0;
			switch(g_menu->id)
					{
						case (1):
						{
							if((*((uint8_t *)(g_menu->ext_data)+0)) != 0)
							{
									if((*((uint8_t *)(g_menu->ext_data)+2)) == 0)(*((uint8_t *)(g_menu->ext_data)+2)) =24;
									(*((uint8_t *)(g_menu->ext_data)+2))--;
									printf("h dec\r\n");
							}
							break;
						}
						
						case (2):
						{
							if((*((uint8_t *)(g_menu->ext_data)+0)) != 0)
							{
									(*((uint8_t *)(g_menu->ext_data)+2))--;
									if((*((uint8_t *)(g_menu->ext_data)+2)) < 16)(*((uint8_t *)(g_menu->ext_data)+2)) =16;
									printf("h dec\r\n");
							}
							break;
						}
						
						default :break;
					}
		}
	}
		
}

static void dis_ext(void)
{
	
	LcdPrintString(4,1,"                ");
	switch(g_menu->id)
	{	
		case (1):
		{
			if((*((uint8_t *)(g_menu->ext_data)+0)) != 0)
			{
				LcdPrintInt(4,2,(int32_t)(*((uint8_t *)(g_menu->ext_data)+1)));
				LcdPrintInt(4,4,(int32_t)(*((uint8_t *)(g_menu->ext_data)+2)));
				printf("guangzhao is on\r\n");
			}
			else
			{
				LcdPrintString(4,2,_IsOFF);
				printf("guangzhao off\r\n");
			}
		}
		
		case (2):
		{
			if((*((uint8_t *)(g_menu->ext_data)+0)) != 0)
			{
				LcdPrintInt(4,2,(int32_t)(*((uint8_t *)(g_menu->ext_data)+1)));
				LcdPrintInt(4,4,(int32_t)(*((uint8_t *)(g_menu->ext_data)+2)));
				printf("wendu  is on \r\n");
			}
			else
			{
				LcdPrintString(4,2,_IsOFF);
				printf("wendu is off\r\n");
			}
		break;
		}
		case (3):
		case (5):
		case (6):
		{
			if((*((uint8_t *)(g_menu->ext_data)+0)) != 0)
			{
				LcdPrintInt(4,2,(int32_t)(*((uint8_t *)(g_menu->ext_data)+1)));
				printf("ext is %d\r\n",*(uint8_t *)(g_menu->ext_data));
			}
			else
			{
				LcdPrintString(4,2,_IsOFF);
			}
			break;
		}
			
		case (4):
		{
			if(g_menu->val != 0)
			{
				LcdPrintString(4,2,_IsON);
				printf("light ON%d\r\n",g_menu->val);
			}
			else
			{
				LcdPrintString(4,2,_IsOFF);
			}
			break;
		}
			
		default :break;
	}
}




void __handle_operate(void)
{
	//printf("------------%d-%d-%d\r\n",m[3].val,r_guangzhao[1],r_guangzhao[2]);
	if(m[3].val !=0)//;guangzhao
	{
		if((r_guangzhao[1] < TimeRaw) && (TimeRaw < r_guangzhao[2]))
		{
			printf("auto guangzhao\r\n");
			if(g_Light >= 100)g_Light=100;
			sync_pwm((100-g_Light)/2);
		
		}
		else
		{
			sync_pwm(0);
		}
	}
	else
	{
		sync_pwm(0);
	}
	
	if(r_wendu[1] > g_TempMax)//����
	{
		//����
	}
	
 if(r_wendu[2] < g_TempMin)//����
	{
		//��ˮ
	}
	
	if(r_huanshui[0] != 0)
	{
		if(TimeRaw%r_huanshui[1] == 0)
		{
			//��ˮ
		}
		else
		{
			//none
		}
	}
	
	if(r_chongyang[0] != 0)
	{
		if(TimeRaw%r_chongyang[1] == 0)
		{
			//chongyang
		}
		else
		{
			//none
		}
	}
	
	if(r_xunhuan[0] != 0)
	{
		if(TimeRaw%r_xunhuan[1] == 0)
		{
			//ѭ��
		}
		else
		{
			//none
		}
	}
	
}



