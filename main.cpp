
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <exception>

#include "Classes.h"

int main() {

    try {

        WorkflowReader WR{};

        WorkflowExecutor WE = WR.getWorkflow();

        WE.execute();
    }
    catch (const WorkflowException& ex) {

        std::cout << "Exception: " << ex.what() << std::endl;
        return -1;

    }

    return 0;
}
