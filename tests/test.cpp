
#include "catch.hpp"
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
#include <queue>
//#include "/workspaces/cs225/TheWikiGame/src/TheWikiGame.h"
#include "/workspaces/CS225 Labs/TheWikiGame/src/TheWikiGame.h"


TEST_CASE("Constructor test") {
    //TheWikiGame test = TheWikiGame("/workspaces/cs225/TheWikiGame/ArticleTJason.txt", "/workspaces/cs225/TheWikiGame/ArticleLinksJason.txt");
    TheWikiGame test = TheWikiGame("/workspaces/CS225 Labs/TheWikiGame/ArticleTKevin.txt", "/workspaces/CS225 Labs/TheWikiGame/ArticleLinksKevin.txt");
    REQUIRE(test.getIDByLink("https://en.wikipedia.org/wiki/Abraham_Lincoln") == 1);
    REQUIRE(test.getLinkByID(1) == "https://en.wikipedia.org/wiki/Abraham_Lincoln");
    REQUIRE(test.getIDByLink("https://en.wikipedia.org/wiki/Bushism") == 111);
    REQUIRE(test.getLinkByID(111) == "https://en.wikipedia.org/wiki/Bushism");
    REQUIRE(test.getLinkByID(17321) == "ID not valid");
    REQUIRE(test.getIDByLink("dasasda") == -1);
    REQUIRE(test.bfs("https://en.wikipedia.org/wiki/Nanny_and_Nanko", "https://en.wikipedia.org/wiki/Nanny_and_Nanko").size() == 0);
    REQUIRE(test.bfs("https://en.wikipedia.org/wiki/Nanny_and_Nanko", "https://en.wikipedia.org/wiki/Lee_Harvey_Oswald").size() - 1 == 3);
    REQUIRE(test.bfs("https://en.wikipedia.org/wiki/Nanny_and_Nanko", "https://www.youtube.com/watch?v=dQw4w9WgXcQ").size() == 0);
    vector<pair<int, double>> temp = test.pageRank(0.85, 100, 10);
    REQUIRE(temp.size() == 1667);
    REQUIRE(test.getLinkByID(temp[0].first) == "https://en.wikipedia.org/wiki/President_of_the_United_States");
    REQUIRE(test.getLinkByID(temp[1].first) == "https://en.wikipedia.org/wiki/Abraham_Lincoln");
}