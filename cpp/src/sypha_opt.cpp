/* sypha_opt.cpp
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
#include "syphacpp/sypha_opt.hpp"

namespace sypha {

    Opt::Opt(ParamSet paramSet, int argc, char ** argv) : m_optConfig(NULL), m_optParseResult(NULL) {
        SYPHA_OPT_CONFIG optConfig = NULL, result = NULL;
        for (Param p : paramSet) {
            result = sypha_opt_config_add_param(optConfig, p.getShortName().c_str(), p.getLongName().c_str(), (p.getFlag() ? 1 : 0), 
                (p.getRequired() ? 1 : 0));
            if (result && !optConfig) {
                optConfig = result;
            } else if (!result) {
                if (optConfig) {
                    sypha_opt_config_free(optConfig);
                    throw new std::exception();
                }
            }
        }
        m_optConfig = optConfig;

        // Could consider trashing the config after this
        m_optParseResult = sypha_opt_parse_args(m_optConfig, argc, argv);

        if (!m_optParseResult) {
            sypha_opt_config_free(optConfig);
            throw new std::exception();
        }
    }

    Opt::~Opt() {
        if (m_optParseResult) {
            sypha_opt_parse_free(m_optParseResult);
            m_optParseResult = NULL;
        }

        if (m_optConfig) {
            sypha_opt_config_free(m_optConfig);
            m_optConfig = NULL;
        }
    }

    bool Opt::get(const std::string & name, std::string & value) const {
        const char * val = sypha_opt_parse_get(m_optParseResult, name.c_str());
        if (val) {
            value.assign(val);
            return true;
        } else {
            return false;
        }
    }

    bool Opt::getExtras(ExtrasList & values) const {
        const char ** valList = sypha_opt_parse_get_extras(m_optParseResult);
        bool ret = (*valList != NULL);
        for (; *valList; valList++) {
            values.push_back(*valList);
        }
        return ret;
    }

} // namespace sypha
