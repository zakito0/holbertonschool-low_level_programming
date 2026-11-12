#include "dog.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/**
 * _putchar - writes the character c to stdout
 * @c: The character to print
 *
 * Return: On success 1, on error -1
 */
int _putchar(char c)
{
	return (write(1, &c, 1));
}
/**
 * _strlen - returns the length of a string
 * @s: string to evaluate
 *
 * Return: the length of the string
 */
int _strlen(char *s)
{
	int len = 0;

	while (s[len] != '\0')
		len++;

	return (len);
}
/**
 * _strcpy - copies a string from src to dest
 * @dest: destination buffer
 * @src: source string
 *
 * Return: pointer to the destination string
 */
char *_strcpy(char *dest, char *src)
{
	int i = 0;

	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';

	return (dest);
}

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

new_dog_instance->name = malloc(sizeof(char) * (_strlen(name) + 1));
if (new_dog_instance->name == NULL)
{
free(new_dog_instance);
return (NULL);
}
_strcpy(new_dog_instance->name, name);

new_dog_instance->owner = malloc(sizeof(char) * (strlen(owner) + 1));
if (new_dog_instance->owner == NULL)
{
free(new_dog_instance->name);
free(new_dog_instance);
return (NULL);
}
_strcpy(new_dog_instance->owner, owner);

new_dog_instance->age = age;

return (new_dog_instance);
}


