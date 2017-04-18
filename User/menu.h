#ifndef _menu_H_
#define _menu_H_


#include <stdio.h>
#include <stdint.h>

//#include 
typedef struct 
{
	unsigned char id;
	void *prev_menu;
	unsigned char lock;
	unsigned char *name;
	unsigned int val;
	void *next_menu;
}menuNODE;

typedef menuNODE menu_list;

extern menu_list *g_menu;//global
extern void menu_init(menu_list *m,\
				unsigned id,\
				void *pre,\
				unsigned char lock,\
				unsigned char *nam,\
				unsigned int value,\
				void *next);

extern void menu_all_config(void);
extern void menu_page_up(void);				
extern void menu_page_down(void);
extern void menu_display(void);
extern void menu_lock(menu_list *p);
extern void menu_unlock(menu_list *p);
extern int16_t  menu_set_val(menu_list *p,int16_t v);


#endif //_menu_H_
