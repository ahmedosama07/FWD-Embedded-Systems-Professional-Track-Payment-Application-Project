#define _CRT_SECURE_NO_WARNINGS 1

#include "terminal.h"
#include <stdio.h>
#include <time.h>

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	time_t rawtime;
	struct tm* date;

	time(&rawtime);

	date = localtime(&rawtime);

	//strftime(termData->transactionDate, sizeof(termData->transactionDate) - 1, "%x", date);
	sprintf(termData->transactionDate, "%02d/%02d/%04d", date->tm_mday, date->tm_mon + 1, date->tm_year + 1900);
	//printf("Current Date: %s", termData->transactionDate);

	return OK_t;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
    uint8_t* expirationDateYears = &(cardData.cardExpirationDate[3]);
    uint8_t* expirationDateMonths = &(cardData.cardExpirationDate[0]);

	uint8_t* terminalDateYears = &(termData.transactionDate[8]);
	uint8_t* terminalDateMonths = &(termData.transactionDate[3]);
    
	uint32_t expirationDate = 100 * atoi(expirationDateYears) + atoi(expirationDateMonths);
    uint32_t terminalDate = 100 * atoi(terminalDateYears) + atoi(terminalDateMonths);

	if (expirationDate < terminalDate)
	{
		return EXPIRED_CARD;
	}

	return OK_t;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	printf("Please Enter the transaction Amount: ");
	scanf("%f", &(termData->transAmount));

	if (termData->transAmount <= 0)
	{
		return INVALID_AMOUNT;
	}

	return OK_t;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount > termData->maxTransAmount)
	{
		printf("Exceeded maximum amount\n");
		return EXCEED_MAX_AMOUNT;
	}

	return OK_t;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float max)
{
	if (max <= 0)
	{
		return INVALID_MAX_AMOUNT;
	}

	termData->maxTransAmount = max;
	return OK_t;
}