// Written by Malin Bohman, 2023. 
#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <unordered_map>
#include <math.h>

struct Value{
    int i;
    float f;
    std::string str;
    std::string type;

    int get(int a, std::string key){
        try{
            if(type != "int"){throw(a);}
            else{return i;}
        }
        catch(int i){
            std::cout << "\n Error in SproutParam! Parameter with key '" << key << "' is not of type int \n";
            exit(0);
        }
    }

    float get(float a, std::string key){
        try{
            if(type != "float"){throw(a);}
            else{return f;}
        }
        catch(float i){
            std::cout << "\n Error in SproutParam! Parameter with key '" << key << "' is not of type float \n";
            exit(0);
            return i;
        }
    }

    std::string get(std::string a, std::string key){
        try{
            if(type != "string"){throw(a);}
            else{return str;}
        }
        catch(std::string i){
            std::cout << "\n Error in SproutParam! Parameter with key '" << key << "' is not of type std::string \n";
            exit(0);
            return i;
        }
    }
};

class SproutParam{
    private:
    std::unordered_map<std::string, Value> map;

    public:
    SproutParam(){};

    int test();

    int read(std::string infile){
        map.clear();

        std::fstream ob;
        ob.open(infile, std::ios::in);

        std::string line;
        std::string key;
        std::string value;

        while(std::getline(ob, line)){
            if(!line.empty() && line.at(0) != '#'){
                std::istringstream iss(line);

                iss >> key >> value;

                float f = atof(value.c_str());
                Value r;

                if(f == 0 && value.length() > 1){
                    r.str = value; r.type = "string";
                }
                else if(f != 0){
                    if(abs(fmod(f,1)) > 0){r.f = f; r.type = "float";}
                    else{r.i = (int) f; r.type = "int";}
                }
                else{
                    r.i = 0; r.type = "int";
                }
                map[key] = r;
            }
        }
        
        return 1;
    }

    template<typename T>
    T get(std::string key){
        static T dummy;
        try{
            auto search = map.find(key);
            if (search != map.end()){
                Value r = search->second;
                return r.get(dummy,key);
            }   
            else{
                throw(key);
            }
        }
        catch(std::string myKey){
            std::cout << "\n Error in SproutParam! Key: '" << key << "' not found\n";
            exit(0);
        }
    }

};


