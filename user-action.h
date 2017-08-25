/*
 * UserAction store all player action info
 * with serialization (json) methods
 */

#ifndef GAMELOGPARSER_USER_ACTION_H
#define GAMELOGPARSER_USER_ACTION_H

#include <map>
#include "rapidjson/prettywriter.h"

struct UserAction {

    UserAction() {}

    UserAction(uint32_t ts_fact,
               const std::string& fact_name,
               uint32_t actor_id,
               const std::map<std::string, int>& props)
    {
        this->ts_fact = ts_fact;
        this->fact_name = fact_name;
        this->actor_id = actor_id;
        this->props = props;
    }

    uint32_t  ts_fact;
    std::string fact_name;
    uint32_t actor_id;
    std::map<std::string, int> props;

    // serialization
    void json(rapidjson::PrettyWriter<rapidjson::StringBuffer>& pw) {

        pw.StartObject();

        pw.Key("ts_fact");
        pw.Int(ts_fact);

        pw.Key("fact_name");
        pw.String(fact_name.c_str());

        pw.Key("actor_id");
        pw.Int(actor_id);

        pw.Key("props");
        pw.StartObject();
        for(auto p : props) {
            pw.Key(p.first.c_str());
            pw.Int(p.second);
        }
        pw.EndObject();

        pw.EndObject();

    }

    
};

#endif //GAMELOGPARSER_USER_ACTION_H
