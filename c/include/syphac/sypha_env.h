/* sypha_env.h
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

#ifndef _SYPHA_ENV_H_
#define _SYPHA_ENV_H_

#include <stdlib.h>

#if defined __cplusplus
extern "C" {
#endif // __cplusplus

// Supports an ASCII .env file and uses c-string {set, get} env API behind the scenes.
// Users have a choice: a) use only the load function and keep using the standard env API
// elsewhere in their code or b) use the wrapper functions as well for future proofing 

// Artificially restrict size of values in our .env support to reasonable limits
#define MAX_KEY_LEN     1024
#define MAX_VALUE_LEN   10240

// looks for a .env file in the current directory and writes its contents to
// the program's environment. 
extern void sypha_env_load_dot_env();

// Wrap standard {set, get} env API to allow for future flexibility
#define sypha_env_get(key)                         getenv(key)
#define sypha_env_set(key, value, overwrite)       setenv(key, value, overwrite)

#if defined __cplusplus
}
#endif // __cplusplus

#endif // _SYPHA_ENV_H_
