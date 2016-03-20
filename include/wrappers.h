#ifndef WRAPPERS_H
#define WRAPPERS_H

/**
 * This file holds wrappers around commonly used C functions that may throw
 * fatal errors. The error checking is just wrapped around the associated
 * C function (take out the _).
 *
 * @author Anthony Benavente
 * @version 03/19/2016
 */

#include <stdlib.h>

/**
 * Does a malloc while also checking for errors.
 *
 * @param size The amount of bytes to allocated
 * @return a pointer to the allocated data
 */
void *_malloc(size_t size);

/** 
 * Does a fork and verifies that the fork succeeded.
 */
void _fork(void);


#endif