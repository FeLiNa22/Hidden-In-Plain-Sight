#include <stdlib.h>
#include <stdio.h>
#include "../inc/utils.h"

steno_data *create_steno_elem(void) {
    steno_data *elem = malloc(sizeof(steno_data));
    if (elem == NULL) {
        perror("Error retrieving meta data from BMP");
        exit(EXIT_FAILURE);
    }
    return elem;
}

void delete_steno_elem(steno_data *elem) {
    // free decoded data
    free(elem->data);
    free(elem);
}

