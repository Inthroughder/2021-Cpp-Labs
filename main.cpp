#define CATCH_CONFIG_MAIN

#include "TritSet.h"

#include "catch.hpp"

#include <unordered_map>

using namespace std;

TEST_CASE("Size and memory") {
    TritSet set(1000);
    REQUIRE(set.capacity() == 1000);

    set[1000000000] = Trit::Unknown;
    REQUIRE(set.capacity() == 1000);

    // false, but no exception or memory allocation
    if (set[2000000000] == Trit::True) {}
    REQUIRE(set.capacity() == 1000);

    //выделение памяти
    set[1000000000] = Trit::True;
    REQUIRE(set.capacity() > 1000);

    //no memory operations
    set[1000000000] = Trit::Unknown;
    REQUIRE(set.capacity() == 1000000001);
    set[1000000] = Trit::False;
    REQUIRE(set.capacity() == 1000000001);

    //освобождение памяти до начального значения или
    //до значения необходимого для хранения последнего установленного трита
    //в данном случае для трита 1000’000
    set.shrink();
    REQUIRE(set.capacity() < 1000000001);

}

TEST_CASE("Logic") {
    TritSet setA(10);
    TritSet setB(100);

    setA[1] = Trit::Unknown;
    setB[1] = Trit::Unknown;

    setA[2] = Trit::Unknown;
    setB[2] = Trit::True;

    setA[3] = Trit::Unknown;
    setB[3] = Trit::False;

    setA[4] = Trit::True;
    setB[4] = Trit::Unknown;

    setA[5] = Trit::True;
    setB[5] = Trit::True;

    setA[6] = Trit::True;
    setB[6] = Trit::False;

    setA[7] = Trit::False;
    setB[7] = Trit::Unknown;

    setA[8] = Trit::False;
    setB[8] = Trit::True;

    setA[9] = Trit::False;
    setB[9] = Trit::False;

    TritSet setC = setA | setB;

    REQUIRE(setC[1] == Trit::Unknown);
    REQUIRE(setC[2] == Trit::True);
    REQUIRE(setC[3] == Trit::Unknown);
    REQUIRE(setC[4] == Trit::True);
    REQUIRE(setC[5] == Trit::True);
    REQUIRE(setC[6] == Trit::True);
    REQUIRE(setC[7] == Trit::Unknown);
    REQUIRE(setC[8] == Trit::True);
    REQUIRE(setC[9] == Trit::False);

    TritSet setD(5);
    setD = setA & setB;

    REQUIRE(setD[1] == Trit::Unknown);
    REQUIRE(setD[2] == Trit::Unknown);
    REQUIRE(setD[3] == Trit::False);
    REQUIRE(setD[4] == Trit::Unknown);
    REQUIRE(setD[5] == Trit::True);
    REQUIRE(setD[6] == Trit::False);
    REQUIRE(setD[7] == Trit::False);
    REQUIRE(setD[8] == Trit::False);
    REQUIRE(setD[9] == Trit::False);

    setD != setD;

    REQUIRE(setD[4] == Trit::Unknown);
    REQUIRE(setD[5] == Trit::False);
    REQUIRE(setD[6] == Trit::True);
}

TEST_CASE("Additional methods") {
    TritSet set(100);

    set[1] = Trit::False;
    set[2] = Trit::True;
    set[3] = Trit::True;

    REQUIRE(set.cardinality(Trit::False) == 1);
    REQUIRE(set.cardinality(Trit::True) == 2);
    REQUIRE(set.cardinality(Trit::Unknown) == 1);

    std::unordered_map<Trit, int> res = set.cardinality();

    REQUIRE(res[Trit::False] == 1);
    REQUIRE(res[Trit::True] == 2);
    REQUIRE(res[Trit::Unknown] == 97);

    set.trim(2);

    REQUIRE(set[1] == Trit::False);
    REQUIRE(set[2] == Trit::Unknown);
    REQUIRE(set[3] == Trit::Unknown);

    set[101] = Trit::True;
    REQUIRE(set.length() == 102);

}
