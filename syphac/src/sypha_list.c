/* sypha_list.c
 *
 * Copyright 2024 David Tuttle
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include <stdlib.h>
#include <memory.h>
#include "syphac/sypha_list.h"

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

struct _sypha_list_item {
    void * data;
    size_t data_sz;
    
    struct _sypha_list_item * prev;
    struct _sypha_list_item * next;
};

struct _sypha_list {
    size_t count;
    struct _sypha_list_item * first;
    struct _sypha_list_item * last;
};

struct _sypha_list_iterator {
    struct _sypha_list * list;
    struct _sypha_list_item * curr;
    unsigned int forward;
    unsigned int is_pristine;
};

SYPHA_LIST sypha_list_create() {
    struct _sypha_list * list;
    if (!(list = (struct _sypha_list *) malloc(sizeof(struct _sypha_list)))) {
        return NULL;
    }
    memset(list, 0x0, sizeof(struct _sypha_list));
    return (SYPHA_LIST) list;
}

void sypha_list_destroy(SYPHA_LIST list) {
    struct _sypha_list * _list = (struct _sypha_list *) list;
    if (!_list) {
        return;
    }

    struct _sypha_list_item * list_item_next = NULL;
    struct _sypha_list_item * list_item = _list->first;
    while (list_item) {
        list_item_next = list_item->next;
        
        free(list_item->data);
        free(list_item);

        list_item = list_item_next;
    }
}

void sypha_list_append_item(SYPHA_LIST list, void * data, size_t data_sz) {
    struct _sypha_list * _list = (struct _sypha_list *) list;
    struct _sypha_list_item * list_item;
    if (!(list_item = (struct _sypha_list_item *) malloc(sizeof(struct _sypha_list_item)))) {
        return;
    }

    if (!(list_item->data = (void *) malloc(sizeof(data_sz)))) {
        free(list_item);
        return;
    }
    memcpy(list_item->data, data, data_sz);
    list_item->data_sz = data_sz;

    // Set the new item as last, update old last to point to this
    list_item->prev = _list->last;
    list_item->next = NULL;
    if (_list->last) {
        _list->last->next = list_item;
    }
    _list->last = list_item;

    // Is it also the first item?
    if (!_list->first) {
        _list->first = list_item;
    }

    _list->count++;
}

void sypha_list_prepend_item(SYPHA_LIST list, void * data, size_t data_sz) {
    struct _sypha_list * _list = (struct _sypha_list *) list;
    struct _sypha_list_item * list_item;
    if (!(list_item = (struct _sypha_list_item *) malloc(sizeof(struct _sypha_list_item)))) {
        return;
    }

    if (!(list_item->data = (void *) malloc(sizeof(data_sz)))) {
        free(list_item);
        return;
    }
    memcpy(list_item->data, data, data_sz);
    list_item->data_sz = data_sz;

    // Set the new item as first, have it point to old first
    list_item->prev = NULL;
    list_item->next = _list->first;
    if (_list->first) {
        _list->first->prev = list_item;
    }
    _list->first = list_item;

    // Is it also the last item?
    if (!_list->last) {
        _list->last = list_item;
    }

    _list->count++;
}

SYPHA_LIST_ITERATOR sypha_list_get_iterator_front(SYPHA_LIST list) {
    struct _sypha_list * _list = (struct _sypha_list *) list;
    struct _sypha_list_iterator * iterator;
    if (!(iterator = (struct _sypha_list_iterator *) malloc(sizeof(struct _sypha_list_iterator)))) {
        return NULL;
    }

    iterator->list = _list;
    iterator->curr = _list->first;
    iterator->forward = 1;
    iterator->is_pristine = 1;

    return (SYPHA_LIST_ITERATOR) iterator;
}

SYPHA_LIST_ITERATOR sypha_list_get_iterator_back(SYPHA_LIST list) {
    struct _sypha_list * _list = (struct _sypha_list *) list;
    struct _sypha_list_iterator * iterator;
    if (!(iterator = (struct _sypha_list_iterator *) malloc(sizeof(struct _sypha_list_iterator)))) {
        return NULL;
    }

    iterator->list = _list;
    iterator->curr = _list->last;
    iterator->forward = 0;
    iterator->is_pristine = 1;

    return (SYPHA_LIST_ITERATOR) iterator;
}

void sypha_list_destroy_iterator(SYPHA_LIST_ITERATOR iterator) {
    struct _sypha_list_iterator * _iterator = (struct _sypha_list_iterator *) iterator;
    free(_iterator);
}

void * sypha_list_iterator_get(SYPHA_LIST_ITERATOR iterator, size_t * data_sz) {
    struct _sypha_list_iterator * _iterator = (struct _sypha_list_iterator *) iterator;
    struct _sypha_list_item * curr = _iterator->curr;

    // Iterator not started OR empty list
    if (_iterator->is_pristine || !curr) {
        return NULL;
    }

    *data_sz = curr->data_sz;
    return curr->data;
}

int sypha_list_iterator_next(SYPHA_LIST_ITERATOR iterator) {
    struct _sypha_list_iterator * _iterator = (struct _sypha_list_iterator *) iterator;
    struct _sypha_list_item * curr = _iterator->curr;
    struct _sypha_list_item * next;

    // Empty list case
    if (!curr) {
        return -1;
    }

    // Advance the iterator to the next, which depends on the
    // perspective of moving from front-to-back or back-to-front.
    if (!_iterator->is_pristine) {
        next = (_iterator->forward) ? curr->next : curr->prev;

        // We hit the "end", return NULL and stay where we are
        if (!next) {
            return -1;
        }

        // Fully move
        _iterator->curr = next;
    } else {
        // else we were in the initial state so don't move and clense that state
        _iterator->is_pristine = 0;
    }

    return 0;
}

int sypha_list_iterator_previous(SYPHA_LIST_ITERATOR iterator) {
    struct _sypha_list_iterator * _iterator = (struct _sypha_list_iterator *) iterator;
    struct _sypha_list_item * curr = _iterator->curr;
    struct _sypha_list_item * next;

    // Empty list case
    if (!curr) {
        return -1;
    }

    if (_iterator->is_pristine) {
        // You can't move to the previous node from the initial state
        return -1;
    }

    // Advance the iterator to the previous, which depends on the
    // perspective of moving from front-to-back or back-to-front.
    next = (_iterator->forward) ? curr->prev : curr->next;

    // We hit the "end", return NULL and stay where we are
    if (!next) {
        return -1;
    }

    // Fully move
    _iterator->curr = next;

    return 0;
}

int sypha_list_iterator_insert_after(SYPHA_LIST_ITERATOR iterator, void * data, size_t data_sz) {
    struct _sypha_list_iterator * _iterator = (struct _sypha_list_iterator *) iterator;
    struct _sypha_list_item * curr = _iterator->curr;
    struct _sypha_list * _list = (struct _sypha_list *) _iterator->list;

    // You can add an item from initial state since it is suppose to be one behind it.  So allow this
    // and leave the initial state in tact so next() still has to be called.

    struct _sypha_list_item * list_item;
    if (!(list_item = (struct _sypha_list_item *) malloc(sizeof(struct _sypha_list_item)))) {
        return -1;
    }

    if (!(list_item->data = (void *) malloc(sizeof(data_sz)))) {
        free(list_item);
        return -1;
    }
    memcpy(list_item->data, data, data_sz);
    list_item->data_sz = data_sz;

    if (curr) {
        // If we haven't moved yet, this is sort of a prepend
        if (_iterator->is_pristine) {
            if (_iterator->forward) {
                list_item->prev = NULL;
                list_item->next = curr;
                curr->prev = list_item;
                _list->first = list_item;
                _iterator->curr = list_item;
            } else {
                list_item->next = NULL;
                list_item->prev = curr;
                curr->next = list_item;
                _list->last = list_item;
                _iterator->curr = list_item;
            }
        } else {
            if (_iterator->forward) {
                list_item->prev = curr;
                list_item->next = curr->next;
                if (curr->next) {
                    curr->next->prev = list_item;
                }
                curr->next = list_item;

                // is new item now the last one?
                if (_list->last == curr) {
                    _list->last = list_item;
                }
            } else {
                list_item->next = curr;
                list_item->prev = curr->prev;
                if (curr->prev) {
                    curr->prev->next = list_item;
                }
                curr->prev = list_item;

                // is new item now the first one?
                if (_list->first == curr) {
                    _list->first = list_item;
                }
            }
        }
    } else {
        // add to an empty list?
        _iterator->curr = list_item;
        _list->first = list_item;
        list_item->prev = NULL;
        _list->last = list_item;
        list_item->next = NULL;
    }

    _list->count++;

    return 0;
}

int sypha_list_iterator_insert_before(SYPHA_LIST_ITERATOR iterator, void * data, size_t data_sz) {
    struct _sypha_list_iterator * _iterator = (struct _sypha_list_iterator *) iterator;
    struct _sypha_list_item * curr = _iterator->curr;
    struct _sypha_list * _list = (struct _sypha_list *) _iterator->list;

    // Can't add anything from initial state.  Regardless of moving forward or backward, the iterator
    // is intially positioned "before" a first item so adding BEFORE THAT doesn't make sense.
    if (_iterator->is_pristine) {
        return -1;
    }

    struct _sypha_list_item * list_item;
    if (!(list_item = (struct _sypha_list_item *) malloc(sizeof(struct _sypha_list_item)))) {
        return -1;
    }

    if (!(list_item->data = (void *) malloc(sizeof(data_sz)))) {
        free(list_item);
        return -1;
    }
    memcpy(list_item->data, data, data_sz);
    list_item->data_sz = data_sz;

    if (curr) {
        if (_iterator->forward) {
            list_item->next = curr;
            list_item->prev = curr->prev;
            if (curr->prev) {
                curr->prev->next = list_item;
            }
            curr->prev = list_item;

            // is new item now the first one?
            if (_list->first == curr) {
                _list->first = list_item;
            }
        } else {
            list_item->prev = curr;
            list_item->next = curr->next;
            if (curr->next) {
                curr->next->prev = list_item;
            }
            curr->next = list_item;

            // is new item now the last one?
            if (_list->last == curr) {
                _list->last = list_item;
            }
        }
    } else {
        // Not sure this code can ever be reached but leave it for now
        // add to an empty list?
        _iterator->curr = list_item;
        _list->first = list_item;
        list_item->prev = NULL;
        _list->last = list_item;
        list_item->next = NULL;
    }

    _list->count++;

    return 0;
}

int sypha_list_iterator_delete_current(SYPHA_LIST_ITERATOR iterator) {
    struct _sypha_list_iterator * _iterator = (struct _sypha_list_iterator *) iterator;
    struct _sypha_list_item * curr = _iterator->curr;
    struct _sypha_list * _list = (struct _sypha_list *) _iterator->list;
    struct _sypha_list_item * curr_prev, * curr_next;

    // Can't remove anything from initial state, or an empty list
    if (_iterator->is_pristine || !curr) {
        return -1;
    }

    curr_prev = curr->prev;
    curr_next = curr->next;

    if (curr_prev) {
        curr_prev->next = curr_next;
    }

    if (curr_next) {
        curr_next->prev = curr_prev;
    }

    // Was the curr item either the first or last one?
    if (_list->first == curr) {
        _list->first = curr_next;
        _iterator->curr = curr_next;
        _iterator->is_pristine = 1;
    }
    if (_list->last == curr) {
        _list->last = curr_prev;
        _iterator->curr = curr_prev;
    }

    // free the item
    free(curr->data);
    free(curr);

    _list->count--;

    return 0;
}

#if defined(__cplusplus)
}
#endif // __cplusplus
