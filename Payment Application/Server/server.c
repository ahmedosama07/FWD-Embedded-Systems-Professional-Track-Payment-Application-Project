#define _CRT_SECURE_NO_WARNINGS 1

#include "server.h"
#include <string.h>

#define DBMAXSIZE 255

uint32_t key = 0;
uint32_t target = 0;
uint32_t sequence = 100;

ST_accountsDB_t accountsDB[DBMAXSIZE] = {
    {9000, "6250941006528599"},
    {500, "60115564485789458"},
    {300.5, "6011000991300009"},
    {1000, "1234567890123456"}
};

ST_transaction_t transactionsDB[DBMAXSIZE];

void fillDB(ST_transaction_t* transactionDB, uint32_t size)
{
    uint32_t i = 0;
    for (i = 0; i < size; i++)
    {
        strcpy(transactionDB[i].cardHolderData.cardExpirationDate, "000");
        strcpy(transactionDB[i].cardHolderData.cardHolderName, "000");
        strcpy(transactionDB[i].cardHolderData.primaryAccountNumber, "000");

        strcpy(transactionDB[i].terminalData.transactionDate, "000");

        transactionDB[i].terminalData.maxTransAmount = 0;
        transactionDB[i].terminalData.transAmount = 0;

        transactionDB[i].transactionSequenceNumber = 0;
        transactionDB[i].transState = 0;
    }
}

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
    uint8_t isValidCard;
    uint8_t isValidAmount;
    uint8_t isSaved;

    isValidCard = isValidAccount(&(transData->cardHolderData));
    isValidAmount = isAmountAvailable(&(transData->terminalData));


    if (isCardExpired(transData->cardHolderData, transData->terminalData) == EXPIRED_CARD)
    {
        puts("Expired Card");
        return DECLINED_STOLEN_CARD;
    }

    if (isValidCard == ACCOUNT_NOT_FOUND)
    {
        transData->transState = DECLINED_STOLEN_CARD;
    }
    else if (isValidAmount == LOW_BALANCE)
    {
        transData->transState = DECLINED_INSUFFECIENT_FUND;
    }
    else
    {
        transData->transState = APPROVED;
    }
    transData->transactionSequenceNumber = sequence;

    isSaved = saveTransaction(transData);

    if ((isValidCard == ACCOUNT_NOT_FOUND) || (target == 255))
    {
        printf("Stolen Card\n");

        return DECLINED_STOLEN_CARD;
    }
    else if ((isValidCard == OK) && (target != 255))
    {
        if (isValidAmount == LOW_BALANCE)
        {
            printf("Low balance\n");
            return DECLINED_INSUFFECIENT_FUND;
        }
        else if (isSaved == SAVING_FAILED)
        {
            printf("Internal Server Error\n");
            return INTERNAL_SERVER_ERROR;
        }
    }
    else if ((isBelowMaxAmount(&(transData->terminalData)) == OK_t) && (isValidCard == OK_c))
    {
        
        accountsDB[target].balance -= transData->terminalData.transAmount;
        printf("Succeeded\n");
    }
    

    return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
    uint8_t isValidName;
    uint8_t isPANRight;
    uint8_t isDateRight;

    isValidName = getCardHolderName(cardData);
    
    while (isValidName == WRONG_NAME)
    {
        isValidName = getCardHolderName(cardData);
    }

    isPANRight = getCardPAN(cardData);

    while (isPANRight == WRONG_PAN)
    {
        isPANRight = getCardPAN(cardData);
    }

    isDateRight = getCardExpiryDate(cardData);

    while (isDateRight == WRONG_EXP_DATE)
    {
        isDateRight = getCardExpiryDate(cardData);
    }

    if ((isValidName == OK_c) && (isPANRight == OK_c) && (isDateRight == OK_c))
    {
        uint32_t i = 0;
        for (i = 0; i < DBMAXSIZE; i++)
        {
            if (strcmp(cardData->primaryAccountNumber, accountsDB[i].primaryAccountNumber) == 0)
            {
                target = i;
                return OK;
            }
        }
        target = 255;
    }
    return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
    uint8_t isValidAmount;

    isValidAmount = getTransactionAmount(termData);

    if (isValidAmount == OK_t)
    {
        if (termData->transAmount > accountsDB[target].balance)
        {
            return LOW_BALANCE;
        }

        return OK;
    }
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
    uint32_t i = sequence - 100;
    
    if (i >= 255)
    {
        return SAVING_FAILED;
    }

    strcpy(transactionsDB[i].cardHolderData.cardExpirationDate, transData->cardHolderData.cardExpirationDate);
    strcpy(transactionsDB[i].cardHolderData.cardHolderName, transData->cardHolderData.cardHolderName);
    strcpy(transactionsDB[i].cardHolderData.primaryAccountNumber, transData->cardHolderData.primaryAccountNumber);

    strcpy(transactionsDB[i].terminalData.transactionDate, transData->terminalData.transactionDate);

    transactionsDB[i].terminalData.maxTransAmount = transData->terminalData.maxTransAmount;
    transactionsDB[i].terminalData.transAmount = transData->terminalData.transAmount;

    transactionsDB[i].transactionSequenceNumber = transData->transactionSequenceNumber;
    transactionsDB[i].transState = transData->transState;

    sequence++;

    return OK;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
    uint32_t i = 0;
    while (transData[i].transactionSequenceNumber != 0)
    {
        if (transData[i].transactionSequenceNumber == transactionSequenceNumber)
        {
            puts("-------------------------");
            puts("Transaction Details:");
            printf("Card Holder Name          :\t\t%s\n", transData[i].cardHolderData.cardHolderName);
            printf("Card PAN                  :\t\t%s\n", transData[i].cardHolderData.primaryAccountNumber);
            printf("Card Expiration Date      :\t\t%s\n", transData[i].cardHolderData.cardExpirationDate);

            printf("Transaction Date          :\t\t%s\n", transData[i].terminalData.transactionDate);
            printf("Transaction Amount        :\t\t%f\n", transData[i].terminalData.transAmount);
            printf("Transaction Maximum Amount:\t\t%f\n", transData[i].terminalData.maxTransAmount);

            printf("Sequence Number           :\t\t%d\n", transData[i].transactionSequenceNumber);
            printf("Transaction State         :\t\t%d\n", transData[i].transState);

            return OK;
        }
        i++;
    }
    puts("Transaction Not Found");
    return TRANSACTION_NOT_FOUND;
}