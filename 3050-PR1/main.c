#include "functions.h"

void printMenu();
void loop();

int main(void) {
	puts("Welcome to the Mini Facebook Simulator!");
	printMenu();

	loop();
	return 0;
}

void printMenu()  {
	printf("Options:\nP [name] { Add person P with name [name] (max 100 chars) }\n");
	printf("F [name] [name] { Add a relationship between two existing people }\n");
	printf("U [name] [name] { Remove a relationship between two existing people }\n");
	printf("L [name] { List all the friends of an existing person }\n");
	printf("Q [name] [name] { Check the relationship status between two people\n");
	printf("X { exit the program }\n\n");
}

void loop() {
	setvbuf(stdout, NULL, _IONBF, 0);
	String buffer = makeArray(BUFFER_SIZE, NULL, sizeof(char));
	Group table = makeArray(PRIME_TABLE_SIZE, NULL, sizeof(People));



	while(1) {
		input(buffer, "Enter an option:\n");
		switch(*buffer) {
			case 'P': {
				People person = makePerson(buffer);
				tableAdd(table, person);
				break;
			} case 'F': {
				break;
			} case 'U': {
				break;
			} case 'L': {
				break;
			} case 'Q': {
				break;
			} case 'X': {
				return;
			} case 'C': { // print the table
				printPersonTable(table);
				break;
			} case 'Z': { // check that a person exists
				printf("Person %s exists: %s\n", buffer + 2, (getPerson(table, buffer + 2)) ? "True" : "False");
				break;
			} default: {
				puts("Invalid input, please try again.");
			}
		}
	}
}
