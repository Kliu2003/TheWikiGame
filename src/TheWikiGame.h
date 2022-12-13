#pragma once
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
using namespace std;

class TheWikiGame{
    public:
        TheWikiGame(std::string titleFile, std::string linksFile); 
        std::vector<std::string> bfs(std::string startLocation, std::string endLocation);
        std::vector<std::string> djikstra(std::string startLocation, std::string endLocation);
        vector<pair<int, double>> pageRank(double dampingFactor, int depth, int topN);
        void resetPageRank();
        std::string getLinkByID(int id);
        int getIDByLink(std::string url);
    private:
        std::unordered_map<std::string, int>linkToId;
        std::unordered_map<int, std::string>idToLink;
        std::unordered_map<int, std::vector<int>>directedAdjacencyList;
        static bool cmp(pair<int, double>& a, pair<int, double>& b);
        std::unordered_map<int, double>idToPageRank;
};