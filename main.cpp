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
    if (argc != 4) {
        std::cerr << "Usage: " << programName << " INPUT_FILE OUTPUT_FILE ALPHA\n\n";        
        return 2;
    }

    std::ifstream inputFile;
    OPEN_FILE(inputFile, argv[1]);

    std::shared_ptr<Algorithms> a(new Algorithms(read_handover(inputFile)));
    std::shared_ptr<Solution> s(a->greedy());
    s->print_solution();
    std::cout << "\nQualidade: " << s->solution_cost << "\n";

    return 0;
}
