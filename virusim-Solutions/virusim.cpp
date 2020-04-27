//
// Simula��o de propaga��o de v�rus.
// Adaptada de um c�digo proposto por David Joiner (Kean University).
//
// Uso: virusim <tamanho-da-populacao> <nro. experimentos> <probab. maxima>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <fstream>
#include <omp.h>
#include <functional>
#include <vector>
#include "Random.h"
#include "Population.h"


enum Solution {SEQ, S1, S2};

typedef std::chrono::time_point<std::chrono::steady_clock> time_p;

std::string returnSchedStr(omp_sched_t schedule){
  switch(schedule){
    case omp_sched_static: return "STATIC";
    case omp_sched_dynamic: return "DYNAMIC";
    case omp_sched_guided: return "GUIDED";
    case omp_sched_auto: return "AUTO";
    default: return "RUNTIME";
  }
  return "RUNTIME";
}


void print_time(std::ofstream& fileOutput, time_p  st, time_p end){

  fileOutput << "Elapsed time in milliseconds : "
  << std::chrono::duration_cast<std::chrono::milliseconds>(end - st).count()
  << " ms" << std::endl;

  fileOutput << "Elapsed time in seconds : "
  << std::chrono::duration_cast<std::chrono::seconds>(end - st).count()
  << " s" << std::endl;


}


void sequencial(int population_size, int n_probs, int n_trials){
  double prob_min = 0.0, prob_max = 1.0;
  int base_seed = 100;

  Population* population = new Population(population_size);
  Random rand;

  double* prob_spread = new double[n_probs];
  double* percent_infected = new double[n_probs];
  double prob_step = (prob_max - prob_min)/(double)(n_probs-1);

  for (int ip = 0; ip < n_probs; ip++) {
      prob_spread[ip] = prob_min + (double) ip * prob_step;
      percent_infected[ip] = 0.0;

      rand.setSeed(base_seed+ip); // nova seq��ncia de n�meros aleat�rios

      for (int it = 0; it < n_trials; it++) {
          population->propagateUntilOut(population->centralPerson(), prob_spread[ip], rand);
          percent_infected[ip] += population->getPercentInfected();
      }

        percent_infected[ip] /= n_trials;
        printf("%lf, %lf\n", prob_spread[ip], percent_infected[ip]);
     }

    delete[] prob_spread;
    delete[] percent_infected;
}


void solution1(int population_size, int n_probs, int n_trials, int n_threads){
  double prob_min = 0.0, prob_max = 1.0;
  int base_seed = 100;

  std::vector<Population*> populations(n_threads);
  for(auto& pops: populations)
    pops = new Population(population_size);

  Random rand;

  double* prob_spread = new double[n_probs];
  double* percent_infected = new double[n_probs];
  double prob_step = (prob_max - prob_min)/(double)(n_probs-1);

  for (int ip = 0; ip < n_probs; ip++) {
      prob_spread[ip] = prob_min + (double) ip * prob_step;
      percent_infected[ip] = 0.0;

      rand.setSeed(base_seed+ip); // nova seq��ncia de n�meros aleat�rios
      #pragma omp parallel for num_threads(n_threads) schedule(runtime)
      for (int it = 0; it < n_trials; it++) {
          int pop_id = omp_get_thread_num();
          populations[pop_id]->propagateUntilOut(populations[pop_id]->centralPerson(), prob_spread[ip], rand);
          percent_infected[ip] += populations[pop_id]->getPercentInfected();
      }

      percent_infected[ip] /= n_trials;
      printf("%lf, %lf\n", prob_spread[ip], percent_infected[ip]);
  }

  delete[] prob_spread;
  delete[] percent_infected;

  for(auto pops: populations)
    delete pops;
}


void solution2(int population_size, int n_probs, int n_trials, int n_threads){
  double prob_min = 0.0, prob_max = 1.0;
  int base_seed = 100;

  std::vector<Population*> populations(n_threads);
  for(auto& pops: populations)
    pops = new Population(population_size);

  Random rand;

  double* prob_spread = new double[n_probs];
  double* percent_infected = new double[n_probs];
  double prob_step = (prob_max - prob_min)/(double)(n_probs-1);

  #pragma omp parallel for num_threads(n_threads) schedule(runtime)
  for (int ip = 0; ip < n_probs; ip++) {
      int pop_id = omp_get_thread_num();
      prob_spread[ip] = prob_min + (double) ip * prob_step;
      percent_infected[ip] = 0.0;

      rand.setSeed(base_seed+ip); // nova seq��ncia de n�meros aleat�rios

      for (int it = 0; it < n_trials; it++) {
        populations[pop_id]->propagateUntilOut(populations[pop_id]->centralPerson(), prob_spread[ip], rand);
        percent_infected[ip] += populations[pop_id]->getPercentInfected();
      }

      percent_infected[ip] /= n_trials;
      printf("%lf, %lf    thread id = %d\n", prob_spread[ip], percent_infected[ip], pop_id);
  }



  delete[] prob_spread;
  delete[] percent_infected;

  for(auto pops: populations)
    delete pops;
}


