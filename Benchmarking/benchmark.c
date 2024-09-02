#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <unistd.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define PURPLE "\033[35m"

typedef struct {
    const char *folder_name;
    const char *compile_cmd;
    const char *run_cmd;
    const char *remove_cmd;
    double avg_time;
} benchmark_info_t;

void printcolor(const char *color, const char *msg) {
    printf("%s%s%s", color, msg, RESET);
}

void compile_and_run(const benchmark_info_t *info) {
    struct timespec start, end;
    double real_time_used, total_time = 0;
    int iterations = 5;

    // Move to folder
    if (chdir(info->folder_name) != 0) {
        fprintf(stderr, "%sError changing directory to %s%s\n", RED, info->folder_name, RESET);
        return;
    }
    
    // Compile the source code
    printcolor(GREEN, "Compiling ");
    printcolor(YELLOW, info->folder_name);
    printcolor(GREEN, " program...\n");
    if (system(info->compile_cmd) != 0) {
        fprintf(stderr, "%sCompilation failed for %s program.%s\n", RED, info->folder_name, RESET);
        chdir("..");
        return;
    }
    
    // Measure the average execution time
    printcolor(GREEN, "Running ");
    printcolor(YELLOW, info->folder_name);
    printcolor(GREEN, " program...\n");

    for (int i = 0; i < iterations; i++) {
        clock_gettime(CLOCK_REALTIME, &start);
        if (system(info->run_cmd) != 0) {
            fprintf(stderr, "%sExecution failed for %s program.%s\n", RED, info->folder_name, RESET);
            chdir("..");
            return;
        }
        clock_gettime(CLOCK_REALTIME, &end);

        // Calculate time for this iteration
        real_time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        total_time += real_time_used;  // Accumulate total time

        // Print time
        printf("[%d] %s%f seconds%s\n", i + 1, YELLOW, real_time_used, RESET);
    }

    // Remove the executable
    system(info->remove_cmd);

    // Calculate and print the average time
    double avg_time = total_time / iterations;
    printcolor(GREEN, "Average time taken to execute ");
    printcolor(YELLOW, info->folder_name);
    printcolor(GREEN, " program: ");
    printf("%s%f seconds%s\n\n", YELLOW, avg_time, RESET);

    // Update the average time in the benchmark info
    benchmark_info_t *info_ptr = (benchmark_info_t *)info;
    info_ptr->avg_time = avg_time;

    // Move back to the original folder
    if (chdir("..") != 0) {
        fprintf(stderr, "%sError changing directory to the original folder%s\n", RED, RESET);
        return;
    }
}

int main() {
    // Define benchmark info for each program
    benchmark_info_t programs[] = {
        {"C", "gcc -o matinv_c ./matinv.c", "./matinv_c", "rm matinv_c", 0},
        {"C++", "g++ -o matinv_cpp ./matinv.cpp", "./matinv_cpp", "rm matinv_cpp", 0},
        {"Java", "javac Matinv.java", "java Matinv", "rm Matinv.class", 0},
        {"Python2", "", "python2 ./matinv.py", "", 0},
        {"Python3", "", "python3 ./matinv.py", "", 0},
        {"Go", "go build matinv.go", "./matinv", "rm matinv", 0},
        {"Javascript", "", "node ./matinv.js", "", 0},
        {"Perl", "", "perl ./matinv.pl", "", 0},
        {"Ruby", "", "ruby ./matinv.rb", "", 0},
        {"Rust", "rustc matinv.rs", "./matinv", "rm matinv", 0},
        {"C#", "mcs -out:matinv.exe matinv.cs", "mono matinv.exe", "rm matinv.exe", 0},
        {"PHP", "", "php matinv.php", "", 0},
        {"Dart", "", "dart run matinv.dart", "", 0},
        {"Julia", "", "julia matinv.jl", "", 0},
        {"F#", "fsharpc matinv.fs -o matinv.exe", "mono matinv.exe", "rm matinv.exe", 0},
        {"Pascal", "fpc matinv.pas", "./matinv", "rm matinv && rm matinv.o", 0},
        {"Lua", "", "lua matinv.lua", "", 0},
        {"Elixir", "", "elixir matinv.exs", "", 0},
        {"Swift", "", "swift matinv.swift", "", 0},
        {"Kotlin", "kotlinc matinv.kt -include-runtime -d matinv.jar", "java -jar matinv.jar", "rm matinv.jar", 0}
    };
    int num_programs = sizeof(programs) / sizeof(programs[0]);

    for (int i = 0; i < num_programs; i++) {
        compile_and_run(&programs[i]);
    }

    // Print the benchmark results
    printcolor(GREEN, "Benchmark results:\n");
    for (int i = 0; i < num_programs; i++) {
        printf("[%2d] %s%f seconds | %s%s%s\n", i + 1, YELLOW, programs[i].avg_time, PURPLE, programs[i].folder_name, RESET);
    }

    return 0;
}
