/* sypha_opt.h
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

#ifndef _SYPHA_OPT_H_
#define _SYPHA_OPT_H_

#if defined __cplusplus
extern "C" {
#endif // __cplusplus

// Opt config object
typedef void * SYPHA_OPT_CONFIG;

// TODO: add support for default values

// Adds a new param to config, pass NULL cfg for first invocation, returns NULL on errror
extern SYPHA_OPT_CONFIG sypha_opt_config_add_param(SYPHA_OPT_CONFIG cfg, const char * short_name, const char * long_name, int is_flag, int is_required);

// Release opt config
extern void sypha_opt_config_free(SYPHA_OPT_CONFIG cfg);

// Dumps a cfg to stdout
extern void sypha_opt_config_print(SYPHA_OPT_CONFIG cfg);

// TODO: maybe add ability to lookup values in a config

// Opt parse result object
typedef void * SYPHA_OPT_PARSE_RESULT;

// Parses all program args, returns NULL on error
extern SYPHA_OPT_PARSE_RESULT sypha_opt_parse_args(SYPHA_OPT_CONFIG cfg, int argc, char ** argv);

// Release a parse result
extern void sypha_opt_parse_free(SYPHA_OPT_PARSE_RESULT parse_result);

// Returns non-zero if param is present in the result. Better for flag checks.
extern int sypha_opt_parse_exist(SYPHA_OPT_PARSE_RESULT parse_result, const char * name);

// Lookup a specific param's value in result. Note: flags don't have a value.  Returns NULL if not found
extern const char * sypha_opt_parse_get_value(SYPHA_OPT_PARSE_RESULT parse_result, const char * name);

// Lookup a specific param in result.
// DEPRECATED: vague about what it does
extern const char * sypha_opt_parse_get(SYPHA_OPT_PARSE_RESULT parse_result, const char * name);

// Grab any args that are valid, given the original config, but weren't associated with a param.
// These are returned in order.  End of list denoted by NULL element.
extern const char ** sypha_opt_parse_get_extras(SYPHA_OPT_PARSE_RESULT parse_result);

// Dumps a parsed result to stdout
extern void sypha_opt_parse_print(SYPHA_OPT_PARSE_RESULT parse_result);

#if defined __cplusplus
}
#endif // __cplusplus

#endif // _SYPHA_OPT_H_
