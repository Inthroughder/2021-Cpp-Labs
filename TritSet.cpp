#include "TritSet.h"

#include <unordered_map>
#include <iostream>

using namespace std;

void TritSet::TritSetMemoryReallocator(unsigned int newSize) {// DONE
    unsigned int newLenTritArray = 2 * (newSize) / UINTSIZE + (1 && ((newSize * 2) % UINTSIZE));
    unsigned int* newArray = new unsigned int[newLenTritArray];// expands and copies all elements/shrinks and copies elemets that fits
    for (unsigned int i = 0; i < ((lenTritArray < newLenTritArray) ? lenTritArray : newLenTritArray); i++) {
        newArray[i] = TritArray[i];
    }
    for (unsigned int i = lenTritArray; i < newLenTritArray; i++) {//if expanded, sets everything to Unknown (zeroes)
        newArray[i] = 0;
    }
    delete[] TritArray;
    TritArray = newArray;
    size = newSize;
    lenTritArray = newLenTritArray; //requires manual lastDefined updating
}

Trit TritSet::TritGetter(unsigned int pos) const {//DONE
    if (pos >= size) {return Trit::Unknown;} else {
        return (Trit) (TritArray[pos * 2 / UINTSIZE] << (pos * 2 % UINTSIZE) >> (UINTSIZE - 2));
    }
}

void TritSet::TritSetter(unsigned int& Trits, unsigned int pos, Trit b) { // DONE

    unsigned int t = 3 << ((UINTSIZE - 2) - (2 * pos) % UINTSIZE); // created 000...0000110000...000, i.e. "11" trit at pos
    t = UINT_MAX ^ t; // created 11111...1111001111111...11111, i.e. a "00" trit in a field of 1's
    t = t & Trits; // created a copy of TritSet chunk (Trits), but on the pos there are zeroes
    Trits = t | ((unsigned int)b << ((UINTSIZE - 2) - (2 * pos) % UINTSIZE)); // now we add b in that place of two zeroes, and it's done

}

void TritSet::TritAssignment(Trit b, unsigned int pos) { //DONE

    if (pos >= size) { //else create a new TritSet with more memory, and delete the old one -- DONE

        if (b != Trit::Unknown) {

            TritSetMemoryReallocator(pos + 1);//requires manual lastDefined update
            lastDefined = -1;
            TritSetter(TritArray[2 * pos / UINTSIZE], pos, b);

        }

    }
    else {

        TritSetter(TritArray[2 * pos / UINTSIZE], pos, b);

    }

    //lastDefined
    if ((lastDefined == -1) || ((int)pos >= lastDefined)) {
        lastDefined = -1;
        unsigned int i = size;
        do {
            i--;
            if (TritGetter(i) != Trit::Unknown) {
                lastDefined = (int)i;
                break;
            }
        } while (i > 0);
    }

}

bool TritSet::TritComparator(Trit b, unsigned int pos) const {//not tested -- do I need it?

    if ((unsigned int)TritGetter(pos) == (unsigned int)b) return 1; else return 0;

}

