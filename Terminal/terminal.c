/*
 * terminal.c
 *
 *  Created on: Nov 2, 2022
 *      Author: Yousif
 */
#include "terminal.h"

#include "stdio.h"
#include "stdint.h"
#include "../Card/card.h"

static uint8_t test_mode = 0;
static uint8_t transactionDate[20];
static float transAmount;

uint8_t check_valid_transactionDate(uint8_t *date) {
	uint8_t day[2];
	day[0] = date[0];
	day[1] = date[1];
	uint8_t month[3];
	month[0] = date[3];
	month[1] = date[4];
	uint8_t *year = date + 6;
	return atoi(month) >= 1 && atoi(month) <= 12 && date[2] == '/'
			&& atoi(day) >= 1 && atoi(day) <= 31 && date[5] == '/'
			&& atoi(year) >= 2022;
}

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData) {
	EN_terminalError_t error;
	if (test_mode == 0) {
		printf("Please enter Transaction date in format DD/MM/YYYY ");
		gets(transactionDate);
	}
	if (strlen(transactionDate) == 0 || strlen(transactionDate) < 10
			|| !check_valid_transactionDate(transactionDate)) {

		error = WRONG_DATE;
	} else {
		strcpy(termData->transactionDate, transactionDate);
		error = TERMINAL_OK;
	}
	return error;
}
void getTransactionDateTest(void) {
	printf("Tester Name: Youssef\n");
	printf("Function Name: %s\n", __func__);
	ST_terminalData_t data;
	EN_terminalError_t error;
	const char *errors[] = { "TERMINAL_OK", "WRONG_DATE", "EXPIRED_CARD",
			"INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT",
			"INVALID_MAX_AMOUNT" };
	test_mode = 1;

	printf("Test case 1\n");
	strcpy(transactionDate, "");
	error = getTransactionDate(&data);
	printf("Input text : %s\n", transactionDate);
	printf("Expected result : %s\n", errors[1]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 2\n"); //less than 10 char
	strcpy(transactionDate, "25/09/22");
	error = getTransactionDate(&data);
	printf("Input text : %s\n", transactionDate);
	printf("Expected result : %s\n", errors[1]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 3\n"); //wrong day
	strcpy(transactionDate, "35/10/2022");
	error = getTransactionDate(&data);
	printf("Input text : %s\n", transactionDate);
	printf("Expected result : %s\n", errors[1]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 4\n"); //wrong month
	strcpy(transactionDate, "22/14/2023");
	error = getTransactionDate(&data);
	printf("Input text : %s\n", transactionDate);
	printf("Expected result : %s\n", errors[1]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 5\n"); //wrong year
	strcpy(transactionDate, "22/03/2020");
	error = getTransactionDate(&data);
	printf("Input text : %s\n", transactionDate);
	printf("Expected result : %s\n", errors[1]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 6\n"); //correct format
	strcpy(transactionDate, "03/11/2022");
	error = getTransactionDate(&data);
	printf("Input text : %s\n", transactionDate);
	printf("Expected result : %s\n", errors[0]);
	printf("Actual result : %s\n", errors[error]);
}
EN_terminalError_t isCardExpired(ST_cardData_t *cardData,
		ST_terminalData_t *termData) {
	uint8_t *card_expiry_date = cardData->cardExpirationDate;
	uint8_t *transaction_date = termData->transactionDate;
	uint8_t expiry_date_month[3];
	expiry_date_month[0] = card_expiry_date[0];
	expiry_date_month[1] = card_expiry_date[1];
	expiry_date_month[2] = '\0';
	uint8_t *expiry_date_year = card_expiry_date + 3;

	uint8_t transaction_date_month[3];
	transaction_date_month[0] = transaction_date[3];
	transaction_date_month[1] = transaction_date[4];
	transaction_date_month[2] = '\0';
	uint8_t *transaction_date_year = transaction_date + 8;

	EN_terminalError_t error;
	if ((atoi(expiry_date_year) > atoi(transaction_date_year))
			|| ((atoi(expiry_date_year) == atoi(transaction_date_year))
					&& (atoi(expiry_date_month) >= atoi(transaction_date_month)))) {
		error = TERMINAL_OK;
	} else {
		error = EXPIRED_CARD;
	}
	return error;

}
void isCardExpriedTest(void) {
	printf("Tester Name: Youssef\n");
	printf("Function Name: %s\n", __func__);
	ST_terminalData_t terminal_data;
	ST_cardData_t card_data;
	EN_terminalError_t error;
	const char *errors[] = { "TERMINAL_OK", "WRONG_DATE", "EXPIRED_CARD",
			"INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT",
			"INVALID_MAX_AMOUNT" };
	test_mode = 1;

	printf("Test case 1\n");
	strcpy(card_data.cardExpirationDate, "05/25");
	strcpy(terminal_data.transactionDate, "26/06/2026");
	error = isCardExpired(&card_data, &terminal_data);
	printf("Input text : Card Expiry Date %s Transaction Date %s\n",
			card_data.cardExpirationDate, terminal_data.transactionDate);
	printf("Expected result : %s\n", errors[2]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 2\n");
	strcpy(card_data.cardExpirationDate, "05/25");
	strcpy(terminal_data.transactionDate, "16/10/2024");
	error = isCardExpired(&card_data, &terminal_data);
	printf("Input text : Card Expiry Date %s Transaction Date %s\n",
			card_data.cardExpirationDate, terminal_data.transactionDate);
	printf("Expected result : %s\n", errors[0]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 3\n");
	strcpy(card_data.cardExpirationDate, "05/25");
	strcpy(terminal_data.transactionDate, "26/05/2025");
	error = isCardExpired(&card_data, &terminal_data);
	printf("Input text : Card Expiry Date %s Transaction Date %s\n",
			card_data.cardExpirationDate, terminal_data.transactionDate);
	printf("Expected result : %s\n", errors[0]);
	printf("Actual result : %s\n", errors[error]);

}
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData) {
	EN_terminalError_t error;
	if (test_mode == 0) {
		printf("Please enter Transaction Amount ");
		scanf("%f", &transAmount);
	}
	if (transAmount <= 0) {
		error = INVALID_AMOUNT;
	} else {
		termData->transAmount = transAmount;
		error = TERMINAL_OK;
	}
	return error;
}
void getTransactionAmountTest(void) {
	printf("Tester Name: Youssef\n");
	printf("Function Name: %s\n", __func__);
	ST_terminalData_t data;
	EN_terminalError_t error;
	const char *errors[] = { "TERMINAL_OK", "WRONG_DATE", "EXPIRED_CARD",
			"INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT",
			"INVALID_MAX_AMOUNT" };
	test_mode = 1;

	printf("Test case 1\n");
	transAmount = -10;
	error = getTransactionAmount(&data);
	printf("Input text : %f\n", transAmount);
	printf("Expected result : %s\n", errors[4]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 2\n");
	transAmount = 20;
	error = getTransactionAmount(&data);
	printf("Input text : %f\n", transAmount);
	printf("Expected result : %s\n", errors[0]);
	printf("Actual result : %s\n", errors[error]);

}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData) {
	EN_terminalError_t error;
	if (termData->transAmount > termData->maxTransAmount)
		error = EXCEED_MAX_AMOUNT;
	else
		error = TERMINAL_OK;
	return error;
}
void isBelowMaxAmountTest(void) {
	printf("Tester Name: Youssef\n");
	printf("Function Name: %s\n", __func__);
	ST_terminalData_t data;
	EN_terminalError_t error;
	const char *errors[] = { "TERMINAL_OK", "WRONG_DATE", "EXPIRED_CARD",
			"INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT",
			"INVALID_MAX_AMOUNT" };
	test_mode = 1;

	printf("Test case 1\n");
	data.transAmount = 20.5;
	data.maxTransAmount = 30;
	error = isBelowMaxAmount(&data);
	printf("Input text : transaction amount %f max amount %f\n",
			data.transAmount, data.maxTransAmount);
	printf("Expected result : %s\n", errors[0]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 2\n");
	data.transAmount = 100;
	data.maxTransAmount = 30;
	error = isBelowMaxAmount(&data);
	printf("Input text : transaction amount %f max amount %f\n",
			data.transAmount, data.maxTransAmount);
	printf("Expected result : %s\n", errors[5]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 3\n");
	data.transAmount = 30;
	data.maxTransAmount = 30;
	error = isBelowMaxAmount(&data);
	printf("Input text : transaction amount %f max amount %f\n",
			data.transAmount, data.maxTransAmount);
	printf("Expected result : %s\n", errors[0]);
	printf("Actual result : %s\n", errors[error]);
}
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount) {
	EN_terminalError_t error;
	if (maxAmount >= 0) {
		error = TERMINAL_OK;
		termData->maxTransAmount = maxAmount;
	} else {
		error = INVALID_MAX_AMOUNT;
	}
	return error;
}
void setMaxAmountTest(void) {
	printf("Tester Name: Youssef\n");
	printf("Function Name: %s\n", __func__);
	ST_terminalData_t data;
	EN_terminalError_t error;
	float max_amount;
	const char *errors[] = { "TERMINAL_OK", "WRONG_DATE", "EXPIRED_CARD",
			"INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT",
			"INVALID_MAX_AMOUNT" };
	test_mode = 1;

	printf("Test case 1\n");
	max_amount = -30;
	error = setMaxAmount(&data, max_amount);
	printf("Input text : %f\n", max_amount);
	printf("Expected result : %s\n", errors[6]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 2\n");
	max_amount = 100;
	error = setMaxAmount(&data, max_amount);
	printf("Input text : %f\n", max_amount);
	printf("Expected result : %s\n", errors[0]);
	printf("Actual result : %s\n", errors[error]);
}
