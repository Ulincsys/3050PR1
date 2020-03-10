#include "functions.h"
#include <stdarg.h>

/* Accepts a hash table of People, frees all friend struct pointers and people pointers,
 * then frees the table itself using the freeArray() function. Calls remAllFriends().
 * Parameters:
 * Group table		- A hash table of size PRIME_TABLE_SIZE
 */
void freeHashTable(Group table) {
	int index = -1;
	while(++index < PRIME_TABLE_SIZE) {
		if(table[index]) {
			People cursor = table[index]->next;
			People temp = table[index];
			while(cursor) {
				printf("Freeing: %s\n", temp->name);
				remAllFriends(temp);
				free(temp);
				temp = cursor;
				cursor = cursor->next;
			}
			remAllFriends(temp);
			free(temp);
		}
	}
	freeArray(table);
}

/* Accepts a Person struct pointer and removes all the friend relationships of the given person
 * using the remFriend() function.
 * Parameters:
 * People person	- A pointer to a Person struct
 */
void remAllFriends(People person) {
	Friends cursor = person->friends;
	while(cursor) {
		People temp = cursor->person;
		cursor = cursor->next;
		remFriend(person, temp);
		remFriend(temp, person);
	}
}

/* Accepts two String names, removes them as friends if they both exist and are friends.
 * Makes a call to areFriends(), getPerson(), peopleExist() and remFriend().
 * Parameters:
 * Group table		- A hash table of size PRIME_TABLE_SIZE
 * String one		- A name String with no extraneous characters
 * String two		- A name String with no extraneous characters
 */
void remFriends(Group table, String one, String two) {
	if(areFriends(table, one, two) && peopleExist(table, 2, one, two)) {
		People person1 = getPerson(table, one);
		People person2 = getPerson(table, two);
		remFriend(person1, person2);
		remFriend(person2, person1);
	}
}

/* Accepts two Person struct pointers and removes the second person from the
 * friends list of the first person if they are friends.
 * Parameters
 * People person1	- A pointer to a Person struct
 * People person2	- A pointer to a Person struct
 */
void remFriend(People person1, People person2) {
	Friends cursor = person1->friends;
	if(cursor && compareName(person2->name, cursor->person->name)) {
		person1->friends = cursor->next;
		free(cursor);
		return;
	}
	Friends temp = cursor;
	while(cursor->next) {
		cursor = cursor->next;
		if(compareName(person2->name, cursor->person->name)) {
			temp->next = cursor->next;
			free(cursor);
			return;
		}
		temp = cursor;
	}
}

/* Accepts a hash table and a String name, and prints the friends list of
 * the given person if found.
 * Parameters:
 * Group table		- A hash table of size PRIME_TABLE_SIZE
 * String name		- A name String with no extraneous characters
 */
void printFriends(Group table, String name) {
	People person = getPerson(table, name);
	if(!person) {
		return;
	}
	Friends cursor = person->friends;
	if(cursor) {
		printf("%s's friends: ", name);
	} else {
		printf("%s has no friends.\n", name);
		return;
	}
	while(cursor) {
		printf("%s%s", cursor->person->name, cursor->next ? ", " : "\n");
		cursor = cursor->next;
	}
}

/* Accepts a hash table and a Person struct pointer, and adds it to the hash
 * table by the string hash of the name. Will not add a person who is already added
 * to the table.
 * Parameters:
 * Group table		- A hash table of size PRIME_TABLE_SIZE
 * People person	- A pointer to a Person struct to add to the table
 */
void tableAdd(Group table, People person) {
	if(personExists(table, person->name)) {
		return;
	}
	int hash = strHash(person->name);
	if(table[hash]) {
		person->next = table[hash];
	}
	table[hash] = person;
}

/* Accepts a String buffer containing two String names separated by a space.
 * Will replace the space in the middle of the buffer with a null terminator
 * character and return a pointer to the beginning of the second name. Will
 * return NULL if there is not a space before the end of the string.
 * Parameters:
 * String buffer	- String Buffer containing two names separated by a space.
 */
String splitString(String buffer) {
	for(int i = 0; (i < BUFFER_SIZE - 2) && (buffer[i] != 0); ++i) {
		if(buffer[i] == ' ') {
			buffer[i] = 0;
			return (buffer + (++i));
		}
	}
	return NULL;
}

/* Accepts a hash table, an integer and a list of String names and determines
 * if at least one of them does not exist. Will return 1 if all people exist,
 * else 0 if at least one does not.
 * Parameters:
 * Group table		- A hash table of size PRIME_TABLE_SIZE
 * int count		- An integer representing the number of names to check
 * String ...		- A list of String  names of size count
 */
