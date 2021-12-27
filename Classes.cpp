
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <exception>

#include "Classes.h"


//-------------------------------EXCEPTIONS-STARTED-------------------------------------


WorkflowException::WorkflowException(){}

const char* WorkflowReadingException::what() const noexcept {
    return "Error while reading the workflow file";
}

const char* InconsistentArgumentException::what() const noexcept {
    return "Inconsistent argument";
}

const char* TextReadingException::what() const noexcept {
    return "Text reading error";
}

const char* TextWritingException::what() const noexcept {
    return "Text writing error";
}

WorkflowReadingException WREx{};
InconsistentArgumentException IAEx{};
TextReadingException TREx{};
TextWritingException TWEx{};

//-------------------------------EXCEPTIONS-ENDED--------------------------------------

//-------------------------------WORKERS-STARTED-------------------------------------


Worker::~Worker() {}


Readfile::Readfile(std::vector <std::string> filenameInput) {
    filename = filenameInput[0];
}

std::vector <std::string> Readfile::execute() {
    std::ifstream f(filename);
    std::vector <std::string> input;
    std::string line;
    if (f.is_open()) {
        while (std::getline(f, line)) {
            input.push_back(line);
            std::cout << line << std::endl;
        }
        f.close();
    } else {
        throw TREx;
    }
    return input;
}

Writefile::Writefile(std::vector <std::string> InputText) {
    Text = InputText;
    Text.pop_back();
    filename = InputText[InputText.size() - 1];
}

std::vector<std::string> Writefile::execute() {
    std::ofstream f(filename);
    if (f.is_open()) {
        for (unsigned int i = 0; i < Text.size(); i++) {
            f << Text[i] << std::endl;
        }
        f.close();
        return Text;
    } else {
        throw TWEx;
    }
}

Grep::Grep(std::vector <std::string> TextInput) {
    Text = TextInput;
    Text.pop_back();
    requiredWord = TextInput[TextInput.size() - 1];
}

std::vector <std::string> Grep::execute() {
    std::vector <std::string> TextOutput;
    for (unsigned int i = 0; i < Text.size(); i++) {
        std::cout << Text[i] << std::endl;
        unsigned int t = Text[i].find(requiredWord);
        std::cout << t << std::endl;
        if (t != /*std::string::npos*/ 4294967295) {
            TextOutput.push_back(Text[i]);
        }
    }
    return TextOutput;
}

Sort::Sort(std::vector <std::string> TextInput) {
    Text = TextInput;
}

std::vector <std::string> Sort::execute() {
    std::sort(Text.begin(), Text.end());
    return Text;
}


Replace::Replace(std::vector <std::string> TextInput) {
    Text = TextInput;
    Text.pop_back();
    Text.pop_back();
    oldWord = TextInput[TextInput.size() - 2];
    newWord = TextInput[TextInput.size() - 1];
}

std::vector <std::string> Replace::execute() {

    for (unsigned int i = 0; i < Text.size(); i++) {
        unsigned int pos = Text[i].find(oldWord);
        while (pos != /*std::string::npos*/ 4294967295) {
            Text[i].replace(pos, oldWord.length(), newWord);
            pos = Text[i].find(oldWord);
        }
    }

    return Text;
}


Dump::Dump(std::vector <std::string> TextInput) {
    Text = TextInput;
    Text.pop_back();
    filename = TextInput[TextInput.size() - 1];
}

std::vector <std::string> Dump::execute() {
    std::ofstream f(filename);

    if (f.is_open()) {
        for (unsigned int i = 0; i < Text.size(); i++) {
            f << Text[i] << std::endl;
        }
        f.close();
    } else {
        throw TWEx;
    }

    return Text;
}

//-------------------------------WORKERS-ENDED-------------------------------------

//-------------------------------EXECUTOR AND READER-STARTED-------------------------------------

WorkflowExecutor::WorkflowExecutor(std::unordered_map <int, std::string> WorkflowListInput, std::vector <int> WorkflowSequenceInput) {
    input = 0;
    WorkflowList = WorkflowListInput;
    WorkflowSequence = WorkflowSequenceInput;
}

Worker* WorkflowExecutor::FactoryMethod(std::string workerName, std::vector <std::string> InputText) {
    if (workerName == "readfile") {
        if (input == 1) {
            throw IAEx;
        }

        input = 1;
        return new Readfile(Text);

    }
    else if (workerName == "writefile") {
        if (input == 0) {
            throw IAEx;
        }

        input = 0;
        return new Writefile(Text);

    }
    else if (workerName == "grep") {
        if (input == 0) {
            throw IAEx;
        }

        return new Grep(Text);
    }
    else if (workerName == "sort") {
        if (input == 0) {
            throw IAEx;
        }

        return new Sort(Text);

    }
    else if (workerName == "replace") {
        if (input == 0) {
            throw IAEx;
        }

        return new Replace(Text);

    }
    else if (workerName == "dump") {
        if (input == 0) {
            throw IAEx;
        }

        return new Dump(Text);
    } else { throw WREx;}

}

void WorkflowExecutor::execute() {
    for (unsigned int i = 0; i < WorkflowSequence.size(); i++) {
        std::string currentBlock = WorkflowList[WorkflowSequence[i]];
        std::string currentWorker;
        std::cout << "Current block: " << currentBlock << std::endl;
        unsigned int delim = currentBlock.find(" ");
        if (delim == /*std::string::npos*/ 4294967295) {
            currentWorker = currentBlock;
        }
        else {
            currentWorker = currentBlock.substr(0, delim);
            std::string argument1 = currentBlock.substr(delim + 1);
            Text.push_back(argument1);
            unsigned int secondDelim = currentBlock.find(" ", delim + 1);
            if (secondDelim != /*std::string::npos*/ 4294967295) {
                std::string argument2 = currentBlock.substr(secondDelim + 1);
                Text.push_back(argument2);
            }
        }//text + all argumets are now loaded into Text

        currentWorkerObjP = FactoryMethod(currentWorker, Text);//get the block object

        Text = currentWorkerObjP->execute();

        delete currentWorkerObjP;

    }
}



WorkflowReader::WorkflowReader() {}

WorkflowExecutor WorkflowReader::getWorkflow() { //throws exceptions if fails to read

    std::ifstream f("workflow.txt");
    std::vector<std::string> input;
    std::string line;

    if (f.is_open()) {
        while (std::getline(f, line)) {
            input.push_back(line);
        }
        f.close();
    } else {
        throw WREx;
    }

    //workflow check will happen during execution
    //convertation to convenient list of blocks:
    std::unordered_map<int, std::string> WorkflowList;

    for (unsigned int i = 1; i <= input.size() - 3; i++) {
        unsigned int numberEnd = input[i].find(" ");
        unsigned int number = std::stoi(input[i].substr(0, numberEnd));

        unsigned int stringBegin = numberEnd + 3;
        std::string string = input[i].substr(stringBegin);

        WorkflowList[number] = string;
    }

    std::vector<int> WorkflowSequence;

    unsigned int numberStart = 0;
    unsigned int numberEnd;
    std::string seq = input[input.size() - 1];
    while (1) {
        numberEnd = seq.find(" ", numberStart);
        if (numberEnd == /*std::string::npos*/ 4294967295) {
            WorkflowSequence.push_back(std::stoi(seq.substr(numberStart)));
            break;
        } else {
            WorkflowSequence.push_back(std::stoi(seq.substr(numberStart, numberEnd - numberStart)));
            numberStart = numberEnd + 4;
        }

    }

    WorkflowExecutor WE(WorkflowList, WorkflowSequence);

    return WE;

}
