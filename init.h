/* 
 * File:   init.h
 * Author: tpeea
 *
 * Created on 18 décembre 2019, 18:47
 */

#ifndef INIT_H
#define	INIT_H


void LCDinit(void);
void initBouton(void);
void initDCI(void);
void wait(unsigned int time);
static void LCDWriteCmd(char c);
void LCDgoto(int l,int c);
void LCDWriteChar(char c);
void LCDWriteString(char * str);


#endif	/* INIT_H */

