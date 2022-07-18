#include "card.h"
#include <string.h>

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
    printf("Please enter card holder name: ");
    gets(cardData->cardHolderName);

    if ((cardData->cardHolderName[0] == '\0') || (strlen(cardData->cardHolderName) < 20))
    {
        return WRONG_NAME;
    }
    return OK_c;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
    printf("Please enter card Expiry Date (MM/YY) : ");
    gets(cardData->cardExpirationDate);

    if ((cardData->cardExpirationDate[0] == '\0') || (strlen(cardData->cardExpirationDate) < 5) || (strlen(cardData->cardExpirationDate) > 5) || ((cardData->cardExpirationDate[0] + cardData->cardExpirationDate[1]) < 97) || ((cardData->cardExpirationDate[0] + cardData->cardExpirationDate[1]) > 105) || (cardData->cardExpirationDate[2] != 47))
    {
        return WRONG_EXP_DATE;
    }
    return OK_c;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
    printf("Please enter account number:\n");
    gets(cardData->primaryAccountNumber);

    if ((cardData->primaryAccountNumber[0] == '\0') || (strlen(cardData->primaryAccountNumber) < 16) || (strlen(cardData->primaryAccountNumber) > 19))
    {
        return WRONG_PAN;
    }
    return OK_c;
}