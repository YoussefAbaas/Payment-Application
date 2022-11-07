/*
 * server.c
 *
 *  Created on: Nov 2, 2022
 *      Author: Yousif
 */
#include "server.h"
#include "stdio.h"
#include "stdint.h"

ST_accountsDB_t accountsDB[255] = { { 2000.0, RUNNING, "4772274182937034" }, {
		30000.0, RUNNING, "6062751115280397" }, { 15000.0, RUNNING,
		"4071928704976471" }, { 100000.0, RUNNING, "2925695425025221" }, {
		20000.0, RUNNING, "5698544142566815" }, { 10000.0, BLOCKED,
		"5344711877603385" }, { 2500.0, RUNNING, "0065838013702968" }, {
		150000.0, BLOCKED, "8844379755614124" }, { 3000.0, RUNNING,
		"8989374615436851" }, { 10000.0, BLOCKED, "5807007076043875" }, };

ST_transaction_t transactions[255] = { 0 };
uint8_t transaction_num = 0;

EN_transState_t recieveTransactionData(ST_transaction_t *transData) {
	EN_transState_t trans_state;
	EN_serverError_t server_state;
	ST_accountsDB_t account_ref;
	server_state = isValidAccount(&(transData->cardHolderData), &account_ref);
	if (server_state == ACCOUNT_NOT_FOUND) {
		trans_state = FRAUD_CARD;
		transData->transState = trans_state;
		server_state = saveTransaction(transData);
		return trans_state;
	}
	server_state = isAmountAvailable(&(transData->terminalData), &account_ref);
	if (server_state == LOW_BALANCE) {
		trans_state = DECLINED_INSUFFECIENT_FUND;
		transData->transState = trans_state;
		server_state = saveTransaction(transData);
		return trans_state;
	}
	server_state = isBlockedAccount(&account_ref);
	if (server_state == BLOCKED_ACCOUNT) {
		trans_state = DECLINED_STOLEN_CARD;
		transData->transState = trans_state;
		server_state = saveTransaction(transData);
		return trans_state;
	}
	if (server_state == SERVER_OK) {
		trans_state = APPROVED;
		account_ref.balance -= transData->terminalData.transAmount;
		transData->transState = trans_state;
		server_state = saveTransaction(transData);
		return trans_state;
	}

}
void recieveTransactionDataTest(void) {
	printf("Tester Name: Youssef\n");
	printf("Function Name: %s\n", __func__);
	ST_transaction_t transData;
	EN_transState_t state;

	const char *states[] = { "APPROVED", "DECLINED_INSUFFECIENT_FUND",
			"DECLINED_STOLEN_CARD", "FRAUD_CARD", "INTERNAL_SERVER_ERROR" };

	printf("Test case 1\n");
	strcpy(transData.cardHolderData.primaryAccountNumber, "6062751215580397");
	state = recieveTransactionData(&transData);
	printf("Input text : Invalid Account Number\n");
	printf("Expected result : %s\n", states[FRAUD_CARD]);
	printf("Actual result : %s\n", states[state]);

	printf("Test case 2\n");
	strcpy(transData.cardHolderData.primaryAccountNumber, "4772274182937034");
	transData.terminalData.transAmount = 3000;
	state = recieveTransactionData(&transData);
	printf("Input text : valid Account Number with invalid amount\n");
	printf("Expected result : %s\n", states[DECLINED_INSUFFECIENT_FUND]);
	printf("Actual result : %s\n", states[state]);

	printf("Test case 3\n");
	strcpy(transData.cardHolderData.primaryAccountNumber, "8844379755614124");
	transData.terminalData.transAmount = 1000;
	state = recieveTransactionData(&transData);
	printf(
			"Input text : valid Account Number , valid Amount but account in blocked\n");
	printf("Expected result : %s\n", states[DECLINED_STOLEN_CARD]);
	printf("Actual result : %s\n", states[state]);

	printf("Test case 4\n");
	strcpy(transData.cardHolderData.primaryAccountNumber, "4772274182937034");
	transData.terminalData.transAmount = 1000;
	state = recieveTransactionData(&transData);
	printf("Input text : valid Account Number with valid amount\n");
	printf("Expected result : %s\n", states[APPROVED]);
	printf("Actual result : %s\n", states[state]);

}
EN_serverError_t isValidAccount(ST_cardData_t *cardData,
		ST_accountsDB_t *accountRefrence) {
	EN_serverError_t state;
	int account_index = -1;
	for (int i = 0; i < 10; i++) {
		if (!strcmp(cardData->primaryAccountNumber,
				accountsDB[i].primaryAccountNumber)) {
			account_index = i;
			break;
		}
	}
	if (account_index == -1) {
		state = ACCOUNT_NOT_FOUND;
		accountRefrence = NULL;
		return state;
	} else {
		state = SERVER_OK;
		*accountRefrence = accountsDB[account_index];
		return state;
	}
}

