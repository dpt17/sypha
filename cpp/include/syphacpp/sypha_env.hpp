/* sypha_env.hpp
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

#ifndef _SYPHA_ENV_HPP_
#define _SYPHA_ENV_HPP_

#include "syphac/sypha_env.h"
#include <string>

namespace sypha {

    // Uses c-strings under the hood

    class Env {
        public:
            Env() {
                sypha_env_load_dot_env();
            }
            
            // Returns false if key not found, otherwise true and sets value
            bool get(const std::string & key, std::string & value) const {
                const char * val = sypha_env_get(key.c_str());
                if (val) {
                    value.assign(val);
                    return true;
                } else {
                    return false;
                }
            }

            void set(const std::string & key, const std::string & value, bool overwrite) {
                sypha_env_set(key.c_str(), value.c_str(), (overwrite ? 1 : 0));
            }
    };

} // namespace sypha

#endif // _SYPHA_ENV_HPP_
