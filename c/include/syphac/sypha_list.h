/* sypha_list.h
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

/* An attempt at making a generic list manipulation construct in C.  We're going for convenience here
 * so if you need to optimize for space just roll your own adhoc, slim singly linked list like you've
 * probably done a 1000 times before.  Since we're trading space for convenience, the list will be
 * doubly linked and will have a few extra goodies.
 * 
 * TODO:
 * 1) better indicate a malloc error
 * 2) add protection against creating a second iterator
 */ 

#ifndef _SYPHA_LIST_H_
#define _SYPHA_LIST_H_

#if defined __cplusplus
extern "C" {
#endif // __cplusplus

// Opague List object
typedef void * SYPHA_LIST;

// Opague list iterator object
typedef void * SYPHA_LIST_ITERATOR;

// Creates an empty list for data items
extern SYPHA_LIST sypha_list_create();

// Releases all allocated resources for the list
extern void sypha_list_destroy(SYPHA_LIST list);

// Insert items into the list making a copy of the item.  If a deep copy of some struct
// isn't needed, just use something like (void *) &ptr.
    // Add item to end of list making a copy of the data
extern void sypha_list_append_item(SYPHA_LIST list, void * data, size_t data_sz);
    // Add item to front of list making a copy of the data
extern void sypha_list_prepend_item(SYPHA_LIST list, void * data, size_t data_sz);

// Get iterators for the list.  Only 1 iterator can be in use at a time. Follows the
// convention of requiring a "next" call to get first item (e.g. positioned before first
// item).
    // Forward iterator from beginning of the list
extern SYPHA_LIST_ITERATOR sypha_list_get_iterator_front();
    // Backward iterator from end of the list
extern SYPHA_LIST_ITERATOR sypha_list_get_iterator_back();
    // Release all iterator resources
extern void sypha_list_destroy_iterator(SYPHA_LIST_ITERATOR iterator);

// Move to "next" item in list from perspective of forward / backward iterator.
    // Returns the data and fills in its size to the data_sz param, returns NULL if at end-of-iterator
extern void * sypha_list_iterator_next(SYPHA_LIST_ITERATOR iterator, size_t * data_sz);

// Move to "previous" item in list from perspective of forward / backward iterator
    // Returns the data and fills in its size to the data_sz param, returns NULL if at beginning-of-iterator
extern void * sypha_list_iterator_previous(SYPHA_LIST_ITERATOR iterator, size_t * data_sz);

// Adding / removing list items via the iterator
    // Insert new item after current item
extern void sypha_list_iterator_insert_after(SYPHA_LIST_ITERATOR iterator, void * data, size_t data_sz);
    // Insert new item before current item
extern void sypha_list_iterator_insert_before(SYPHA_LIST_ITERATOR iterator, void * data, size_t data_sz);
    // Delete the current item
extern void sypha_list_iterator_delete_current(SYPHA_LIST_ITERATOR iterator);

#if defined __cplusplus
}
#endif // __cplusplus

#endif // _SYPHA_LIST_H_
