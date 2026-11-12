#ifndef DOG_H
#define DOG_H

/**
 * struct dog - structure containing information about a dog
 * @name: the name of the dog
 * @age: the age of the dog
 * @owner: the name of the dog's owner
 *
 * Description: This structure stores basic details about a dog,
 * including its name, age, and owner.
 */
struct dog
{
	char *name;
	float age;
	char *owner;
};

#endif