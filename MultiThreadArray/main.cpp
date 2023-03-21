#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <thread>
#include <chrono>
#include <functional>

void generate_random_vector(std::vector<long long>& in, int seed, unsigned long long size)
{
    srand(seed);
    in.clear();
    in.reserve(size);
    for (unsigned long long i = 0; i < size; ++i)
    {
        in.push_back(rand() % 100);
    }
}

void print_vector(std::vector<long long>& in)
{
    for (size_t i = 0; i < in.size(); ++i)
    {
        std::cout << in[i] << " ";
    }
    std::cout << "\n";
}

void single_thread_summ(std::vector<long long>& in, int left, int right, long long& summ)
{
    for (int i = left; i <= right; ++i)
    {
        summ += in[i];
    }
}

long long multi_thread_summ(std::vector<long long>& in, int left, int right, int threads_num)
{
    long long summ = 0;                       
    long long total_size = right - left + 1; 
    long long part_size = total_size / threads_num;
    long long last_part_size = total_size % threads_num;

    std::vector<std::thread> threads;

    int partial_size;
    if (last_part_size == 0) partial_size = threads_num;
    else partial_size = threads_num + 1;

    std::vector<long long> partial_summs(partial_size);
    for (int i = 0; i < threads_num; ++i)
    {
        int current_left = part_size * i;
        int current_right = (part_size * (i + 1)) - 1;
        threads.push_back(std::thread(single_thread_summ, std::reference_wrapper<std::vector<long long> >(in),
         current_left, current_right, std::reference_wrapper<long long>(partial_summs[i])));
    }

    
    for (int i = 0; i < threads_num; ++i)
    {
        threads[i].join();     
    }


    if (last_part_size != 0)
    {
        int last_left = right - last_part_size + 1;
        threads.push_back(std::thread(single_thread_summ, std::reference_wrapper<std::vector<long long> >(in), last_left, right, std::reference_wrapper<long long>(partial_summs[threads_num])));
        threads[threads_num].join();
    }


    single_thread_summ(partial_summs, 0, partial_summs.size() - 1, summ);
    return summ;
}


int main()
{
    std::vector<long long> v;
    long long summ = 0;
    int randomseed = 16;
    generate_random_vector(v, randomseed, 5000000);
    std::cout << "\n\n\n";
    auto start1 = std::chrono::high_resolution_clock::now();
    single_thread_summ(v, 0, v.size() - 1, summ);
    auto stop1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time1 = stop1 - start1;
    std::cout << "Single thread result: " << summ << "   array size: " << v.size() << "\n";
    std::cout << "Elapsed time = " << elapsed_time1.count() << " sec\n";
    std::cout << "\n\n\n";
    
    for (int i = 1; i < 100; ++i)
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::cout << "Threads number: " << i << " result: " << multi_thread_summ(v, 0, v.size() - 1, i) << "\n";
        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = stop - start;
        std::cout << "Elapsed time = " << elapsed_time.count() << " sec\n";
    }
    return 0;
}