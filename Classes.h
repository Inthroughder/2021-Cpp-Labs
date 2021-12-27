
#ifndef LAB2_CLASSES_H
#define LAB2_CLASSES_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <exception>


//-------------------------------EXCEPTIONS-STARTED-------------------------------------


class WorkflowException : public std::exception {
public:
    WorkflowException();
    virtual const char* what() const noexcept = 0;
};

class WorkflowReadingException : public WorkflowException {
public:
    const char* what() const noexcept;
};

class InconsistentArgumentException : public WorkflowException {
public:
    const char* what() const noexcept;
};

class TextReadingException : public WorkflowException {
public:
    const char* what() const noexcept;
};

class TextWritingException : public WorkflowException {
public:
    const char* what() const noexcept;
};


//-------------------------------EXCEPTIONS-ENDED--------------------------------------

//-------------------------------WORKERS-STARTED-------------------------------------

class Worker {
public:
    virtual std::vector <std::string> execute() = 0;
    ~Worker();
};

class Readfile : public Worker {
private:
    std::vector<std::string> Text;
    std::string filename;
public:

    Readfile(std::vector <std::string> filenameInput);

    std::vector <std::string> execute();

};

class Writefile : public Worker {
private:
    std::string filename;
    std::vector<std::string> Text;
public:
    Writefile(std::vector <std::string> InputText);

    std::vector<std::string> execute();

};

class Grep : public Worker {
private:
    std::string requiredWord;
    std::vector<std::string> Text;
public:
    Grep(std::vector <std::string> TextInput);

    std::vector <std::string> execute();

};

class Sort : public Worker {
private:
    std::vector<std::string> Text;
public:
    Sort(std::vector <std::string> TextInput);

    std::vector <std::string> execute();
};

class Replace : public Worker {
private:
    std::vector<std::string> Text;
    std::string oldWord;
    std::string newWord;
public:
    Replace(std::vector <std::string> TextInput);

    std::vector <std::string> execute();

};

class Dump : public Worker {
private:
    std::vector<std::string> Text;
    std::string filename;
public:
    Dump(std::vector <std::string> TextInput);

    std::vector <std::string> execute();

};


//-------------------------------WORKERS-ENDED-------------------------------------

//-------------------------------EXECUTOR AND READER-STARTED-------------------------------------

class WorkflowExecutor {

private:

    int input;
    std::vector <int> WorkflowSequence;
    std::unordered_map <int, std::string> WorkflowList;
    std::vector <std::string> Text;
    Worker* currentWorkerObjP;

public:

    WorkflowExecutor(std::unordered_map <int, std::string> WorkflowListInput, std::vector <int> WorkflowSequenceInput);

    Worker* FactoryMethod(std::string workerName, std::vector <std::string> InputText);

    void execute();

};

class WorkflowReader {

public:

    WorkflowReader();

    WorkflowExecutor getWorkflow();

};

#endif //LAB2_CLASSES_H
