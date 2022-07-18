#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include "..\types.h"
#include "..\Card\card.h"
#include "..\Terminal\terminal.h"

#define DBMAXSIZE 255

typedef enum EN_transState_t
{
	APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, INTERNAL_SERVER_ERROR
}EN_transState_t;

typedef enum EN_serverError_t
{
	OK, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE
}EN_serverError_t;

typedef struct ST_transaction_t
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_transState_t transState;
	uint32_t transactionSequenceNumber;
}ST_transaction_t;

typedef struct ST_accountsDB_t
{
	float balance;
	uint8_t primaryAccountNumber[20];
}ST_accountsDB_t;

EN_transState_t recieveTransactionData(ST_transaction_t* transData);
EN_serverError_t isValidAccount(ST_cardData_t* cardData);
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData);
EN_serverError_t saveTransaction(ST_transaction_t* transData);
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData);

void fillDB(ST_transaction_t* transactionDB, uint32_t size);

extern ST_accountsDB_t accountsDB[DBMAXSIZE];
extern ST_transaction_t transactionsDB[DBMAXSIZE];
extern uint32_t key;
extern uint32_t sequence;

#endif // SERVER_H_INCLUDED
