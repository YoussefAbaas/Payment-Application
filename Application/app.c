/*
 * app.c
 *
 *  Created on: Nov 2, 2022
 *      Author: Yousif
 */
#include "app.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Server/server.h"

void appStart(void) {
	ST_cardData_t cardData;
	ST_terminalData_t termData;
	EN_terminalError_t termError;
	EN_transState_t transState;
	ST_transaction_t transData;
	EN_cardError_t cardError;
	cardError = getCardHolderName(&cardData);
	if (cardError == WRONG_NAME) {
		printf("Wrong Name\n");
		return;
	}
	cardError = getCardExpiryDate(&cardData);
	if (cardError == WRONG_EXP_DATE) {
		printf("Wrong expiration date\n");
		return;
	}
	cardError = getCardPAN(&cardData);
	if (cardError == WRONG_PAN) {
		printf("Wrong Pan\n");
		return;
	}
	getTransactionDate(&termData);
	termError = isCardExpired(&cardData, &termData);
	if (termError == EXPIRED_CARD) {
		printf("Declined Expired Card\n");
		return;
	}
	cardError=CARD_OK;
	setMaxAmount(&termData, 3000);
	getTransactionAmount(&termData);
	termError = isBelowMaxAmount(&termData);
	if (termError == EXCEED_MAX_AMOUNT) {
		printf("Declined Amount Exceeding Limit\n");
		return;
	}

	transData.cardHolderData = cardData;
	transData.terminalData = termData;
	transState = recieveTransactionData(&transData);
	listSavedTransactions();
	if (transState == FRAUD_CARD) {
		//printf("Declined Invalid account\n");
		return;
	}
	if (transState == DECLINED_INSUFFECIENT_FUND) {
		//printf("Declined Insufficient funds\n");
		return;
	}
	if (transState == DECLINED_STOLEN_CARD) {
		//printf("Declined Blocked Account\n");
		return;
	}

	printf("Transaction is saved successfully\n");

}
