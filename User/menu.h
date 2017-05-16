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
	int8_t *name;
	int32_t val;
	void *ext_data;
	void *next_menu;
}menuNODE;


typedef menuNODE menu_list;

enum keyvalue{KEY_INC=1,KEY_DEC,KEY_PAGE_DOWN,KEY_OK,KEY_INC_TEMP,KEY_EXCHEANGE_WATER,KEY_LOOP,KEY_OXY,KEY_LIGHT};

extern menu_list *g_menu;//global
extern void menu_init(menu_list *m,\
				uint8_t id,\
				void *pre,\
				uint8_t lock,\
				int8_t *nam,\
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
void menu_display(void);
void dis_time(void);
void __handle_key(void);
void __handle_operate(void);

#endif //_menu_H_
