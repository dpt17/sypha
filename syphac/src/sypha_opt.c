/* sypha_opt.c
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
#include <stdio.h>
#include <string.h>
#include "syphac/sypha_opt.h"

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

struct _sypha_opt_config_item {
    char * short_name;
    char * long_name;
    int is_flag;
    int is_required;
    struct _sypha_opt_config_item * next;
};

struct _sypha_opt_result_item {
    char * short_name;
    char * long_name;
    char * value;
    struct _sypha_opt_result_item * next;
};

struct _sypha_opt_result {
    struct _sypha_opt_result_item * items;
    char ** extras;
};

// Returns matching config or NULL if nothing found
static struct _sypha_opt_config_item * sypha_opt_config_find(struct _sypha_opt_config_item * items, const char * token) {
    while (items) {
        if (strcmp(items->short_name, token) == 0 || strcmp(items->long_name, token) == 0) {
            return items;
        }
        items = items->next;
    }
    return NULL;
}

SYPHA_OPT_CONFIG sypha_opt_config_add_param(SYPHA_OPT_CONFIG cfg, const char * short_name, const char * long_name, int is_flag, int is_required) {
    struct _sypha_opt_config_item * item, * parent;
    
    // TODO: also check that second char is an alpha
    if (!short_name || strlen(short_name) != 2 || short_name[0] != '-') {
        return NULL;
    }
    
    // TODO: also check that the third char is an alpha
    if (!long_name || strlen(long_name) < 3 || long_name[0] != '-' || long_name[1] != '-') {
        return NULL;
    }

    if (!(item = (struct _sypha_opt_config_item *) malloc(sizeof(struct _sypha_opt_config_item)))) {
        return NULL;
    }
    item->short_name = NULL;
    item->long_name = NULL;
    item->short_name = strdup(short_name);
    item->long_name = strdup(long_name);
    item->is_flag = is_flag;
    item->is_required = is_required;
    item->next = NULL;
    if (!item->short_name || !item->long_name) {
        free(item->short_name);
        free(item->long_name);
        free(item);
        return NULL;
    }

    if ((parent = (struct _sypha_opt_config_item *) cfg)) {
        while (parent->next) {
            parent = parent->next;
        }
        parent->next = item;
        return cfg;
    } else {
        return (SYPHA_OPT_CONFIG) item;
    }
}

void sypha_opt_config_free(SYPHA_OPT_CONFIG cfg) {
    struct _sypha_opt_config_item * param, * next;
    param = (struct _sypha_opt_config_item *) cfg;
    while (param) {
        next = param->next;
        free(param->short_name);
        free(param->long_name);
        free(param);
        param = next;
    }
}

void sypha_opt_config_print(SYPHA_OPT_CONFIG cfg) {
    struct _sypha_opt_config_item * param = (struct _sypha_opt_config_item *) cfg;

    printf("SYPHA_OPT_CONFIG:\n{\n");
    while (param) {
        printf("\t{ %s, %s, %s, %s }\n", param->short_name, param->long_name, 
            ((param->is_flag) ? "flag" : "non-flag"), ((param->is_required) ? "required" : "optional"));
        param = param->next;
    }
    printf("}\n");
}

static int sypha_validate_required_in_result(struct _sypha_opt_result * result, struct _sypha_opt_config_item * config) {
    // Go through config, see if each required param is present
    while (config) {
        if (config->is_required) {
            if (!sypha_opt_parse_exist(result, config->short_name)) {
                return 0;
            }
        }
        config = config->next;
    }

    return 1;
}

SYPHA_OPT_PARSE_RESULT sypha_opt_parse_args(SYPHA_OPT_CONFIG cfg, int argc, char ** argv) {
    struct _sypha_opt_result * result;
    struct _sypha_opt_config_item * config_items = (struct _sypha_opt_config_item *) cfg;

    if (!(result = (struct _sypha_opt_result *) malloc(sizeof(struct _sypha_opt_result)))) {
        return NULL;
    }
    result->items = NULL;
    result->extras = NULL;

    size_t extras_size = argc * sizeof(char *);
    if (!(result->extras = (char **) malloc(extras_size))) {
        sypha_opt_parse_free(result);
        return NULL;
    }
    memset(result->extras, 0x0, extras_size);

    int extras_count = 0;
    int last_needs_value = 0;
    struct _sypha_opt_config_item * config_item = NULL;
    struct _sypha_opt_result_item * result_item = NULL;
    struct _sypha_opt_result_item * last_result_item = NULL;

    // Start at index 1 to skip the program name
    for (int i=1; i < argc; i++) {
        char * arg = argv[i];
        size_t argLen = strlen(arg);

        // -X ?
        if (argLen == 2 && arg[0] == '-') {
            // find it
            if ((config_item = sypha_opt_config_find(config_items, arg))) {
                if (!(result_item = (struct _sypha_opt_result_item *) malloc(sizeof(struct _sypha_opt_result_item)))) {
                    sypha_opt_parse_free(result);
                    return NULL;
                }
                result_item->short_name = NULL;
                result_item->long_name = NULL;
                result_item->value = NULL;
                result_item->next = NULL;
                result_item->short_name = strdup(config_item->short_name);
                result_item->long_name = strdup(config_item->long_name);
                if (!result_item->short_name || !result_item->long_name) {
                    sypha_opt_parse_free(result);
                    return NULL;
                }
                if (last_result_item == NULL) {
                    result->items = result_item;
                } else {
                    last_result_item->next = result_item;
                }
                last_result_item = result_item;
                last_needs_value = !config_item->is_flag;
                continue;
            } else {
                // unknown param
                sypha_opt_parse_free(result);
                return NULL;
            }
        }

        // --XYZ ?
        if (argLen > 2 && arg[0] == '-' && arg[1] == '-') {
           // find it
            if ((config_item = sypha_opt_config_find(config_items, arg))) {
                if (!(result_item = (struct _sypha_opt_result_item *) malloc(sizeof(struct _sypha_opt_result_item)))) {
                    sypha_opt_parse_free(result);
                    return NULL;
                }
                result_item->short_name = NULL;
                result_item->long_name = NULL;
                result_item->value = NULL;
                result_item->next = NULL;
                result_item->short_name = strdup(config_item->short_name);
                result_item->long_name = strdup(config_item->long_name);
                if (!result_item->short_name || !result_item->long_name) {
                    sypha_opt_parse_free(result);
                    return NULL;
                }
                if (last_result_item == NULL) {
                    result->items = result_item;
                } else {
                    last_result_item->next = result_item;
                }
                last_result_item = result_item;
                last_needs_value = !config_item->is_flag;
                continue;
            } else {
                // unknown param
                sypha_opt_parse_free(result);
                return NULL;
            }
        }

        if (last_needs_value) {
            if (!(result_item->value = strdup(arg))) {
                sypha_opt_parse_free(result);
                return NULL;
            }
            last_needs_value = 0;
        } else {
            // add it to rando token list
            if (!(result->extras[extras_count] = strdup(arg))) {
                sypha_opt_parse_free(result);
                return NULL;
            }
            extras_count++;
        }
    }

    if (!sypha_validate_required_in_result(result, config_items)) {
        sypha_opt_parse_free(result);
        return NULL;
    }

    return result;
}

void sypha_opt_parse_free(SYPHA_OPT_PARSE_RESULT parse_result) {
    char ** extras;
    struct _sypha_opt_result_item * item, * item_next;
    struct _sypha_opt_result * opt_result = (struct _sypha_opt_result *) parse_result;

    if (!opt_result) {
        return;
    }

    item = opt_result->items;
    while (item) {
        free(item->short_name);
        free(item->long_name);
        free(item->value);
        item_next = item->next;
        free(item);
        item = item_next;
    }
    
    extras = opt_result->extras;
    while (extras) {
        if (*extras) {
            free(*extras);
            extras++;
        } else {
            break;
        }
    }
    free(opt_result->extras);

    free(opt_result);
}

static struct _sypha_opt_result_item * sypha_opt_parse_result_find(struct _sypha_opt_result_item * items, const char * name) {
    while (items) {
        if ((strcmp(name, items->short_name) == 0) || (strcmp(name, items->long_name) == 0)) {
            return items;
        }
        items = items->next;
    }
    return NULL;
}

int sypha_opt_parse_exist(SYPHA_OPT_PARSE_RESULT parse_result, const char * name) {
    struct _sypha_opt_result * result;

    if (!(result = (struct _sypha_opt_result *) parse_result)) {
        return 0;
    }

    return (sypha_opt_parse_result_find(result->items, name) != NULL);
}

const char * sypha_opt_parse_get_value(SYPHA_OPT_PARSE_RESULT parse_result, const char * name) {
    struct _sypha_opt_result * result;
    struct _sypha_opt_result_item * item;

    if (!(result = (struct _sypha_opt_result *) parse_result)) {
        return NULL;
    }

    item = sypha_opt_parse_result_find(result->items, name);

    return ((item == NULL) ? NULL : item->value);
}

// Deprecated: switch to clearer sypha_opt_parse_exist and sypha_opt_parse_get_value
const char * sypha_opt_parse_get(SYPHA_OPT_PARSE_RESULT parse_result, const char * name) {
    struct _sypha_opt_result * result;
    struct _sypha_opt_result_item * items;
    const char * value;

    if (!(result = (struct _sypha_opt_result *) parse_result)) {
        return NULL;
    }

    value = NULL;
    items = result->items;
    while (items) {
        if ((strcmp(name, items->short_name) == 0) || (strcmp(name, items->long_name) == 0)) {
            value = items->value;
            break;
        }
        items = items->next;
    }

    return value;
}

const char ** sypha_opt_parse_get_extras(SYPHA_OPT_PARSE_RESULT parse_result) {
    struct _sypha_opt_result * result;
    
    if (!(result = (struct _sypha_opt_result *) parse_result)) {
        return NULL;
    }

    return (const char **) result->extras;
}

void sypha_opt_parse_print(SYPHA_OPT_PARSE_RESULT parse_result) {
    struct _sypha_opt_result * result;
    
    printf("SYPHA_OPT_PARSE_RESULT:\n{\n");
    if (!(result = (struct _sypha_opt_result *) parse_result)) {
        return;
    }

    struct _sypha_opt_result_item * items = result->items;
    while (items) {
        printf("\t{ \"%s\" | \"%s\" => ", items->short_name, items->long_name);
        if (items->value) {
            printf("\"%s\" }\n", items->value);
        } else {
            printf("_set_ }\n");
        }
        items = items->next;
    }
    
    char ** extras = result->extras;
    while (*extras) {
        printf("\t{ %s }\n", *extras);
        extras++;
    }
    printf("}\n");
}

#if defined(__cplusplus)
}
#endif // __cplusplus
