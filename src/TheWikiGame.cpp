#include "TheWikiGame.h"
#include <queue>

TheWikiGame::TheWikiGame(std::string titleFile, std::string linksFile) {
    ifstream wordsFile(titleFile);
    std::string word;
    int count = 1; 
    if (wordsFile.is_open()) {
        std::cout << "file open" << std::endl;
        while (getline(wordsFile, word)) {
            if (word.at(0) == ' ') {
                word.erase(0, 1); 
            }
            if (word.at(word.size() - 1) == ' ') {
                word.erase(word.size() - 1, 1); 
            }
            articleTitles.insert(word);   
            linkToId[word] = count;
            std::cout << word << " " << count << std::endl;
            idToLink[count] = word;
            count++;      
        }
    }
    std::cout << "al id: " << linkToId["Bibliography of Joe Biden"] << std::endl;
    ifstream wordsFile1(linksFile);
    std::string line;
    bool odd = true; 
    std::string curword = "";
    if (wordsFile1.is_open()) {
        std::cout << "wordsfile1 open" << std::endl;
        while (getline(wordsFile1, line)) { 
            if(odd) {
                curword = line; 
                //std::cout << "curword: " << curword << std::endl;
                if (curword.at(0) == ' ') {
                    curword.erase(0, 1); 
                }
                if (curword.at(curword.size() - 1) == ' ') {
                    curword.erase(curword.size() - 1, 1); 
                }
                odd = false; 
            } else { 
                std::vector<int> tempvect; 
                std::string process = "";
                for (char c : line) { 
                    if (c == ',') { 
                        if (linkToId.count(process) == 1) {
                           tempvect.push_back(linkToId[process]); 
                        }
                        process = ""; 
                    } else {
                        process.push_back(c); 
                    }
                }
                odd = true; 
                int curwordint = linkToId[curword];
                //std::cout << "curword int: " << curwordint << std::endl;
                directedAdjacencyList[curwordint] = tempvect;
            }
        }
    }
    std::cout << directedAdjacencyList.size() << std::endl;
}

std::vector<std::vector<string>> TheWikiGame::bfs(std::string startLocation, std::string endLocation) {
    std::queue<int>bfsQueue;
    int startId = linkToId[startLocation];
    bfsQueue.push(startId);
    int dist = 0;
    bool foundPath = false;
    while(dist < 6 && !foundPath){
        int degreeSize = bfsQueue.size();
        for(int j = 0; j < degreeSize; j++){
            int currentPage = bfsQueue.front();
            bfsQueue.pop();
            vector<int>linksToOtherPages = directedAdjacencyList[currentPage];
            for(int k = 0; k < linksToOtherPages.size(); k++){
                int adjacentId = linksToOtherPages[k];
                string s = idToLink[adjacentId];
                if(s == endLocation){
                    foundPath = true;
                }
            }
        }
        dist++;
    }
    
    std::cout<<dist<<std::endl;
    return std::vector<std::vector<string>>();
}

std::vector<std::vector<string>> TheWikiGame::djikstra(std::string startLocation, std::string endLocation) {
    return std::vector<std::vector<string>>();
}

int TheWikiGame::test(){
    return 1;
}