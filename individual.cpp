#include <fstream>
#include <cstdlib>
#include <algorithm>
#include "individual.hpp"

using std::ifstream;
using std::rand;
using std::swap;
#define DEBUG_MODE
#ifdef DEBUG_MODE
#include <iostream>
using std::cout;
using std::endl;
#endif

// Initialize static member variables in Individual //
int    Individual::FACILITY_NUM      = 0;     
int    Individual::CUSTOMER_NUM      = 0;     
int**  Individual::ASSIGN_COST       = nullptr;  
int*   Individual::CUSTOMER_DEMAND   = nullptr;      
int*   Individual::OPEN_COST         = nullptr; 
int*   Individual::FACILITY_CAPACITY = nullptr;    
int Individual::CROSSOVER_RATE    = 0;
int Individual::MUTATION_RATE     = 0;   

void Individual::clearData() {
  if (ASSIGN_COST != nullptr){
    for (int i = 0; i < CUSTOMER_NUM; ++i) {
      delete[] ASSIGN_COST[i];
    }
    delete[] ASSIGN_COST;
    ASSIGN_COST = nullptr;
  }
  if (CUSTOMER_DEMAND != nullptr) {
    delete[] CUSTOMER_DEMAND;
    CUSTOMER_DEMAND = nullptr;
  }
  if (OPEN_COST != nullptr) {
    delete[] OPEN_COST;
    OPEN_COST = nullptr;
  }
  if (FACILITY_CAPACITY != nullptr) {
    delete[] FACILITY_CAPACITY;
    FACILITY_CAPACITY = nullptr;
  }
  // FACILITY_NUM = CUSTOMER_NUM = 0;
  // CROSSOVER_RATE = MUTATION_RATE = 0.0;
}

void Individual::init(string filePath) {
  // Create a input file stream //
  ifstream is;
  is.open(filePath); 
  double temp = 0;
  
  // If the file is not open properly //
  if (!is.is_open()) { 
    throw string("E: Unable To Open File");
    return;
  }

  // Clear existed data and free memory space
  clearData();
  
  // Number of facilities and customers //
  is >> FACILITY_NUM >> CUSTOMER_NUM;
  
  // Capacity and Open Cost //
  FACILITY_CAPACITY = new int[FACILITY_NUM];
  OPEN_COST = new int [FACILITY_NUM];
  for (int i = 0; i < FACILITY_NUM; ++i) {
    is >> FACILITY_CAPACITY[i] >> OPEN_COST[i];
  }

  // Demand of customers //
  CUSTOMER_DEMAND = new int[CUSTOMER_NUM];
  for (int i = 0; i < CUSTOMER_NUM; ++i) {
    is >> temp;
    CUSTOMER_DEMAND[i] = temp;
  }

  // Assign Cost //
  ASSIGN_COST = new int*[CUSTOMER_NUM];
  for (int i = 0; i < CUSTOMER_NUM; ++i) {
    ASSIGN_COST[i] = new int[FACILITY_NUM];
    for (int t = 0; t < FACILITY_NUM; ++t) {
      is >> temp;
      ASSIGN_COST[i][t] = temp;
    }
  }

  #ifdef DEBUG_MODE
    cout << "Status after reading data:" << endl
      << "Facility Number: " << FACILITY_NUM << endl
      << "Customer Number: " << CUSTOMER_NUM << endl
      << "Assign Cost: " << endl;
    for (int i = 0; i < CUSTOMER_NUM; ++i) {
      for (int t = 0; t < FACILITY_NUM; ++t) {
        cout << ASSIGN_COST[i][t] << " ";
      }
      cout << endl;
    }
    cout << "Facility Capacities: " << endl;
    for (int i = 0; i < FACILITY_NUM; ++i) {
      cout << FACILITY_CAPACITY[i] << " ";
    }
    cout << endl
      << "Facility Open Cost: " << endl;
    for (int i = 0; i < FACILITY_NUM; ++i) {
      cout << OPEN_COST[i] << " ";
    } 
    cout << endl
      << "Customers' demand: " << endl;
    for (int i = 0; i < CUSTOMER_NUM; ++i) {
      cout << CUSTOMER_DEMAND[i] << " ";
    }
    cout << endl;
  #endif
}

void Individual::setGAFactors(int CROSSOVER_RATE,
  int MUTATION_RATE) {
  Individual::CROSSOVER_RATE = CROSSOVER_RATE;
  Individual::MUTATION_RATE = MUTATION_RATE;
}

