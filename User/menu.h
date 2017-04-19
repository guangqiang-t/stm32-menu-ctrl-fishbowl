#ifndef _menu_H_
#define _menu_H_


#include <stdio.h>
#include <stdint.h>

//#include 
typedef struct 
{
	uint8_t id;
	void *prev_menu;
	uint8_t lock;
	uint8_t *name;
	int32_t val;
	void *ext_data;
	void *next_menu;
}menuNODE;

typedef menuNODE menu_list;

extern menu_list *g_menu;//global
extern void menu_init(menu_list *m,\
				uint8_t id,\
				void *pre,\
				uint8_t lock,\
				uint8_t *nam,\
				int32_t value,\
				void *ext_data,\
				void *next);

extern void menu_all_config(void);
extern void menu_page_up(void);				
extern void menu_page_down(void);
extern void menu_display(void);
extern void menu_lock(menu_list *p);
extern void menu_unlock(menu_list *p);
extern int32_t  menu_set_val(menu_list *p,int32_t v);


#endif //_menu_H_
