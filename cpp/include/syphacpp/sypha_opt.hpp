/* sypha_opt.hpp
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

#ifndef _SYPHA_OPT_HPP_
#define _SYPHA_OPT_HPP_

#include <string>
#include <set>
#include <list>
#include "syphac/sypha_opt.h"

namespace sypha {

    class Opt {
        public:
            class Param {
                private:
                    std::string m_shortName;
                    std::string m_longName;
                    bool m_flag;
                    bool m_required;

                public:
                    Param(const std::string & shortName, const std::string & longName, bool flag, bool required) : 
                        m_shortName(shortName), m_longName(longName), m_flag(flag), m_required(required)
                    {

                    }

                    const std::string & getShortName() const { return m_shortName; }
                    const std::string & getLongName() const { return m_longName; }
                    bool getFlag() const { return m_flag; }
                    bool getRequired() const { return m_required; }

                    bool operator<(const Param & p) const {
                        return m_shortName < p.m_shortName;
                    }
            };

            typedef std::set<Param> ParamSet;

            typedef std::list<std::string> ExtrasList;

        private:
            SYPHA_OPT_CONFIG m_optConfig;
            SYPHA_OPT_PARSE_RESULT m_optParseResult;

        public:
            Opt(ParamSet paramSet, int argc, char ** argv);
            ~Opt();

            // Fetch results, returns false if no value(s) found
            bool get(const std::string & name, std::string & value) const;
            bool getExtras(ExtrasList & values) const;
    };

} // samespace sypha

#endif // _SYPHA_OPT_HPP_
