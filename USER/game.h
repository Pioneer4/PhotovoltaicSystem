/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : game.h
*Author         : Walter
*Version        : v1.0
*Date           : 2018-11-24
*Description    : ���ļ�������game.h�����еĺ���ԭ�� 
*Function List  : 
*History        :
*************************************************************/
#ifndef __GAME_H
#define __GAME_H


void HmiChatGame(void);
void CreateFile(u8 serialNum);
void HeaderInfoSava(u8 serialNum);
void PhoVolDataSava(u8 serialNum);
u8 GetStringLen(char *str);

#endif