int Individual::estimateCost(Individual& individual) {
  if (!isValid(individual.gene))
    return INT32_MAX;
  bool* facilities = new bool[FACILITY_NUM];
  int cost = 0;
  for (int i = 0; i < FACILITY_NUM; ++i) {
    facilities[i] = false;
  }

  for (int i = 0; i < CUSTOMER_NUM; ++i) {
    if (facilities[individual.gene[i]] == false) {
      cost += OPEN_COST[individual.gene[i]];
      facilities[individual.gene[i]] = true;
    }
    cost += ASSIGN_COST[i][individual.gene[i]];
  }

  delete []facilities;
  return individual.cost = cost;
}

void Individual::onePointCrossover(int* parentGene1,
  int* parentGene2) {
    if (rand() % 100 < (CROSSOVER_RATE / 2)) {
      int swapPoint = rand() % CUSTOMER_NUM;
      for (int i = 0; i < swapPoint; ++i) {
        swap(parentGene1[i], parentGene2[i]);
      }
    }
}

void Individual::twoPointsCrossover(int* parentGene1,
  int* parentGene2) {
  if (rand() % 100 < (CROSSOVER_RATE / 2)) {
    int swapPoint1 = rand() % CUSTOMER_NUM;
    int swapPoint2 = rand() % CUSTOMER_NUM;
    if (swapPoint2 < swapPoint1) swap(swapPoint1, swapPoint2);
    for (int i = swapPoint1; i <= swapPoint2; ++i) {
      swap(parentGene1[i], parentGene2[i]);
    }
  }
}

void Individual::mutate(int* gene) {
  while(rand() % 100 < MUTATION_RATE) {
    gene[rand() % CUSTOMER_NUM] = rand() % FACILITY_NUM;
  }

  while(rand() % 100 < MUTATION_RATE) {
    swap(gene[rand() % CUSTOMER_NUM], gene[rand() % CUSTOMER_NUM]);
  }
}

Individual::Individual() {
  this->gene = new int[CUSTOMER_NUM];
  
  int* facilities = new int[FACILITY_NUM];
  for (int i = 0; i < FACILITY_NUM; ++i) {
    facilities[i] = 0;
  }

  for (int i = 0; i < CUSTOMER_NUM; ++i) {
    do {
      gene[i] = rand() % FACILITY_NUM;
    } while (CUSTOMER_DEMAND[i] + 
      facilities[gene[i]] > FACILITY_CAPACITY[gene[i]]);
  }
  
  cost = estimateCost(*this);
  delete[] facilities;
}

Individual::~Individual() {
  if (gene != nullptr) delete[] gene;
}

Individual::Individual(int* gene) {
  this->gene = gene;
  cost = estimateCost(*this);
}

int* Individual::getSolution() {
  return gene;
}

bool Individual::isValid(int* gene) {
  int* facilities = new int[FACILITY_NUM];
  bool flag = true;
  for (int i = 0; i < FACILITY_NUM; ++i) {
    facilities[i] = 0;
  }

  for (int i = 0; i < CUSTOMER_NUM; ++i) {
    facilities[gene[i]] += CUSTOMER_DEMAND[i];
    if (facilities[gene[i]] > FACILITY_CAPACITY[gene[i]]) {
      flag = false;
    }
  }

  delete[] facilities;
  return flag;
}

Individual::Individual(const Individual& individual) {
  gene = new int[CUSTOMER_NUM];
  for (int i = 0; i < CUSTOMER_NUM; ++i) {
    gene[i] = individual.gene[i];
  }
  cost = individual.cost;
}

Individual& Individual::operator=(const Individual& individual) {
  if (gene == nullptr) gene = new int[CUSTOMER_NUM];
  for (int i = 0; i < CUSTOMER_NUM; ++i) {
    gene[i] = individual.gene[i];
  }
  cost = individual.cost;
  return *this;
}

int* Individual::getGeneCopy() {
  int *copy = new int[CUSTOMER_NUM];
  for (int i = 0; i < CUSTOMER_NUM; ++i)
    copy[i] = gene[i];
  return copy;
}

int Individual::getCost() {
  return cost;
}

int Individual::getFacilityNum() {
  return FACILITY_NUM;
}
int Individual::getCustomerNum() {
  return CUSTOMER_NUM;
}