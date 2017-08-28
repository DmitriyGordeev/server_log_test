/*
 * UserAction store all player action info
 * with serialization (json) methods
 */

#ifndef GAMELOGPARSER_USER_ACTION_H
#define GAMELOGPARSER_USER_ACTION_H

#include <map>
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"

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

    bool parse(const std::string json_str) {
        rapidjson::Document doc;

        char* c_json = new char[json_str.length()];
        strcpy(c_json, json_str.c_str());
        
        // json validation:
        if(doc.ParseInsitu(c_json).HasParseError()) {
            delete c_json;
            return false;
        }


        if(doc.HasMember("ts_fact")) {
            if(doc["ts_fact"].IsInt()) {
                ts_fact = static_cast<uint32_t>(doc["ts_fact"].GetInt());
            } else
                return false;
        }
        else
            return false;

        if(doc.HasMember("fact_name")) {
            if(doc["fact_name"].IsString()) {
                fact_name = doc["fact_name"].GetString();
            } else
                return false;
        }
        else
            return false;

        if(doc.HasMember("actor_id")) {
            if(doc["actor_id"].IsInt()) {
                actor_id = static_cast<uint32_t>(doc["actor_id"].GetInt());
            } else
                return false;
        }
        else
            return false;

        // props:
        if(doc.HasMember("props")) {
            if(doc["props"].IsObject()) {
                auto props_node = doc["props"].GetObject();
                for(auto itr = props_node.MemberBegin(); itr != props_node.MemberEnd(); ++itr) {
                    if(itr->value.IsInt())
                        props[itr->name.GetString()] = itr->value.GetInt();
                }
            }
            else
                return false;
        }
        else
            return false;

        return true;
    }

    
};

#endif //GAMELOGPARSER_USER_ACTION_H