//--------------------------------TRITREFERENCE-STARTED--------------------------------------

        TritSet::TritReference::TritReference(TritSet* TritSetPointer, unsigned int position) { //DONE
            TSPointer = TritSetPointer;
            pos = position;
        }

        TritSet::TritReference::operator Trit() { //DONE
            return this->TSPointer->TritGetter(pos);
        }

        void TritSet::TritReference::operator = (Trit b) { //DONE

            this->TSPointer->TritAssignment(b, this->pos);

        }

        bool TritSet::TritReference::operator == (Trit b) const { //DONE

            return this->TSPointer->TritComparator(b, pos);

        }

    //--------------------------------TRITREFERENCE-ENDED----------------------------------------

    TritSet::TritSet(unsigned int sizeOfSet) { //constructor -- DONE
        size = sizeOfSet;
        originSize = sizeOfSet;
        lastDefined = -1;
        lenTritArray = 2 * sizeOfSet / UINTSIZE + (1 && ((sizeOfSet * 2) % UINTSIZE));
        TritArray = new unsigned int[lenTritArray];
        for (unsigned int i = 0; i < lenTritArray; i++) {
            TritArray[i] = 0;
        }
    }

    TritSet::~TritSet() { //DONE
        delete[] TritArray;
    }

    TritSet::TritSet(const TritSet& TS) { //copy constructor -- DONE
        size = TS.size;
        originSize = TS.originSize;
        lastDefined = TS.lastDefined;
        lenTritArray = TS.lenTritArray;
        TritArray = new unsigned int[lenTritArray];
        for (unsigned int i = 0; i < lenTritArray; i++) {
            TritArray[i] = TS.TritArray[i];
        }
    }

    TritSet& TritSet::operator=(const TritSet& TS) {// DONE
        if (this != &TS) {
            delete[] TritArray;
            size = TS.size;
            originSize = TS.originSize;
            lastDefined = TS.lastDefined;
            lenTritArray = TS.lenTritArray;
            TritArray = new unsigned int[lenTritArray];
            for (unsigned int i = 0; i < lenTritArray; i++) {
                TritArray[i] = (TS.TritArray)[i];
            }
        }
        return *this;
    }

    unsigned int TritSet::capacity() { //DONE
        return this->size;
    }

    TritSet::TritReference TritSet::operator [] (unsigned int position) { //DONE
        return TritReference(this, position);
    }

    Trit TritSet::operator [] (unsigned int position) const { //DONE
        return TritGetter(position);
    }

    int TritSet::shrink() { //DONE
        //choose between originSize and lastDefined
        unsigned int newLenTritArraySize;
        if (lastDefined != -1) {
            newLenTritArraySize = 2 * (lastDefined + 1) / UINTSIZE + (1 && (((lastDefined + 1) * 2) % UINTSIZE));
            size = lastDefined + 1;

        }
        else if (size > originSize) {
            newLenTritArraySize = 2 * (originSize) / UINTSIZE + (1 && ((originSize * 2) % UINTSIZE));
            size = originSize;
        }
        else return -1;

        unsigned int* tempPointer = new unsigned int[newLenTritArraySize];
        for (unsigned int i = 0; i < newLenTritArraySize; i++) {
            tempPointer[i] = TritArray[i];
        }
        delete[] TritArray;
        TritArray = tempPointer;

        return 0;
    }

    TritSet TritSet::operator & (const TritSet& B) { // DONE
        TritSet newSet = (*this);
        newSet &= B;
        return newSet;
    }

    void TritSet::operator &=(const TritSet& B) {// DONE
        unsigned int greater, lesser;
        if (size > B.size) {
            greater = size;
            lesser = B.size;
        }
        else {
            greater = B.size;
            lesser = size;
        }//size of a is greater or equal to b's

        for (unsigned int i = 0; i < lesser; i++) {
            Trit a = (*this)[i];
            Trit b = B[i];
            switch (a) {
                case Trit::False:

                    TritSetter((this->TritArray)[i * 2 / UINTSIZE], i, Trit::False);
                    break;

                case Trit::True:

                    TritSetter((this->TritArray)[i * 2 / UINTSIZE], i, b);
                    break;

                case Trit::Unknown:

                    switch (b) {
                        case Trit::False:
                            TritSetter((this->TritArray)[i * 2 / UINTSIZE], i, Trit::False);
                            break;
                        default:
                            TritSetter((this->TritArray)[i * 2 / UINTSIZE], i, Trit::Unknown);
                            break;
                    }
                    break;

            }
        }

        if (size < B.size) {
            (this->TritSetMemoryReallocator)(B.size);
            for (unsigned int i = lesser; i < greater; i++) {
                switch ((*this)[i]) {
                    case Trit::False:

                        TritSetter((this->TritArray)[i * 2 / UINTSIZE], i, Trit::False);
                        break;

                    default:

                        TritSetter((this->TritArray)[i * 2 / UINTSIZE], i, Trit::Unknown);
                        break;

                }
            }

        }

        this->lastDefined = -1;
        unsigned int i = this->size;
        do {
            i--;
            if ((this->TritGetter(i)) != Trit::Unknown) { this->lastDefined = (int)i; break; }
        } while (i > 0);

    }

    TritSet TritSet::operator | (const TritSet& B) {// DONE
        TritSet newSet = (*this);
        newSet |= B;
        return newSet;
    }

    void TritSet::operator |=(const TritSet& B) {// DONE
        unsigned int greater, lesser;
        if (size > B.size) {
            greater = size;
            lesser = B.size;
        }
        else {
            greater = B.size;
            lesser = size;
        }//size of a is greater or equal to b's

        for (unsigned int i = 0; i < lesser; i++) {
            Trit a = (*this)[i];
            Trit b = B[i];
            switch (a) {
                case Trit::False:

                    TritSetter((this->TritArray)[i * 2 / UINTSIZE], i, b);
                    break;

                case Trit::True:

                    TritSetter((this->TritArray)[i * 2 / UINTSIZE], i, Trit::True);
                    break;

                case Trit::Unknown:

                    switch (b) {
                        case Trit::True:
                            TritSetter((this->TritArray)[i * 2 / UINTSIZE], i, Trit::True);
                            break;
                        default:
                            TritSetter((this->TritArray)[i * 2 / UINTSIZE], i, Trit::Unknown);
                            break;
                    }
                    break;

            }
        }

        if (size < B.size) {

            (this->TritSetMemoryReallocator)(B.size);
            for (unsigned int i = lesser; i < greater; i++) {
                TritSetter((this->TritArray)[i * 2 / UINTSIZE], i, B[i]);
            }

        }

        this->lastDefined = -1;
        unsigned int i = this->size;
        do {
            i--;
            if ((this->TritGetter)(i) != Trit::Unknown) { this->lastDefined = (int)i; break; }
        } while (i > 0);
    }

    TritSet TritSet::operator ! () { // DONE
        TritSet newSet = !(*this);
        return newSet;
    }

    void TritSet::operator != (const TritSet& B){ // DONE
        for (unsigned int i = 0; i < size; i++) {
            Trit temp = (*this)[i];
            switch (temp) {
                case Trit::True:
                    TritSetter(this->TritArray[i * 2 / UINTSIZE], i, Trit::False);
                    break;
                case Trit::False:
                    TritSetter(this->TritArray[i * 2 / UINTSIZE], i, Trit::True);
                    break;
            }
        }
    }

    unsigned int TritSet::cardinality(Trit b) {// DONE
        unsigned int res = 0;
        if (b != Trit::Unknown) {
            for (unsigned int i = 0; i < size; i++) {
                if ((*this)[i] == b) res++;
            }
        }
        else if (lastDefined != -1) {
            for (unsigned int i = 0; i < lastDefined; i++) {
                if ((*this)[i] == Trit::Unknown) res++;
            }
        }
        else {
            res = size;
        }
        return res;
    }

    std::unordered_map<Trit, int> TritSet::cardinality() { // DONE
        std::unordered_map<Trit, int> output = {
                {Trit::False, 0},
                {Trit::Unknown, 0},
                {Trit::True, 0}
        };

        for (unsigned int i = 0; i < size; i++) {
            output[(Trit)((*this)[i])]++;
        }
        return output;
    }

    void TritSet::trim(unsigned int lastIndex) { // DONE
        for (unsigned int i = lastIndex; i < size; i++) {
            (*this)[i] = Trit::Unknown;
        }
    }

    unsigned int TritSet::length() {
        return lastDefined + 1;
    }

ostream& operator<< (ostream& out, const Trit& trit) { // DONE
    switch (trit) {
        case Trit::True:
            cout << "True";
            break;
        case Trit::False:
            cout << "False";
            break;
        case Trit::Unknown:
            cout << "Unknown";
            break;
    }
    return out;
}
