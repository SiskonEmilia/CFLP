#include "individual.hpp"
#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cstdio>

using namespace std;

int main() {
  // Log file stream initializing //
  ofstream csvOutput;
  ofstream detailOutput;
  csvOutput.open("result.csv");
  detailOutput.open("detail.txt");
  csvOutput << ",Result,Time(s)\n";

  // Timer //
  time_t start;
  // Status monitor //
  int count = 0, scoreRecord;
  int* facilities = nullptr;
  // Case info //
  int consumerNum, facilityNum;

  // Set possibility for crossover and mutating //
  int crossover_rate, mutating_rate;
  do{
    cout << "Set your possibility of crossover and mutating (in percentage): " << endl
    << "(format: {crossover_rate} {mutating_rate}, recommend: 20 5)" << endl;
    cin >> crossover_rate >> mutating_rate;
  } while((crossover_rate > 100 || mutating_rate > 100 ||
    crossover_rate < 0 || mutating_rate < 0) && 
    cout << "possibility should be between 0 and 100" << endl);
  
  // Set population size //
  int populationSize, pairSize;
  do {
    cout << "Set your population size (can be divided by 4):";
    cin >> populationSize;
    pairSize = populationSize / 4;
  } while(populationSize <= 0 && populationSize % 4 != 0  && cout << "Population size should be larger than 0 and can be divided by 4. (recommend: 20)" << endl);

  
  Individual::setGAFactors(crossover_rate, mutating_rate);

  // Directory path //
  string rootDir = "../Instances/p";
  // Load each file and execute algorithm //
  for (int number = 1; number < 72; ++number) {
  
    try {
      Individual::init(rootDir + to_string(number));
    }
    catch(string str) {
      cout << str;
      return 0;
    }
    catch(exception e) {
      cout << "Error while initializing: " << e.what() << endl;
      return 0;
    }
  
    // Initializing for case //
    count = 0;
    Individual bestScore;
    vector<Individual> *population = new vector<Individual>(populationSize);
    vector<Individual> *newPopulation;
    scoreRecord = bestScore.getCost();
    consumerNum = Individual::getCustomerNum();
    facilityNum = Individual::getFacilityNum();

    if (facilities != nullptr) delete[] facilities;
    facilities = new int[facilityNum];
    for (int i = 0; i < facilityNum; ++i) facilities[i] = 0;
    // start timer //
    start = time(nullptr);

    do {
      // create a new population //
      newPopulation = new vector<Individual>;

      // randomly select two player to compare // 
      // place the better one into the new population //
      for (int i = 0; i < pairSize * 2 + 1; ++i) {
        auto &player1 = (*population)[rand() % populationSize],
          &player2 = (*population)[rand() % populationSize];
        if (player1.getCost() < player2.getCost()) {
          newPopulation->push_back(player1);
        } else {
          newPopulation->push_back(player2);
        }
      }

      // reserve the best one //
      newPopulation->push_back(bestScore);

      // Find your mate and do something //
      for (int i = 0; i < pairSize + 1; ++i) {
        int *gene1 = (*newPopulation)[i * 2].getGeneCopy(),
          *gene2 = (*newPopulation)[i * 2 + 1].getGeneCopy();
        int tryCount = 0;
        do {
          // try your best to have valid child //
          Individual::onePointCrossover(gene1, gene2);
          Individual::twoPointsCrossover(gene1, gene2);
          Individual::mutate(gene1);
          Individual::mutate(gene2);
          ++tryCount;
        } while((!Individual::isValid(gene1) && !Individual::isValid(gene2)) 
          && (tryCount < 1000));
        
        // Put children into new population //
        newPopulation->push_back(Individual(gene1));
        newPopulation->push_back(Individual(gene2));
      }

      // Find the best one //
      for (auto& individual : *newPopulation) {
        if (individual.getCost() < bestScore.getCost()) {
          bestScore = individual;
        }
      }

      // Kill the old generation //
      delete population;
      // Time goes in turn //
      population = newPopulation;

      // Are the best still him? //
      if (bestScore.getCost() == scoreRecord) {
        if (++count > 180000) {
          // After 180000 times of trying, I give up find a better one //
          time_t timeSpent = time(nullptr) - start;
          csvOutput << "p" << number << "," << scoreRecord
            << "," <<  timeSpent<< "\n";
          detailOutput << "Testcase: p" << number 
            << " Final Cost: " << scoreRecord 
            << " Time Spent: " << timeSpent << " second(s)" << endl;
          int *solution = bestScore.getSolution();
          for (int t = 0; t < consumerNum; ++t) {
            facilities[solution[t]] = 1;
          }

          for (int t = 0; t < facilityNum; ++t) {
            detailOutput << facilities[t] << " ";
          }
          detailOutput << endl;
          for (int t = 0; t < consumerNum; ++t) {
            detailOutput << solution[t] << " ";
          }
          detailOutput << endl;
          delete newPopulation;
          cout << "p" << number << " Done" << endl;
          break;
        }
      } else {
        // Oh! A new king! //
        cout << "New Best At" << count << " :" << scoreRecord << endl;
        scoreRecord = bestScore.getCost();
        count = 0;
      }
    } while (true);
  }
  if (facilities != nullptr) delete[] facilities;
  detailOutput.close();
  csvOutput.close();
  return 0;
}