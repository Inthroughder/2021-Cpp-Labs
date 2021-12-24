#ifndef UNTITLED_TRITSET_H
#define UNTITLED_TRITSET_H

#include <unordered_map>
#include <iostream>

using namespace std;

const unsigned int UINTSIZE = sizeof(unsigned int) * 8;

enum class Trit : unsigned int { False = 2, True = 1, Unknown = 0 };

ostream& operator<< (ostream&, const Trit&);

class TritSet {

    friend class TritReference;
    class TritReference;

private:

    unsigned int* TritArray;
    unsigned int size;
    unsigned int lastDefined;
    unsigned int lenTritArray;
    unsigned int originSize;

    void TritSetMemoryReallocator(unsigned int newSize);

    Trit TritGetter(unsigned int pos) const;

    static void TritSetter(unsigned int& Trits, unsigned int pos, Trit b);

    void TritAssignment(Trit b, unsigned int pos);

    bool TritComparator(Trit b, unsigned int pos) const;

    //--------------------------------TRITREFERENCE-STARTED--------------------------------------

    class TritReference {

        friend class TritSet;

    public:

        TritReference(TritSet* TritSetPointer, unsigned int position);

        operator Trit();

        void operator = (Trit b);

        bool operator == (Trit b) const;

    private:

        TritSet* TSPointer;
        unsigned int pos;

    };

    //--------------------------------TRITREFERENCE-ENDED----------------------------------------

public:

    TritSet(unsigned int sizeOfSet);

    ~TritSet();

    TritSet(const TritSet& TS);

    TritSet& operator=(const TritSet& TS);

    unsigned int capacity();

    TritReference operator [] (unsigned int position);

    Trit operator [] (unsigned int position) const;

    int shrink();


    TritSet operator & (const TritSet& B);

    void operator &=(const TritSet& B);

    TritSet operator | (const TritSet& B);

    void operator |=(const TritSet& B);

    TritSet operator ! ();

    void operator !=(const TritSet& B);


    unsigned int cardinality(Trit b);

    std::unordered_map<Trit, int> cardinality();

    void trim(unsigned int lastIndex);

    unsigned int length();

};

#endif //UNTITLED_TRITSET_H