int run_program(std::ofstream& fileOutput, Solution solution,  int population_size, int n_probs, int n_trials, int n_threads){
  int ck = 0;
  omp_sched_t sched_t = omp_sched_auto;
  auto func = std::function<void()>();

  switch (solution) {
    case SEQ: fileOutput<<"Sequencial "<<"\n";
              func = std::bind(sequencial, population_size, n_probs, n_trials);
      break;
    case S1:  fileOutput<<"Parallel 1  "<<"\n";
              sched_t = omp_sched_dynamic;
              ck = n_trials/2;
              func = std::bind(solution1, population_size, n_probs, n_trials, n_threads);
      break;
    case S2:  fileOutput<<"Parallel 2 "<<"\n\n";
              sched_t = omp_sched_dynamic;
              ck = n_probs/n_threads;
              func = std::bind(solution2, population_size, n_probs, n_trials, n_threads);
      break;
  }

  std::cout<<"Pop = "<<population_size<<" Prob = "<<n_probs<<" N tirals = "<<n_trials<<" n_threads"<<n_threads<<"\n";


  omp_set_schedule(sched_t, ck);
  printf("Probabilidade, Percentual Infectado\n");

  auto st = std::chrono::steady_clock::now();
  try {
    func();
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Erro: alocacao de memoria" << std::endl;
    fileOutput<<"Erro: alocacao de memoria" << std::endl;
    fileOutput.close();
    return 1;
  }

  auto end = std::chrono::steady_clock::now();
  if(solution!=SEQ)
    fileOutput<<"Num threads = "<<n_threads<<" Schedule = "<<returnSchedStr(sched_t)<<" Chunk = "<<ck<<"\n";

  fileOutput<<"Size pop = "<<population_size<<" N trials = "<<n_trials<<" N probs = "<<n_probs<<"\n";
  print_time(fileOutput, st, end);
  fileOutput<<"\n----------------------------------------------------------------------------------\n\n\n";

  return 0;
}

int test_solution1(std::ofstream& fileOutput){
  int population_size = 40;
  int n_trials = 500;
  int n_probs = 100;
  int n_threads = 2;
  int result = 0;

  for(int i = 0;i<9;i++){
    if(i%3==0){
      n_threads = 2;
      if(i>0)
        n_trials*=5;

      result = run_program(fileOutput, static_cast<Solution>(0), population_size,n_probs,  n_trials, n_threads);

      if(result){
         return 1;
         fileOutput<<"Error when running sequential test\n";
       }
     }

    result = run_program(fileOutput, S1, population_size,n_probs,  n_trials, n_threads);
    if(result){
       return 1;
       fileOutput<<"Error when running solution 1 test\n";
     }

     n_threads+=2;
   }

   return 0;
}

int test_solution2(std::ofstream& fileOutput){
  int population_size = 50;
  int n_trials = 100;
  int n_probs = 100;
  int n_threads = 2;
  int result = 0;

  for(int i = 0;i<9;i++){
    if(i%3==0){
      n_threads = 2;
      if(i>0){
        population_size+=50;
      }

      result = run_program(fileOutput,SEQ, population_size,n_probs,  n_trials, n_threads);

      if(result){
         return 1;
         fileOutput<<"Error when running sequential test\n";
       }
     }

    result = run_program(fileOutput, S2, population_size,n_probs,  n_trials, n_threads);
    if(result){
       return 1;
       fileOutput<<"Error when running solution 2 test\n";
     }

     n_threads+=2;
   }



   return 0;
}


int main(int argc, char* argv[]){

   std::ofstream fileOutput("../results.txt", std::ios::out | std::ios::trunc);
   omp_set_dynamic(0);

   int result = test_solution1(fileOutput);
   if(result) return 1;

   result = test_solution2(fileOutput);
   if(result) return 1;

   fileOutput.close();
   return 0;
}
