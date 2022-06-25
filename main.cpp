#include "Algorithms.hpp"
#include "read_instances.hpp"

#define OPEN_FILE(fileObj, fileName)                                           \
    do {                                                                       \
        fileObj.open(fileName);                                                \
        if (!fileObj.is_open()) {                                              \
            std::cerr                                                          \
                << " error opening file `"                                     \
                << fileName << "`\n";                                          \
            return 1;                                                          \
        }                                                                      \
    } while (0)

int main(int argc, char const *argv[])
{
    const char *programName = *argv;
    if (argc != 5) {
        std::cerr << "Usage: " << programName << " INPUT_FILE OUTPUT_FILE HANDOVER ALPHA\n";
        std::cerr << "HANDOVER = 0 => READ RAN AND SPARSE\nHANDOVER = 1 => READ HANDOVER\n";       
        return 2;
    }

    std::ifstream inputFile;
    OPEN_FILE(inputFile, argv[1]);
    std::ofstream outputFile;
    OPEN_FILE(outputFile, argv[2]);
    std::string handover = argv[3];

    float alpha = std::stof(argv[4]);

    std::shared_ptr<Algorithms> a(new Algorithms(read(inputFile, handover))); 
    std::shared_ptr<Solution> s(a->greedyNodes());
    
    s->print_solution(outputFile);

    std::cout << "\nQualidade Cheaper: " << s->solution_cost << "\n";
    
    if (!s->solutionViable()) {
        std::cout << "\n\nNão é viável!\n\n";
    }


    return 0;
}
