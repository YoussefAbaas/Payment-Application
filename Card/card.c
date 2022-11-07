/*
 * card.c
 *
 *  Created on: Nov 2, 2022
 *      Author: Yousif
 */
#include "card.h"
#include "stdio.h"
#include "stdint.h"

static uint8_t card_name[25];
static uint8_t card_expiry_date[10];
static uint8_t card_pan[25];
static uint8_t test_mode = 0;
EN_cardError_t getCardHolderName(ST_cardData_t *cardData) {
	EN_cardError_t error;
	if (test_mode == 0) {
		printf("Please enter card name ");
		fflush(stdin);
		gets(card_name);
		fflush(stdin);
	}
	if (strlen(card_name) == 0 || strlen(card_name) < 20
			|| strlen(card_name) > 24) {
		error = WRONG_NAME;
	} else {
		strcpy(cardData->cardHolderName, card_name);
		error = CARD_OK;
	}
	return error;
}
void getCardHolderNameTest(void) {
	printf("Tester Name: Youssef\n");
	printf("Function Name: %s\n", __func__);
	ST_cardData_t data;
	EN_cardError_t error;
	const char *errors[] = { "CARD_OK", "WRONG_NAME", "WRONG_EXP_DATE",
			"WRONG_PAN" };
	test_mode = 1;

	printf("Test case 1\n");
	strcpy(card_name, "youssefabbas");
	error = getCardHolderName(&data);
	printf("Input text : %s\n", card_name);
	printf("Expected result : %s\n", errors[1]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 2\n");
	strcpy(card_name, "youssefabbasyoussefahmedd");
	error = getCardHolderName(&data);
	printf("Input text : %s\n", card_name);
	printf("Expected result : %s\n", errors[1]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 3\n");
	strcpy(card_name, "");
	error = getCardHolderName(&data);
	printf("Input text : %s\n", card_name);
	printf("Expected result : %s\n", errors[1]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 4\n");
	strcpy(card_name, "youssefabbasahmeeddd");
	error = getCardHolderName(&data);
	printf("Input text : %s\n", card_name);
	printf("Expected result : %s\n", errors[0]);
	printf("Actual result : %s\n", errors[error]);

}
uint8_t check_valid_expirydate(uint8_t *expiry_date) {
	uint8_t month[2];
	month[0] = expiry_date[0];
	month[1] = expiry_date[1];
	uint8_t *year = expiry_date + 3;
	return atoi(month) >= 1 && atoi(month) <= 12 && expiry_date[2] == '/'
			&& atoi(year) >= 1 && atoi(year) <= 30;
}
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData) {

	EN_cardError_t error;
	if (test_mode == 0) {
		printf("Please enter card expiry date :");
		gets(card_expiry_date);
	}
	if (strlen(card_expiry_date) == 0 || strlen(card_expiry_date) != 5
			|| !check_valid_expirydate(card_expiry_date)) {
		error = WRONG_EXP_DATE;
	} else {
		strcpy(cardData->cardExpirationDate, card_expiry_date);
		error = CARD_OK;
	}
	return error;
}
void getCardExpiryDateTest(void) {

	printf("Tester Name: Youssef\n");
	printf("Function Name: %s\n", __func__);
	ST_cardData_t data;
	EN_cardError_t error;
	const char *errors[] = { "CARD_OK", "WRONG_NAME", "WRONG_EXP_DATE",
			"WRONG_PAN" };
	test_mode = 1;

	printf("Test case 1\n");
	strcpy(card_expiry_date, "");
	error = getCardExpiryDate(&data);
	printf("Input text : %s\n", card_expiry_date);
	printf("Expected result : %s\n", errors[2]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 2\n");
	strcpy(card_expiry_date, "05/255");
	error = getCardExpiryDate(&data);
	printf("Input text : %s\n", card_expiry_date);
	printf("Expected result : %s\n", errors[2]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 3\n");
	strcpy(card_expiry_date, "13/15");
	error = getCardExpiryDate(&data);
	printf("Input text : %s\n", card_expiry_date);
	printf("Expected result : %s\n", errors[2]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 4\n");
	strcpy(card_expiry_date, "05/25");
	error = getCardExpiryDate(&data);
	printf("Input text : %s\n", card_expiry_date);
	printf("Expected result : %s\n", errors[0]);
	printf("Actual result : %s\n", errors[error]);

}
EN_cardError_t getCardPAN(ST_cardData_t *cardData) {
	EN_cardError_t error;
	if (test_mode == 0) {
		printf("Please enter primary account number ");
		gets(card_pan);
	}
	if (strlen(card_pan) == 0 || strlen(card_pan) < 16
			|| strlen(card_pan) > 19) {
		error = WRONG_PAN;
	} else {
		strcpy(cardData->primaryAccountNumber, card_pan);
		error = CARD_OK;
	}
	return error;
}
void getCardPANTest(void) {
	printf("Tester Name: Youssef\n");
	printf("Function Name: %s\n", __func__);
	ST_cardData_t data;
	EN_cardError_t error;
	const char *errors[] = { "CARD_OK", "WRONG_NAME", "WRONG_EXP_DATE",
			"WRONG_PAN" };
	test_mode = 1;

	printf("Test case 1\n");
	strcpy(card_pan, "12345678912345678");
	error = getCardPAN(&data);
	printf("Input text : %s\n", card_pan);
	printf("Expected result : %s\n", errors[0]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 2\n");
	strcpy(card_pan, "12345678998712345678");
	error = getCardPAN(&data);
	printf("Input text : %s\n", card_pan);
	printf("Expected result : %s\n", errors[3]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 3\n");
	strcpy(card_pan, "12345678");
	error = getCardPAN(&data);
	printf("Input text : %s\n", card_pan);
	printf("Expected result : %s\n", errors[3]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 4\n");
	strcpy(card_pan, "");
	error = getCardPAN(&data);
	printf("Input text : %s\n", card_pan);
	printf("Expected result : %s\n", errors[3]);
	printf("Actual result : %s\n", errors[error]);
}
