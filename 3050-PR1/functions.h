#include <stdio.h>
#include <stdlib.h>

#define NAME_SIZE 101
#define BUFFER_SIZE 204
#define PRIME_MULTIPLIER 137
#define PRIME_TABLE_SIZE 109
#define COMMAND_OFFSET 2

typedef struct Person Person;
typedef struct Friend Friend;
typedef Friend* Friends;
typedef Person* People;
typedef People* Group;
typedef char* String;
typedef String* Strings;
typedef void* Any;
typedef int* Array;

struct Person {
	char name[101];
	Friends friends;
	People next;
};

struct Friend {
	People person;
	Friends next;
};

void setName(People person, String name);
void input(String buffer, String message);
int compareName(String one, String two);
Any makeArray(int size, Any any, int data_size);
int getsize(Any any);
void printArray(Any any, String type);
void freeArray(Any any);
void printName(People person);
People makePerson(String buffer);
int strHash(String name);
void tableAdd(Group table, People person);
void printPersonTable(Group table);
void setFriend(Group table, String name1, String name2);
Friends getFriends(Group table, String name);
People getPerson(Group table, String name);
int areFriends(Group table, String one, String two);
void addFriends(Group table, String one, String two);
int personExists(Group table, String name);
int peopleExist(Group table, int count, ...);
void addFriend(People person1, People person2);
String splitString(String buffer);
void remFriends(Group table, String one, String two);
void remFriend(People person1, People person2);
void printFriends(Group table, String name);











