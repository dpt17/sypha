/* opt.h
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

#ifndef _OPT_H_
#define _OPT_H_

#if defined __cplusplus
extern "C" {
#endif // __cplusplus

// Opt config object
typedef void * OPT_CONFIG;

// TODO: add support for default values

// Adds a new param to config, pass NULL cfg for first invocation, returns NULL on errror
extern OPT_CONFIG opt_config_add_param(OPT_CONFIG cfg, const char * short_name, const char * long_name, int is_flag, int is_required);

// Release opt config
extern void opt_config_free(OPT_CONFIG cfg);

// Dumps a cfg to stdout
extern void opt_config_print(OPT_CONFIG cfg);

// TODO: maybe add ability to lookup values in a config

// Opt parse result object
typedef void * OPT_PARSE_RESULT;

// Parses all program args, returns NULL on error
extern OPT_PARSE_RESULT opt_parse_args(OPT_CONFIG cfg, int argc, char ** argv);

// Release a parse result
extern void opt_parse_free(OPT_PARSE_RESULT parse_result);

// Lookup a specific param in result
extern const char * opt_parse_get(OPT_PARSE_RESULT parse_result, const char * name);

// Grab any args that are valid, given the original config, but weren't associated with a param.
// These are returned in order.  End of list denoted by NULL element.
extern const char ** opt_parse_get_extras(OPT_PARSE_RESULT parse_result);

// Dumps a parsed result to stdout
extern void opt_parse_print(OPT_PARSE_RESULT parse_result);

#if defined __cplusplus
}
#endif // __cplusplus

#endif // _OPT_H_
