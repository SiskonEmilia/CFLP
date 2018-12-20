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
    Individual *bestScore = new Individual;
    consumerNum = Individual::getCustomerNum();
    facilityNum = Individual::getFacilityNum();

    if (facilities != nullptr) delete[] facilities;
    facilities = new int[facilityNum];
    for (int i = 0; i < facilityNum; ++i) facilities[i] = 0;
    // start timer //
    start = time(nullptr);

    while ((scoreRecord = bestScore->getCost()) == INT32_MAX) {
      delete bestScore;
      bestScore = new Individual();
      cout << "*";
    }


    cout << "*";
    do {
      auto temp3 = bestScore->getNeighborsWithTryMin();
      
      for (int i = 0; i < consumerNum; ++i) {
        if (bestScore->getCost() > temp3[i]->getCost()) {
          bestScore = temp3[i];
          bestScore->getCost();
        }
        else {
          delete temp3[i];
        }
      }
      delete []temp3;
      // Are the best still him? //
      if (bestScore->getCost() == scoreRecord) {
        time_t timeSpent = time(nullptr) - start;
        csvOutput << "p" << number << "," << scoreRecord
          << "," <<  timeSpent<< "\n";
        detailOutput << "Testcase: p" << number 
          << " Final Cost: " << scoreRecord 
          << " Time Spent: " << timeSpent << " second(s)" << endl;
        int *solution = bestScore->getSolution();
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
        cout << "p" << number << " Done" << endl;
        break;
        
      } else {
        // Oh! A new king! //
        scoreRecord = bestScore->getCost();
        cout << "New Best At " << count << " :" << scoreRecord << endl;
        count = 0;
      }

    } while (true);
  }
  if (facilities != nullptr) delete[] facilities;
  detailOutput.close();
  csvOutput.close();
  return 0;
}
