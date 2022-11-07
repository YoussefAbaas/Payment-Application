/*
 * main.c
 *
 *  Created on: Nov 2, 2022
 *      Author: Yousif
 */
#include "Card/card.h"
#include "Terminal/terminal.h"
#include "Server/server.h"
#include "Application/app.h"
#include "stdio.h"
int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	while(1)
	{
		appStart();
	}
}
