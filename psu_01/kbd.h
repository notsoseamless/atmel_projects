/* vim: set sw=8 ts=8 si : */
/*************************************************************************
 Title	:   include file for the keyboard of the digital power supply
 Copyright: GPL
***************************************************************************/
#ifndef KBD_H
#define KBD_H

extern void init_kbd(void);
extern uint8_t check_u_button(int16_t *u);
extern uint8_t check_i_button(int16_t *i);
extern uint8_t check_store_button(void);

#endif 
