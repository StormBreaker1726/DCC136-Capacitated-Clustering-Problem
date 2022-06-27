#include "Algorithms.hpp"
#include "read_instances.hpp"

#include <chrono>

using relogio_t = std::chrono::high_resolution_clock;
using tempo_t  = relogio_t::time_point;

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

    long seed = time(NULL)*time(NULL)/rand();
    srand(seed);

    std::ifstream inputFile;
    OPEN_FILE(inputFile, argv[1]);
    std::ofstream outputFile;
    OPEN_FILE(outputFile, argv[2]);
    std::string handover = argv[3];

    float alpha = std::stof(argv[4]);

    std::shared_ptr<Algorithms> a(new Algorithms(read(inputFile, handover)));

    tempo_t start = relogio_t::now();
    std::shared_ptr<Solution> s(a->greedy(alpha, 100));
    tempo_t end = relogio_t::now();
    double delta = std::chrono::duration_cast<std::chrono::milliseconds>((end) - (start)).count();
    
    outputFile << "Seed: " << seed << "\n";
    outputFile << "Qualidade: " << s->solution_cost << "\n";
    outputFile << "Tempo de execução: " << delta << "\n\n";
    
    s->print_solution(outputFile);

    std::cout << "\nSeed: " << seed << "\n";
    std::cout << "Qualidade: " << s->solution_cost << "\n";
    std::cout << "Tempo de execução: " << delta << "\n";
    
    if (!s->solutionViable()) {
        std::cout << "\n\nNão é viável!\n\n";
    }


    return 0;
}