int peopleExist(Group table, int count, ...) {
	va_list args;
	va_start(args, count);
	while(count-- > 0) {
		String name = va_arg(args, String);
		if(!personExists(table, name)) {
			va_end(args);
			return 0;
		}
	}
	va_end(args);
	return 1;
}
/*// Previous non-portable version of this function with varargs implemented manually.
 * Strings people = (Strings)(&count);
	++people;
	while(count-- > 0) {
		if(!personExists(table, people[count])) {
			return 0;
		}
	}
	return 1;
 */

/* Accepts a hash table and a String name, and uses getPerson() to determine if the person
 * name given is in the table.
 * Parameters:
 * Group table		- A hash table of size PRIME_TABLE_SIZE
 * String name		- A name String with no extraneous characters
 */
int personExists(Group table, String name) {
	return (getPerson(table, name) != NULL);
}

/* Accepts two String names, adds them as friends if they both exist and are not already friends.
 * Makes a call to areFriends(), getPerson(), peopleExist() and addFriend().
 * Parameters:
 * Group table		- A hash table of size PRIME_TABLE_SIZE
 * String one		- A name String with no extraneous characters
 * String two		- A name String with no extraneous characters
 */
void addFriends(Group table, String one, String two) {
	if(!areFriends(table, one, two) && peopleExist(table, 2, one, two)) {
		People person1 = getPerson(table, one);
		People person2 = getPerson(table, two);
		addFriend(person1, person2);
		addFriend(person2, person1);
	}
}

/* Accepts two Person struct pointers and adds the second person to the friends list of
 * the first person. Assumes the given people are not already friends.
 * Parameters
 * People person1	- A pointer to a Person struct
 * People person2	- A pointer to a Person struct
 */
void addFriend(People person1, People person2) {
	Friends cursor = person1->friends;
	Friends relationship = malloc(sizeof(Friend));
	relationship->person = person2;
	relationship->next = cursor;
	person1->friends = relationship;
}

/* Accepts a hash table and two String names, and determines if those two people exist
 * in the hash table as well as if they are friends. Will return false if either person
 * does not exist in the table, or if they are not on each others' friends list.
 * Parameters:
 * Group table		- A hash table of size PRIME_TABLE_SIZE
 * String one		- A name String with no extraneous characters
 * String two		- A name String with no extraneous characters
 */
int areFriends(Group table, String one, String two) {
	if(!peopleExist(table, 2, one, two)) {
		return 0;
	}
	Friends cursor = getFriends(table, one);
	while(cursor) {
		if(compareName(two, cursor->person->name)) {
			return 1;
		}
		cursor = cursor->next;
	}

	return 0;
}

/* Accepts two String names and compares them element by element, up to NAME_SIZE or until
 * at least one of the two names has a null terminator. Assumes 0 is false and 1 is true.
 * Parameters:
 * Group table		- A hash table of size PRIME_TABLE_SIZE
 * String one		- A name String with no extraneous characters
 * String two		- A name String with no extraneous characters
 */
int compareName(String one, String two) {
	int i;
	for(i = 0; (i < NAME_SIZE) && (one[i] || two[i]); ++i) {
		if(one[i] != two[i]) {
			return 0;
		}
	}

	return one[i] == two[i];
}

/* Accepts a hash table of People struct pointers and prints every entry, including empty slots.
 * Prints all chained entries on the same line, as they occupy the same index.
 * Parameters:
 * Group table		- A hash table of size PRIME_TABLE_SIZE
 */
void printPersonTable(Group table) {
	for(int i = 0; i < PRIME_TABLE_SIZE; ++i) {
		printf("[ ");
		People cursor = table[i];
		while(cursor) {
			printName(cursor);
			printf("%s", ((cursor->next) ? ", " : ""));
			cursor = cursor->next;
		}
		printf(" ]\n");
	}
}

/* Accepts a hash table and a String name, and returns a Person pointer to the Person struct
 * containing the given name. Will return NULL if the name does not exist in the table.
 * Group table		- A hash table of size PRIME_TABLE_SIZE
 * String name		- A name String with no extraneous characters
 */
People getPerson(Group table, String name) {
	int hash = strHash(name);
	People cursor = table[hash];
	while(cursor) {
		if(compareName(cursor->name, name)) {
			return cursor;
		}
		cursor = cursor->next;
	}

	return NULL;
}

/* Accepts a hash table and a String name, and returns a Friend pointer to the given person's
 * Friend list. Will return NULL if the given person does not exist in the table. Makes a call
 * to getPerson() in order to retrieve the Person pointer.
 * Parameters:
 * Group table		- A hash table of size PRIME_TABLE_SIZE
 * String name		- A name String with no extraneous characters
 */
Friends getFriends(Group table, String name) {
	People cursor = getPerson(table, name);
	if(cursor) {
		return cursor->friends;
	}

	return NULL;
}

