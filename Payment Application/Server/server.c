#include "server.h"

#define DBMAXSIZE 255
ST_accountsDB_t accountsDB[DBMAXSIZE] = {
    {9000, "6250941006528599"},
    {500, "60115564485789458"},
    {300.5, "6011000991300009"}
};

ST_transaction_t transactionsDB[DBMAXSIZE] = {
    {{"000", "000", "000"}, {0, 0, "000"}, 0, 0}
};

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{

}