#ifndef INDIVIDUAL_HPP
#define INDIVIDUAL_HPP
#include <string>

using std::string;


class Individual {
private:
  // Static variables from data file //
  static int    FACILITY_NUM;     // Number of facilities
  static int    CUSTOMER_NUM;     // Number of customers
  static int**  ASSIGN_COST;      // Matrix of customer's demand
  static int*   MIN_ASSIGN_COST;  // Minimal assign cost for each consumer
  static int*   MIN_ASSIGN;       // Minimal facility index
  static int*   CUSTOMER_DEMAND;  // Vector of facilities' assignment cost
  static int*   OPEN_COST;        // The cost of facility to open
  static int*   FACILITY_CAPACITY;// The capacity of each facility

  // Object's private data member //
  int*          gene;             // Gene, the strategy of assignment
  int           cost;

public:
  // Static methods //
  static void init(string filePath);              // Use data file to initialize static values
  static int  estimateCost(Individual &individual); // estimate grade for individual
  static void clearData();                        // Clear data and free memory space
  static bool isValid(int* gene);                 // Check whether a gene is valid

  static int getFacilityNum();
  static int getCustomerNum();

  // Constructors //
  Individual();
  ~Individual();
  Individual(int* gene);
  Individual(const Individual& individual);

  // Public methods //
  int*    getSolution();  // Get the solution
  int*    getGeneCopy();  // Get a copy of gene
  int     getCost();      // Get the cost
  Individual** getNeighborsWithTryMin();
  Individual& operator=(const Individual& individual);
};
#endif