/* Generates a hash integer based upon the give name String. Uses the PRIME_MULTIPLIER and
 * PRIME_TABLE_SIZE to generate an integer index for hash table insertion.
 * Parameters:
 * String name		- A name String with no extraneous characters
 */
int strHash(String name) {
	int hash = name[0];
	for(int i = 0; (name[i] != 0) && (i < NAME_SIZE); ++i) {
		hash += (i % 2) ? name[i] : name[i] * PRIME_MULTIPLIER;
	}
	return hash % PRIME_TABLE_SIZE;
}

/* Accepts a pointer to a Person struct and inserts the given name String into the name element.
 * Parameters:
 * People person	- A pointer to a Person struct
 * String name		- A name String with no extraneous characters
 */
void setName(People person, String name) {
	int i;
	for(i = 0; (i < NAME_SIZE - 1) && (name[i] != 0); ++i) {
		person->name[i] = name[i];
	}
	person->name[i] = 0;
}

/* Accepts a pointer to a Person struct and prints the name element using printf. Does not
 * implicitly add a newline at the end of the output.
 * Parameters:
 * People person	- A pointer to a Person struct
 */
void printName(People person) {
	printf("%s", person->name);
}

/* Accepts a buffer String and an output message, then reads into the buffer String provided.
 * Will only read in a string of length < BUFFER_SIZE, and does not implicitly add a newline after
 * the message String provided. Will overwrite any characters already in the buffer String. Replaces
 * return characters with null terminators. Will attempt to read from a FILE pointer allocated using
 * fopen("commands.txt", "r"), and will read from stdin if that file was not found.
 * Parameters:
 * String buffer	- String Buffer to write to from STDIN, must be at least as long as BUFFER_SIZE
 * String message	- String message to be printed during input, is not implicitly followed by newline.
 */
void input(String buffer, String message) {
	printf("%s", message);
	fgets(buffer, BUFFER_SIZE, (cmdIn) ? cmdIn : stdin);
	for(int i = 0; i < BUFFER_SIZE; ++i) {
		if((buffer[i] == '\n') || (buffer[i] == '\r')) {
			buffer[i] = 0;
		}
	}
}

/* Accepts a buffer String and generates a pointer to a Person struct with the given name.
 * The name string given is expected to be NULL terminated, with a length <= NAME_SIZE.
 * Will return NULL if malloc fails, and will segfault if passed an invalid buffer String.
 * Parameters:
 * String buffer	- Buffer containing a name string, prefixed by one command character and a space.
 */
People makePerson(String buffer) {
	People person = malloc(sizeof(Person));
	if(!person) { return NULL; }
	setName(person, buffer + COMMAND_OFFSET);
	person->friends = NULL;
	person->next = NULL;
	return person;
}

/* Takes an array pointer of any type with the size stored at the pre-index location of -1 int spaces,
 * and returns the size. Will segfault if passed a NULL or invalid pointer.
 * Parameters:
 * 	Any any 		- An array pointer which has an integer representing its size at index -1.
 */
int getsize(Any any) {
	Array temp = (Array)any;
	return temp[-1];
}

/* Creates an array with size stored at integer index -1, and also copies any given array data into
 * the newly allocated memory. Requires the size of the data type to be given. If the data array parameter
 * is NULL, the array will be initialized to 0. The data array given MUST be at least as long as the size
 * given, but any elements after the given size will be ignored. The data array given doesn't necessarily
 * need to be of the same type as the data_size given, but it must be at least the same size in bytes.
 * Will return NULL if allocating memory failed.
 * Parameters:
 * 	int size 		- The size of the array to create in terms of the number of elements.
 * 	Any any 		- An array of data to be entered into the newly created array, will be ignored if NULL.
 * 	int data_size 	- The size of the array data type (IE: use sizeof(int) for an integer array etc..).
 */
Any makeArray(int size, Any any, int data_size) {
	String init = malloc((data_size * size) + sizeof(int));
	if(!init) { return NULL; }

	Array temp = (Array)init;
	temp[0] = size;
	init += sizeof(int);

	if(any) {
		String data = (String)any;
		for(int i = 0; i < (data_size * size); ++i) {
			init[i] = data[i];
		}
	} else {
		for(int i = 0; i < (data_size * size); ++i) {
			init[i] = 0;
		}
	}
	return init;
}

/* Takes an array pointer of any type with the size stored at the pre-index location of -1 int spaces,
 * and frees the block of memory allocated to it. Will segfault if passed a NULL or invalid pointer.
 * Parameters:
 * 	Any any 		- An array pointer which has an integer representing its size at index -1.
 */
void freeArray(Any any) {
	Array temp = (Array)any;
	free(temp - 1);
}
