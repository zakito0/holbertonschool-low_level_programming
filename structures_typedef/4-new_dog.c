#include "dog.h"
#include <stdlib.h>
#include <string.h>

/**
 * new_dog - creates a new dog_t instance
 * @name: name of the dog
 * @age: age of the dog
 * @owner: owner of the dog
 *
 * Return: pointer to the newly created dog_t instance, or NULL on failure
 */
dog_t *new_dog(char *name, float age, char *owner)
{
	dog_t *new_dog_instance = malloc(sizeof(dog_t));

	if (new_dog_instance == NULL)
		return (NULL);

	new_dog_instance->name = malloc(sizeof(char) * (strlen(name) + 1));
	if (new_dog_instance->name == NULL)
	{
		free(new_dog_instance);
		return (NULL);
	}
	strcpy(new_dog_instance->name, name);

	new_dog_instance->owner = malloc(sizeof(char) * (strlen(owner) + 1));
	if (new_dog_instance->owner == NULL)
	{
		free(new_dog_instance->name);
		free(new_dog_instance);
		return (NULL);
	}
	strcpy(new_dog_instance->owner, owner);

	new_dog_instance->age = age;

	return (new_dog_instance);
}

