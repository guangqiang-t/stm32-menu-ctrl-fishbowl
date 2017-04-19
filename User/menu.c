#include "menu.h"

#define LOCKED (1)
#define UNLOCKED (0)
#define ERROR_LOCKED (INT32_MIN)

menu_list m[6];//all menu list strust 
menu_list *g_menu=m;
uint8_t m_name[7][16]={"guangzhao","wendu","huanshui","dengguang","chongyang","shuixunhuan"};
uint8_t  null[]="NULL";// null pointer to this 
uint8_t ext[]="2102";//just for  test


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
	menu_init(&m[0],1,&m[5],0,m_name[0],200,ext,&m[1]);
	menu_init(&m[1],2,&m[0],0,m_name[1], 25,null,&m[2]);
	menu_init(&m[2],3,&m[1],0,m_name[2],  0,null,&m[3]);
	menu_init(&m[3],4,&m[2],0,m_name[3], 30,null,&m[4]);
	menu_init(&m[4],5,&m[3],0,m_name[4],  1,null,&m[5]);
	menu_init(&m[5],6,&m[4],0,m_name[5],600,null,&m[0]);
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
	printf("%d:%s-%d-ext-%s\r\n",g_menu->id,g_menu->name,g_menu->val,(uint8_t *)(g_menu->ext_data));
}

