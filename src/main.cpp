#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "graphTheory.h"

std::string buses =
    "TransLink:CRYSTALVILLE,WILLOWBROOK,HARMONYVILLE,SILVERLAKE,OCEANVIEW@CRYSTALVILLE,WILLOWBROOK,HARMONYVILLE,SILVERLAKE,OCEANVIEW\n"
    "GoBus:CRYSTALVILLE,WILLOWBROOK,HARMONYVILLE,SILVERLAKE,OCEANVIEW@CRYSTALVILLE,WILLOWBROOK,HARMONYVILLE,SILVERLAKE,OCEANVIEW\n"
    "SwiftRide:CRYSTALVILLE,WILLOWBROOK,HARMONYVILLE,SILVERLAKE,OCEANVIEW@CRYSTALVILLE,WILLOWBROOK,HARMONYVILLE,SILVERLAKE,OCEANVIEW\n"
    "SpeedyBus:CRYSTALVILLE,WILLOWBROOK,HARMONYVILLE@CRYSTALVILLE,WILLOWBROOK,HARMONYVILLE\n"
    "CityHopper:CRYSTALVILLE,WILLOWBROOK,HARMONYVILLE,SILVERLAKE,OCEANVIEW@MOONSTONE\n"
    "RouteMasters:MOONSTONE@CRYSTALVILLE,WILLOWBROOK,HARMONYVILLE,SILVERLAKE,OCEANVIEW\n"
    "MetroTrans:CRYSTALVILLE,WILLOWBROOK,HARMONYVILLE,SILVERLAKE,OCEANVIEW,RIVERDALE,CEDARVILLE@CRYSTALVILLE,WILLOWBROOK,HARMONYVILLE,SILVERLAKE,OCEANVIEW,RIVERDALE,CEDARVILLE\n"
    "MegaBus:MAPLEWOOD,CRYSTALVILLE@MAPLEWOOD,CRYSTALVILLE\n";

std::vector<std::string> ParseCSV(
    const std::string &l)
{
    std::vector<std::string> token;
    std::stringstream sst(l);
    std::string a;
    while (getline(sst, a, ','))
        token.push_back(a);
    return token;
}

main()
{
    raven::graph::cGraph g;
    g.directed();
    std::map<std::pair<std::string, std::string>, std::vector<std::string>> mpBus;
    std::stringstream ss(buses);
    std::string line;
    while (getline(ss, line))
    {
        // std::cout << line << "\n";
        int p1 = line.find(":");
        int p2 = line.find("@", p1);
        std::string busName = line.substr(0, p1);
        std::string srcs = line.substr(p1 + 1, p2 - p1 - 1);
        std::string dsts;
        dsts = line.substr(p2 + 1);
        auto vs = ParseCSV(srcs);
        auto vd = ParseCSV(dsts);
        for (auto &s : vs)
            for (auto &d : vd)
            {
                // std::cout << s << " -> " << d  << "\n";
                g.add(s, d);
                auto it = mpBus.find(std::make_pair(s, d));
                if (it == mpBus.end())
                {
                    std::vector<std::string> vb{busName};
                    mpBus.insert(
                        std::make_pair(
                            std::make_pair(s, d),
                            vb));
                }
                else
                    it->second.push_back(busName);
            }
    }

    // for( auto& s : g.edgeList() )
    //     std::cout <<"l " << g.userName(s.first) <<" "<< g.userName(s.second) << " 1\n";

    raven::graph::cTourNodes T;
    T.calculate(g);

    std::set<std::string> setUsed;

    std::string src, dst;
    std::string tourText;
    bool first = true;
    for (int v : T.getTour())
    {
        if (first)
        {
            first = false;
            src = g.userName(v);
            continue;
        }
        dst = g.userName(v);
        auto vBus = mpBus.find(std::make_pair(src, dst))->second;
        std::string busUsed;
        for (auto &b : vBus)
        {
            auto itUsed = setUsed.find(b);
            if (itUsed == setUsed.end())
            {
                busUsed = b;
                setUsed.insert(b);
                break;
            }
        }
        if( busUsed.empty() )
            busUsed = vBus[0];

        tourText += g.userName(v) + " ->(" + busUsed + ")-> ";
        src = dst;
    }
    std::cout << tourText << "\n";

    return 0;
}
