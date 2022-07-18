#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include "Card/card.h"
#include "Terminal/terminal.h"
#include "Server/server.h"



int main()
{
    ST_cardData_t card;
    ST_terminalData_t terminal;
    printf("Hello world!\n");
    getCardExpiryDate(&card);
    getTransactionDate(&terminal);
    printf("\n%d\n", isCardExpired(card, terminal));

    getTransactionAmount(&terminal);
    setMaxAmount(&terminal, 1000);
    printf("\n%d\n", isBelowMaxAmount(&terminal));

    printf("%f\n\n", accountsDB[1].balance);
    printf("%f", transactionsDB[100].terminalData.transactionDate);
    return 0;
}
