#include "menu.h"
#include "12864s.h"
#include "DS1302.h"
#include "bh1750.h"
#include "DS18B20.h"
#include "key.h"

#define LOCKED (1)
#define UNLOCKED (0)
#define ERROR_LOCKED (INT32_MIN)

menu_list m[6];//all menu list strust 
menu_list *g_menu=m;
uint8_t m_name[7][16]={"光照","温度","换水","灯光","充氧","循环"};
uint8_t  null[]="NULL";// null pointer to this 
uint8_t _IsON[]="ON";
uint8_t _IsOFF[]="OFF";
uint8_t ext[]="2102";//just for  test]
uint8_t g_current_key=0;
uint8_t r_guangzhao[3]={0,8,19};
uint8_t r_wendu[3]={0,8,16};
uint8_t r_huanshui[2]={0,1};

uint8_t g_menu_nest=0;

void __handle_key(void);


static void dis_temp(void);
static void dis_light(void);
static void dis_ext(void);


void menu_init(menu_list *m,\
				uint8_t id,\
				void *pre,\
				uint8_t lock,\
				uint8_t *nam,\
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
	menu_init(&m[3],4,&m[2],0,m_name[3],4,_IsOFF,&m[4]);
	menu_init(&m[4],5,&m[3],0,m_name[4],5,_IsOFF,&m[5]);
	menu_init(&m[5],6,&m[4],0,m_name[5],6,_IsOFF,&m[0]);
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
	printf("%d:%s-%d-ext-%s\r\n",g_menu->id,g_menu->name,g_menu->val,(uint8_t *)(g_menu->ext_data));
	
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
		
	else if(g_current_key== KEY_OK)
	{
		
		g_current_key=0;
		if(g_menu_nest == 0)
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
						(*((uint8_t *)(g_menu->ext_data)+0)) = ~(*((uint8_t *)(g_menu->ext_data)+0));
						printf("guangzhao ON/OFF\r\n");
						break;
					}
					
					case (2):
					{
						(*((uint8_t *)(g_menu->ext_data)+0)) = ~(*((uint8_t *)(g_menu->ext_data)+0));
						printf("temp ON/OFF\r\n");
						break;
					}
					case (3):
					{
						(*((uint8_t *)(g_menu->ext_data)+0)) = ~(*((uint8_t *)(g_menu->ext_data)+0));
						printf("exchange water ON/OFF\r\n");
						break;
					}
					
					
					default :break;
				}
		}
	}
	
	else if(g_current_key== KEY_INC)
	{
		g_current_key=0;
		switch(g_menu->id)
				{
					case (1):
					{
						if((*((uint8_t *)(g_menu->ext_data)+0)) == 0)
						{
								(*((uint8_t *)(g_menu->ext_data)+1))++;
								if((*((uint8_t *)(g_menu->ext_data)+1)) == 24)(*((uint8_t *)(g_menu->ext_data)+1)) =0;
								printf("guangzhao inc\r\n");
						}
						break;
					}
					case (2):
					{
						if((*((uint8_t *)(g_menu->ext_data)+0)) == 0)
						{
								(*((uint8_t *)(g_menu->ext_data)+1))++;
								if((*((uint8_t *)(g_menu->ext_data)+1)) > 15)(*((uint8_t *)(g_menu->ext_data)+1)) =15;
								printf("wendu inc\r\n");
						}
						break;
					}
					
					case (3):
					{
						if((*((uint8_t *)(g_menu->ext_data)+0)) == 0)
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
	
		else if(g_current_key== KEY_DEC)
	{
		g_current_key=0;
		switch(g_menu->id)
				{
					case (1):
					{
						if((*((uint8_t *)(g_menu->ext_data)+0)) == 0)
						{
								if((*((uint8_t *)(g_menu->ext_data)+1)) == 0)(*((uint8_t *)(g_menu->ext_data)+1)) =24;
								(*((uint8_t *)(g_menu->ext_data)+1))--;
								printf("guangzhao dec\r\n");
						}
						break;
					}
					case (2):
					{
						if((*((uint8_t *)(g_menu->ext_data)+0)) == 0)
						{
								(*((uint8_t *)(g_menu->ext_data)+1))--;
								if((*((uint8_t *)(g_menu->ext_data)+1)) < 5)(*((uint8_t *)(g_menu->ext_data)+1)) =5;
								printf("wendu dec\r\n");
						}
						break;
					}
					case (3):
					{
						if((*((uint8_t *)(g_menu->ext_data)+0)) == 0)
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
	
	else if(g_current_key== KEY_LOOP)
	{
		g_current_key=0;
		switch(g_menu->id)
				{
					case (1):
					{
						if((*((uint8_t *)(g_menu->ext_data)+0)) == 0)
						{
								(*((uint8_t *)(g_menu->ext_data)+2))++;
								if((*((uint8_t *)(g_menu->ext_data)+2)) == 24)(*((uint8_t *)(g_menu->ext_data)+2)) =0;//time 
								printf("h inc\r\n");
						}
						break;
					}
					
					case (2):
					{
						if((*((uint8_t *)(g_menu->ext_data)+0)) == 0)
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
	
		else if(g_current_key== KEY_OXY)
	{
		g_current_key=0;
		switch(g_menu->id)
				{
					case (1):
					{
						if((*((uint8_t *)(g_menu->ext_data)+0)) == 0)
						{
								if((*((uint8_t *)(g_menu->ext_data)+2)) == 0)(*((uint8_t *)(g_menu->ext_data)+2)) =24;
								(*((uint8_t *)(g_menu->ext_data)+2))--;
								printf("h dec\r\n");
						}
						break;
					}
					
					case (2):
					{
						if((*((uint8_t *)(g_menu->ext_data)+0)) == 0)
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

static void dis_ext(void)
{
	
	LcdPrintString(4,1,"                ");
	switch(g_menu->id)
	{	
		case (1):
		{
			if((*((uint8_t *)(g_menu->ext_data)+0)) == 0)
			{
				LcdPrintInt(4,2,(int32_t)(*((uint8_t *)(g_menu->ext_data)+1)));
				LcdPrintInt(4,4,(int32_t)(*((uint8_t *)(g_menu->ext_data)+2)));
				printf("ext is %d\r\n",*(uint8_t *)(g_menu->ext_data));
			}
			else
			{
				LcdPrintString(4,2,"OFF");
			}
		}
		
		case (2):
		{
			if((*((uint8_t *)(g_menu->ext_data)+0)) == 0)
			{
				LcdPrintInt(4,2,(int32_t)(*((uint8_t *)(g_menu->ext_data)+1)));
				LcdPrintInt(4,4,(int32_t)(*((uint8_t *)(g_menu->ext_data)+2)));
				printf("ext is %d\r\n",*(uint8_t *)(g_menu->ext_data));
			}
			else
			{
				LcdPrintString(4,2,"OFF");
			}
		break;
		}
		case (3):
		{
			if((*((uint8_t *)(g_menu->ext_data)+0)) == 0)
			{
				LcdPrintInt(4,2,(int32_t)(*((uint8_t *)(g_menu->ext_data)+1)));
				printf("ext is %d\r\n",*(uint8_t *)(g_menu->ext_data));
			}
			else
			{
				LcdPrintString(4,2,"OFF");
			}
			break;
		}
		
		default :break;
	}
}


