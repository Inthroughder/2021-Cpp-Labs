//TODO:
//fix << operator crashing
//Q1 - unordered map?
//Q2 - Catch2 what how?

//what is static and const? check every meaning
//next lab is necessary

#include <cassert>
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

	//unsigned int* TritArray;
	//unsigned int size;
	//unsigned int lastDefined;
	//unsigned int lenTritArray;
	//unsigned int originSize

	void TritSetMemoryReallocator(unsigned int newSize) {// DONE
		unsigned int newLenTritArray = 2 * (newSize) / UINTSIZE + (1 && (((newSize) * 2) % UINTSIZE));
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

	Trit TritGetter(unsigned int pos) const {//DONE
		return (Trit)(TritArray[pos * 2 / UINTSIZE] << (pos * 2 % UINTSIZE) >> (UINTSIZE - 2));
	}

	static void TritSetter(unsigned int& Trits, unsigned int pos, Trit b) { // DONE

		unsigned int t = 3 << ((UINTSIZE - 2) - (2 * pos) % UINTSIZE); // created 000...0000110000...000, i.e. "11" trit at pos
		t = UINT_MAX ^ t; // created 11111...1111001111111...11111, i.e. a "00" trit in a field of 1's
		t = t & Trits; // created a copy of TritSet chunk (Trits), but on the pos there are zeroes
		Trits = t | ((unsigned int)b << ((UINTSIZE - 2) - (2 * pos) % UINTSIZE)); // now we add b in that place of two zeroes, and it's done

	}

	void TritAssignment(Trit b, unsigned int pos) { //DONE

		if (pos >= size) { //else create a new TritSet with more memory, and delete the old one -- DONE

			if (b != Trit::Unknown) {

				TritSetMemoryReallocator(pos + 1);//requires manual lastDefined update
				this->lastDefined = -1;

			}

		}

		TritSetter(TritArray[2 * pos / UINTSIZE], pos, b);

		//lastDefined
		if ((lastDefined) == -1 || ((int)pos >= lastDefined)) {
			lastDefined = -1;
			unsigned int i = size;
			do {
				i--;
				if (TritGetter(i) != Trit::Unknown) { lastDefined = (int)i; break; }
			} while (i > 0);
		}

	}

	unsigned int TritComparator(Trit b, unsigned int pos) {//not tested -- do I need it?

		if ((unsigned int)TritGetter(pos) == (unsigned int)b) return 1; else return 0;

	}

	//--------------------------------TRITREFERENCE-STARTED--------------------------------------

	class TritReference {

	public:

		TritReference(TritSet* TritSetPointer, unsigned int position) { //DONE
			TSPointer = TritSetPointer;
			pos = position;
		}

		operator Trit() { //DONE
			return this->TSPointer->TritGetter(pos);
		}

		void operator = (Trit b) { //DONE

			this->TSPointer->TritAssignment(b, this->pos);

		}

		unsigned int operator == (Trit b) { //DONE

			return this->TSPointer->TritComparator(b, pos);

		}

	private:

		TritSet* TSPointer;
		unsigned int pos;

	};

	//--------------------------------TRITREFERENCE-ENDED----------------------------------------

public:

	TritSet(unsigned int sizeOfSet) { //constructor -- DONE
		assert(sizeOfSet != 0);
		size = sizeOfSet;
		originSize = sizeOfSet;
		lastDefined = -1;
		lenTritArray = 2 * sizeOfSet / UINTSIZE + (1 && ((sizeOfSet * 2) % UINTSIZE));
		TritArray = new unsigned int[lenTritArray];
		for (unsigned int i = 0; i < lenTritArray; i++) {
			TritArray[i] = 0;
		}
	}

	~TritSet() { //DONE
		delete[] TritArray;
	}

	//TritSet(TritSet&& TS) { //move constructor -- for logical operators -- not sure if I even need it
	//
	//}

	TritSet(const TritSet& TS) { //copy constructor -- DONE
		size = TS.size;
		originSize = TS.originSize;
		lastDefined = TS.lastDefined;
		lenTritArray = TS.lenTritArray;
		TritArray = new unsigned int[lenTritArray];
		for (unsigned int i = 0; i < lenTritArray; i++) {
			TritArray[i] = TS.TritArray[i];
		}
	}

	TritSet& operator=(const TritSet& TS) {// DONE
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

	unsigned int capacity() { //DONE
		return this->size;
	}

	TritReference operator [] (unsigned int position) { //DONE
		return TritReference(this, position);
	}

	Trit operator [] (unsigned int position) const { //DONE
		return TritGetter(position);
	}

	int shrink() { //DONE
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

	TritSet operator & (const TritSet& B) { // DONE, but randomly crashes through cout <<, don't know why
		unsigned int greater, lesser;
		if (size > B.size) {
			greater = size;
			lesser = B.size;
		}
		else {
			greater = B.size;
			lesser = size;
		}//size of a is greater or equal to b's

		TritSet newSet(greater);
		for (unsigned int i = 0; i < lesser; i++) {
			Trit a = (*this)[i];
			Trit b = B[i];
			switch (a) {
			case Trit::False:

				TritSetter(newSet.TritArray[i * 2 / UINTSIZE], i, Trit::False);
				break;

			case Trit::True:

				TritSetter(newSet.TritArray[i * 2 / UINTSIZE], i, b);
				break;

			case Trit::Unknown:

				switch (b) {
				case Trit::False:
					TritSetter(newSet.TritArray[i * 2 / UINTSIZE], i, Trit::False);
					break;
				default:
					TritSetter(newSet.TritArray[i * 2 / UINTSIZE], i, Trit::Unknown);
					break;
				}
				break;

			}
		}

		if (size > B.size) {

			for (unsigned int i = lesser; i < greater; i++) {
				switch ((*this)[i]) {
				case Trit::False:

					TritSetter(newSet.TritArray[i * 2 / UINTSIZE], i, Trit::False);
					break;

				default:

					TritSetter(newSet.TritArray[i * 2 / UINTSIZE], i, Trit::Unknown);
					break;

				}
			}

		}
		else {

			for (unsigned int i = lesser; i < greater; i++) {
				switch (B[i]) {
				case Trit::False:

					TritSetter(newSet.TritArray[i * 2 / UINTSIZE], i, Trit::False);
					break;

				default:

					TritSetter(newSet.TritArray[i * 2 / UINTSIZE], i, Trit::Unknown);
					break;

				}
			}

		}

		newSet.lastDefined = -1;
		unsigned int i = newSet.size;
		do {
			i--;
			if (newSet.TritGetter(i) != Trit::Unknown) { newSet.lastDefined = (int)i; break; }
		} while (i > 0);

		return newSet;
	}

	TritSet operator | (const TritSet& B) {// DONE, but also randomly crashes through cout <<
		unsigned int greater, lesser;
		if (size > B.size) {
			greater = size;
			lesser = B.size;
		}
		else {
			greater = B.size;
			lesser = size;
		}//size of a is greater or equal to b's

		TritSet newSet(greater);
		for (unsigned int i = 0; i < lesser; i++) {
			Trit a = (*this)[i];
			Trit b = B[i];
			switch (a) {
			case Trit::False:

				TritSetter(newSet.TritArray[i * 2 / UINTSIZE], i, b);
				break;

			case Trit::True:

				TritSetter(newSet.TritArray[i * 2 / UINTSIZE], i, Trit::True);
				break;

			case Trit::Unknown:

				switch (b) {
				case Trit::True:
					TritSetter(newSet.TritArray[i * 2 / UINTSIZE], i, Trit::True);
					break;
				default:
					TritSetter(newSet.TritArray[i * 2 / UINTSIZE], i, Trit::Unknown);
					break;
				}
				break;

			}
		}

		if (size > B.size) {

			for (unsigned int i = lesser; i < greater; i++) {
				TritSetter(newSet.TritArray[i * 2 / UINTSIZE], i, (*this)[i]);
			}

		}
		else {

			for (unsigned int i = lesser; i < greater; i++) {
				TritSetter(newSet.TritArray[i * 2 / UINTSIZE], i, B[i]);
			}

		}

		newSet.lastDefined = -1;
		unsigned int i = newSet.size;
		do {
			i--;
			if (newSet.TritGetter(i) != Trit::Unknown) { newSet.lastDefined = (int)i; break; }
		} while (i > 0);

		return newSet;
	}

	TritSet operator ! () { //DONE
		TritSet newSet(size);
		for (unsigned int i = 0; i < size; i++) {
			Trit temp = (*this)[i];
			switch (temp) {
			case Trit::True:
				TritSetter(newSet.TritArray[i * 2 / UINTSIZE], i, Trit::False);
				break;
			case Trit::False:
				TritSetter(newSet.TritArray[i * 2 / UINTSIZE], i, Trit::True);
				break;
			}
		}
		return newSet;
	}

	unsigned int cardinality(Trit b) {// DONE
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

	std::unordered_map< Trit, int, std::hash<int> > cardinality() {//Q1 - how to use unordered map and what's up with hashes?

	}

	void trim(unsigned int lastIndex) { // DONE
		for (unsigned int i = lastIndex; i < size; i++) {
			(*this)[i] = Trit::Unknown;
		}
	}

	unsigned int length() {
		return lastDefined + 1;
	}

	unsigned int* TritArray;
	unsigned int size;
	int lastDefined;
	unsigned int lenTritArray;
	unsigned int originSize;

};

ostream& operator<< (ostream& out, const Trit& trit) { // DONE -- CRASHES PROGRAM RANDOMLY, PROBABLY BECAUSE OF MEMORE LEAKS/DESTRUCTOR PROBLEMS
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

int main() {

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

	setA[90] = Trit::Unknown;

	TritSet newSet = setA | setB;

	cout << 0 << " = " << newSet[0] << endl;
	cout << 1 << " = " << newSet[1] << endl;
	cout << 2 << " = " << newSet[2] << endl;
	cout << 3 << " = " << newSet[3] << endl;
	cout << 4 << " = " << newSet[4] << endl;
	cout << 5 << " = " << newSet[5] << endl;
	cout << 6 << " = " << newSet[6] << endl;
	cout << 7 << " = " << newSet[7] << endl;
	cout << 8 << " = " << newSet[8] << endl;
	cout << 9 << " = " << newSet[9] << endl;

	cout << setA.cardinality(Trit::Unknown) << endl;

	setA.trim(9);

	cout << setA[9] << endl;

	cout << setA.length() << endl;

	return 0;
}
