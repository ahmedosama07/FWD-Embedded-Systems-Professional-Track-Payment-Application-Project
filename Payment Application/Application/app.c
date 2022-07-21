#define _CRT_SECURE_NO_WARNINGS 1

#include "app.h"
#include "..\Card\card.h"
#include "..\Terminal\terminal.h"
#include "..\Server\server.h"

#include <stdio.h>
#include <string.h>

void appStart(void)
{
	ST_transaction_t transaction;
	ST_transaction_t* transactionptr = &transaction;

	setMaxAmount(&(transactionptr->terminalData), 5000);
	

	uint8_t another;
	another = 'y';
	
	

	do
	{
		getTransactionDate(&(transactionptr->terminalData));
		
		recieveTransactionData(transactionptr);
		uint32_t i = 0;
		for (i = 0; i < DBMAXSIZE; i++)
		{
			if ((strcmp(transactionptr->cardHolderData.primaryAccountNumber, accountsDB[i].primaryAccountNumber) == 0) && isCardExpired(transactionptr->cardHolderData, transactionptr->terminalData) == OK_t)
			{
				printf("%s\t\t%f\n", accountsDB[i].primaryAccountNumber, accountsDB[i].balance);
				printf("Transaction number: %d\n", sequence - 1);
			}
		}

		uint8_t c;
		printf("Do you want to show transaction details (y/n) : ");
		scanf(" %c", &c);

		if (c == 'y')
		{
			uint32_t num;
			printf("Enter the sequence of the transaction: ");
			scanf("%d", &num);

			getTransaction(num, transactionsDB);
		}

		printf("\n\nDo you want another transaction?\n(y/n): ");
		scanf(" %c", &another);
	} while (another == 'y');

	
}