void isValidAccountTest(void) {
	printf("Tester Name: Youssef\n");
	printf("Function Name: %s\n", __func__);
	ST_cardData_t data;
	EN_serverError_t error;
	ST_accountsDB_t account_reference;

	const char *errors[] = { "SERVER_OK", "SAVING_FAILED",
			"TRANSACTION_NOT_FOUND", "ACCOUNT_NOT_FOUND", "LOW_BALANCE",
			"BLOCKED_ACCOUNT" };

	printf("Test case 1\n");
	strcpy(data.primaryAccountNumber, "5807007076043875");
	error = isValidAccount(&data, &account_reference);
	printf("Input text : %s\n", data.primaryAccountNumber);
	printf("Expected result : %s\n", errors[0]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 2\n");
	strcpy(data.primaryAccountNumber, "6062851116580497");
	error = isValidAccount(&data, &account_reference);
	printf("Input text : %s\n", data.primaryAccountNumber);
	printf("Expected result : %s\n", errors[3]);
	printf("Actual result : %s\n", errors[error]);
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence) {
	if (accountRefrence->state == BLOCKED)
		return BLOCKED_ACCOUNT;
	else
		return SERVER_OK;
}
void isBlockedAccountTest(void) {
	printf("Tester Name: Youssef\n");
	printf("Function Name: %s\n", __func__);
	EN_serverError_t error;
	ST_accountsDB_t *account_reference;

	const char *errors[] = { "SERVER_OK", "SAVING_FAILED",
			"TRANSACTION_NOT_FOUND", "ACCOUNT_NOT_FOUND", "LOW_BALANCE",
			"BLOCKED_ACCOUNT" };

	printf("Test case 1\n");
	error = isBlockedAccount(&accountsDB[1]);
	printf("Input text : Account index %d\n", 1);
	printf("Expected result : %s\n", errors[0]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 2\n");
	error = isBlockedAccount(&accountsDB[9]);
	printf("Input text : Account index %d\n", 9);
	printf("Expected result : %s\n", errors[5]);
	printf("Actual result : %s\n", errors[error]);

}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData,
		ST_accountsDB_t *accountRefrence) {
	if (termData->transAmount > accountRefrence->balance) {
		return LOW_BALANCE;
	} else {
		return SERVER_OK;
	}
}
void isAmountAvailableTest(void) {
	printf("Tester Name: Youssef\n");
	printf("Function Name: %s\n", __func__);
	EN_serverError_t error;
	ST_terminalData_t termData;
	termData.transAmount = 5000;

	const char *errors[] = { "SERVER_OK", "SAVING_FAILED",
			"TRANSACTION_NOT_FOUND", "ACCOUNT_NOT_FOUND", "LOW_BALANCE",
			"BLOCKED_ACCOUNT" };

	printf("Test case 1\n");
	error = isAmountAvailable(&termData, &accountsDB[0]);
	printf("Input text : Transaction amount %f Account balance %f\n",
			termData.transAmount, accountsDB[0].balance);
	printf("Expected result : %s\n", errors[4]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 2\n");
	error = isAmountAvailable(&termData, &accountsDB[2]);
	printf("Input text : Transaction amount %f Account balance %f\n",
			termData.transAmount, accountsDB[2].balance);
	printf("Expected result : %s\n", errors[0]);
	printf("Actual result : %s\n", errors[error]);

}
EN_serverError_t saveTransaction(ST_transaction_t *transData) {
	transData->transactionSequenceNumber = transaction_num;
	transactions[transaction_num].cardHolderData = transData->cardHolderData;
	transactions[transaction_num].terminalData = transData->terminalData;
	transactions[transaction_num].transState = transData->transState;
	transactions[transaction_num].transactionSequenceNumber =
			transData->transactionSequenceNumber;
	transaction_num++;
	return SERVER_OK;
}
void saveTransactionTest(void) {
	printf("Tester Name: Youssef\n");
	printf("Function Name: %s\n", __func__);
	ST_transaction_t transData;
	EN_serverError_t error;
	const char *errors[] = { "SERVER_OK", "SAVING_FAILED",
			"TRANSACTION_NOT_FOUND", "ACCOUNT_NOT_FOUND", "LOW_BALANCE",
			"BLOCKED_ACCOUNT" };
	strcpy(transData.cardHolderData.cardExpirationDate, "05/25");
	strcpy(transData.cardHolderData.cardHolderName, "YoussefAbbasYousseef");
	strcpy(transData.cardHolderData.primaryAccountNumber, "4772274182937034");
	transData.terminalData.transAmount = 100;
	strcpy(transData.terminalData.transactionDate, "25/10/2022");
	transData.terminalData.maxTransAmount = 500;
	error = saveTransaction(&transData);
	printf("Test case 1\n");
	printf("Input text : New Trasaction with sequence %d\n",
			transData.transactionSequenceNumber);
	printf("Expected result : %s\n", errors[0]);
	printf("Actual result : %s\n", errors[error]);
}

void listSavedTransactions(void) {
	char *trans_states[] = { "APPROVED", "DECLINED_INSUFFECIENT_FUND",
			"DECLINED_STOLEN_CARD", "FRAUD_CARD", "INTERNAL_SERVER_ERROR" };
	for (int i = 0; i < transaction_num; i++) {
		printf(
				"#########################\n Transaction Sequence Number:%d\n Transaction Date:%s\n Transaction Amount:%f\n Transaction State:%s\n Terminal Max Amount:%f\n Cardholder Name:%s\n PAN:%s\n Card Expiration Date:%s\n #########################\n",
				transactions[i].transactionSequenceNumber,
				transactions[i].terminalData.transactionDate,
				transactions[i].terminalData.transAmount,
				trans_states[transactions[i].transState],
				transactions[i].terminalData.maxTransAmount,
				transactions[i].cardHolderData.cardHolderName,
				transactions[i].cardHolderData.primaryAccountNumber,
				transactions[i].cardHolderData.cardExpirationDate);
	}
}
void listSavedTransactionsTest(void) {
	printf("Tester Name: Youssef\n");
	printf("Function Name: %s\n", __func__);
	ST_transaction_t transData;
	strcpy(transData.cardHolderData.cardExpirationDate, "05/25");
	strcpy(transData.cardHolderData.cardHolderName, "YoussefAbbasYousseef");
	strcpy(transData.cardHolderData.primaryAccountNumber, "4772274182937034");
	transData.terminalData.transAmount = 100;
	strcpy(transData.terminalData.transactionDate, "25/10/2022");
	transData.terminalData.maxTransAmount = 500;
	saveTransaction(&transData);
	printf("Test case 1\n");
	printf("Input text : None \n");
	printf("Expected result : Saved Transactions data\n");
	printf("Actual result : \n");
	listSavedTransactions();

}
