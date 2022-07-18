#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include "Card/card.h"
#include "Terminal/terminal.h"
#include "Server/server.h"
#include "Application/app.h"



int main()
{
    fillDB(transactionsDB, DBMAXSIZE);

    appStart();

    return 0;
}
