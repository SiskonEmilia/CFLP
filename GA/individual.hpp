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
  static int*   CUSTOMER_DEMAND;  // Vector of facilities' assignment cost
  static int*   OPEN_COST;        // The cost of facility to open
  static int*   FACILITY_CAPACITY;// The capacity of each facility
 
  // Static variables for GA //
  static int CROSSOVER_RATE;   
  static int MUTATION_RATE;

  // Object's private data member //
  int*          gene;             // Gene, the strategy of assignment
  int           cost;

public:
  // Static methods //
  static void init(string filePath);              // Use data file to initialize static values
  static void setGAFactors(int CROSSOVER_RATE,
    int MUTATION_RATE);                        // Set GA-related factors
  static int    estimateCost(Individual &individual); // estimate grade for individual
  static void onePointCrossover(int* parentGene1,
    int* parentGene2);                            // Generate new gene with one-point crossover
  static void twoPointsCrossover(int* parentGene1,
    int* parentGene2);                            // Generate new gene with two-points crossover
  static void mutate(int* gene);                  // Generate new gene with mutation
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
  int     getCost();
  Individual& operator=(const Individual& individual);
};
#endif