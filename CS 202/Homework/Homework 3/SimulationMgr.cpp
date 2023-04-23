/**
 * Author: Deniz Tuna ONGUNER
 * ID: 22001788
 * Section: 2
 * Homework: 3
 * Description: Implementations of WildLifeSimulation, Food, and Creature objects; with a main function to run.
 *
 * References:
 * https://www.w3schools.com/cpp/cpp_files.asp
 * https://www.geeksforgeeks.org/vector-in-cpp-stl/
 * https://www.geeksforgeeks.org/command-line-arguments-in-c-cpp/
 */

#include <cmath> // To calculate the distance between two points: sqrt and pow functions.
#include <sstream> // To convert the data fetched from the text file to primitive types.
#include <fstream> // To read the text file and fetch the data from it.
#include "SimulationMgr.h"

/**
 * @brief Calculates the distance between two points.
 *
 * @param x1 The x-coordinate of the first point.
 * @param x2 The x-coordinate of the second point.
 * @param y1 The y-coordinate of the first point.
 * @param y2 The y-coordinate of the second point.
 * @return The distance between the two points.
 */
double calculateTheDistanceBetweenTwoPoints(const double x1, const double x2, const double y1, const double y2) {
    return std::sqrt(std::pow((x2 - x1), 2) + std::pow((y2 - y1), 2));
}

/**
 * @brief Creates a new WildLifeSimulation, then starts it.
 *
 * @return Exit code 0.
 */
int main(int argc, char **argv) {

    if (argc == 2) {

        WildLifeSimulation WLSim;
        WLSim.fetchDataFromFile(std::string(argv[1]));
        WLSim.start();

    } else {

        std::cerr << "Usage: " << std::string(argv[0]) << " filename\n";
        std::exit(EXIT_FAILURE);

    }

    return 0;
}

/**
 * @brief Constructs a new WildLifeSimulation object.
 */
WildLifeSimulation::WildLifeSimulation() : timeAsTicks(0), bestFood(NULL) {
    // pass
}

/**
 * @brief Fetches data from the file passed by the user and populates the creature and food containers.
 *
 * @param fileName The name of the data file that is to be fetched.
 * @throw runtime_error If the file cannot be loaded.
 */
void WildLifeSimulation::fetchDataFromFile(const std::string &fileName) {

    std::ifstream dataFile(fileName);

    if (!dataFile) {
        throw std::runtime_error("Failed to load file: " + fileName);
    }

    std::string line;
    std::getline(dataFile, line);

    const int creaturesCount = std::stoi(line);
    char _ignore;

    for (int i = 0; i < creaturesCount; i++) {

        std::getline(dataFile, line);
        std::stringstream ss(line);

        int creatureId, creatureHealth;
        double creaturePositionX, creaturePositionY;

        ss >> creatureId >> _ignore >> creaturePositionX >> _ignore
           >> creaturePositionY >> _ignore >> creatureHealth;

        creaturesInTheSimulation.push_back(Creature(creatureId, creaturePositionX, creaturePositionY,
                                                    creatureHealth));
    }

    while (std::getline(dataFile, line)) {

        std::stringstream ss(line);

        int foodId, foodQuality, foodSpawnTime;
        double foodPositionX, foodPositionY;

        ss >> foodId >> _ignore >> foodPositionX >> _ignore
           >> foodPositionY >> _ignore >> foodQuality >> _ignore >> foodSpawnTime;

        allFetchedFoodResources.push(Food(foodId, foodPositionX, foodPositionY,
                                          foodQuality, foodSpawnTime));
    }

    dataFile.close();
    std::sort(creaturesInTheSimulation.begin(), creaturesInTheSimulation.end());
}

/**
 * @brief Starts the simulation and runs it until all the creatures in the simulation die.
 */
void WildLifeSimulation::start() {

    while (true) {

        // Exit condition: A wild life simulation should run until all the creatures die.
        if (areAllCreaturesDead()) {
            break;
        }

        // Step 1: Current locations of all the creatures should be printed.
        printOutAllCreatures();

        // Step 2: New food resources should be added if any.
        addNewFoodResourcesIfAny();

        // Step 3: Fights between creatures should be resolved.
        runFightsInBetweenCreatures();

        // Step 4: Creatures should consume the best food if it is nearby.
        makeCreaturesConsumeNearbyFood();

        // Step 4.5 (Intermediate Step): Creatures should move towards the best food.
        moveCreaturesTowardsTheBestFood();

        // Step 5: Health of each creature should decrease by 1.
        decreaseHealthOfAllCreaturesByOne();

        // Last Step: Time, as ticks, should be increased by 1 for the next iterations.
        timeAsTicks += 1;
    }
}

/**
 * @brief Constructs a new (default) Food object.
 */
WildLifeSimulation::Food::Food() {
    // pass
}

/**
 * @brief Constructs a new Food object.
 *
 * @param id The ID of the food.
 * @param x The initial x-coordinate of the food.
 * @param y The initial y-coordinate of the food.
 * @param quality The quality of the food.
 * @param spawnTime The spawn time of the food.
 */
WildLifeSimulation::Food::Food(int id, double x, double y, int quality, int spawnTime) :
        id(id), x(x), y(y), quality(quality), spawnTime(spawnTime) {
    // pass
}

/**
 * @brief Overloads the less-than operator for Food objects.
 *
 * @param otherFood The other Food object to compare.
 * @return True if this food's spawn time is less than the other food's,
           True again if this food's quality is greater than the other food's when their spawn times are equal;
           false otherwise.
 */
bool WildLifeSimulation::Food::operator<(const WildLifeSimulation::Food &otherFood) const {
    if (spawnTime == otherFood.spawnTime) {
        return quality > otherFood.quality;
    }
    return spawnTime < otherFood.spawnTime;
}

/**
 * @brief Returns the x-coordinate of the current location of the food item.
 *
 * @return The current x-coordinate of the food item's location.
 */
double WildLifeSimulation::Food::getX() const {
    return x;
}

/**
 * @brief Returns the y-coordinate of the current location of the food item.
 *
 * @return The current y-coordinate of the food item's location.
 */
double WildLifeSimulation::Food::getY() const {
    return y;
}

/**
 * @brief Returns the quality of the food item.
 *
 * @return The quality of the food item.
 */
int WildLifeSimulation::Food::getQuality() const {
    return quality;
}

/**
 * @brief Returns the time at which this food item was spawned.
 *
 * @return The spawn time of the food item.
 */
int WildLifeSimulation::Food::getSpawnTime() const {
    return spawnTime;
}

/**
 * @brief Resets the spawn time of the food item to -1.
 */
void WildLifeSimulation::Food::resetSpawnTime() {
    spawnTime = -1;
}

/**
 * @brief Constructs a new Creature object.
 *
 * @param id The ID of the creature.
 * @param x The initial x-coordinate of the creature.
 * @param y The initial y-coordinate of the creature.
 * @param health The health of the creature.
 */
WildLifeSimulation::Creature::Creature(int id, double x, double y, int health) : id(id), x(x), y(y), health(health) {
    // pass
}

/**
 * @brief Overloads the less-than operator for Creature objects.
 *
 * @param otherCreature The other Creature object to compare.
 * @return True if this creature's ID is less than the other creature's, false otherwise.
 */
bool WildLifeSimulation::Creature::operator<(const WildLifeSimulation::Creature &otherCreature) const {
    return id < otherCreature.id;
}

/**
 * @brief Returns the unique identifier (id) of the creature.
 *
 * @return The ID of the creature.
 */
int WildLifeSimulation::Creature::getId() const {
    return id;
}

/**
 * @brief Returns the x-coordinate of the current location of the creature.
 *
 * @return The current x-coordinate of the creature's location.
 */
double WildLifeSimulation::Creature::getX() const {
    return x;
}

/**
 * @brief Returns the y-coordinate of the current location of the creature.
 *
 * @return The current y-coordinate of the creature's location.
 */
double WildLifeSimulation::Creature::getY() const {
    return y;
}

/**
 * @brief Checks if the creature is alive.
 *
 * @return True if the health of the creature is above zero, false otherwise.
 */
bool WildLifeSimulation::Creature::isAlive() const {
    return health > 0;
}

/**
 * @brief Calculates the speed of the creature based on its health.
 *
 * @return The speed of the creature.
 */
double WildLifeSimulation::Creature::getSpeed() const {
    if (health > 10) {
        return 10 / (double) health;
    }
    return 1;
}

/**
 * @brief Increases the creature's health as the quality of the consumed food.
 *
 * @param qualityOfTheFood The quality of the food consumed by the creature.
 */
void WildLifeSimulation::Creature::eatFood(const int qualityOfTheFood) {
    health += qualityOfTheFood;
}

/**
 * @brief Decreases the creature's health by the provided amount.
 *
 * @param amountOfReduction The provided reduction amount for the health of the creature.
 */
void WildLifeSimulation::Creature::decreaseHealth(const int amountOfReduction) {
    health -= amountOfReduction;
}

/**
 * @brief Simulates a fight between two creatures in the simulation.
 *
 * @param otherCreature The other creature to fight against.
 */
void WildLifeSimulation::Creature::fightAgainst(WildLifeSimulation::Creature &otherCreature) {
    if (health <= 0 || otherCreature.health <= 0 ||
            calculateTheDistanceBetweenTwoPoints(x, otherCreature.x, y, otherCreature.y) >= 2) {
        return;
    }

    if (health >= otherCreature.health) {
        otherCreature.health = 0;
        return;
    }

    health = 0;
}

/**
 * @brief Moves the creature towards the specified coordinates.
 *
 * @param destinationX The x-coordinate of the destination point.
 * @param destinationY The y-coordinate of the destination point.
 */
void WildLifeSimulation::Creature::moveTowards(const double destinationX, const double destinationY) {
    const double distanceX = (destinationX - x), distanceY = (destinationY - y),
            vectorMagnitude = std::sqrt(distanceX * distanceX + distanceY * distanceY);

    x += getSpeed() * (double) (distanceX / vectorMagnitude);
    y += getSpeed() * (double) (distanceY / vectorMagnitude);
}

/**
 * @brief Checks if all the creatures in the simulation are dead.
 *
 * @return True if all the creatures are dead, false otherwise.
 */
bool WildLifeSimulation::areAllCreaturesDead() const {
    for (int i = 0; i < creaturesInTheSimulation.size(); i++) {
        if (creaturesInTheSimulation[i].isAlive()) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Prints the positions of all the creatures in the simulation onto the console even if they are already dead.
 */
void WildLifeSimulation::printOutAllCreatures() const {
    for (int i = 0; i < creaturesInTheSimulation.size(); i++) {
        const Creature *currentCreature = &creaturesInTheSimulation[i];
        printf("Creature %d: %.2f, %.2f\n", currentCreature->getId(), currentCreature->getX(), currentCreature->getY());
        currentCreature = NULL;
    }
}

/**
 * @brief Adds new food resources into the simulation at a specific time tick if any available.
 */
void WildLifeSimulation::addNewFoodResourcesIfAny() {
    while (!allFetchedFoodResources.empty()) {

        Food topFood = allFetchedFoodResources.top();

        if (topFood.getSpawnTime() != timeAsTicks) {
            break;
        }

        topFood.resetSpawnTime();
        currentlyAvailableFoods.push(topFood);
        allFetchedFoodResources.pop();
    }
}

/**
 * @brief Perpetuates the fights between all pairs of the creatures in the simulation.
 */
void WildLifeSimulation::runFightsInBetweenCreatures() {
    for (int i = 0; i < creaturesInTheSimulation.size(); i++) {
        for (int j = i + 1; j < creaturesInTheSimulation.size(); j++) {
            creaturesInTheSimulation[i].fightAgainst(creaturesInTheSimulation[j]);
        }
    }
}

/**
 * @brief Makes the first creature that reaches the best food consume it, then sets the new best food.
 */
void WildLifeSimulation::makeCreaturesConsumeNearbyFood() {
    if (!bestFood) {
        if (currentlyAvailableFoods.empty()) {
            return;
        }
        bestFood = &currentlyAvailableFoods.top();
    }

    for (int i = 0; i < creaturesInTheSimulation.size(); i++) {
        Creature *currentCreature = &creaturesInTheSimulation[i];

        if (!currentCreature->isAlive()) {
            currentCreature = NULL;
            continue;
        }

        if (calculateTheDistanceBetweenTwoPoints(currentCreature->getX(), bestFood->getX(),
                                                 currentCreature->getY(), bestFood->getY()) < 1) {

            currentCreature->eatFood(bestFood->getQuality());
            currentlyAvailableFoods.pop();

            if (currentlyAvailableFoods.empty()) {
                bestFood = NULL;
                return;
            }

            bestFood = &currentlyAvailableFoods.top();
        }

        currentCreature = NULL;
    }
}

/**
 * @brief Moves all the creatures in the simulation towards the best food available if they are alive.
 */
void WildLifeSimulation::moveCreaturesTowardsTheBestFood() {
    if (!bestFood) {
        return;
    }

    for (int i = 0; i < creaturesInTheSimulation.size(); i++) {
        Creature *currentCreature = &creaturesInTheSimulation[i];
        if (currentCreature->isAlive()) {
            currentCreature->moveTowards(bestFood->getX(), bestFood->getY());
        }
        currentCreature = NULL;
    }
}

/**
 * @brief Decreases the healths of all the creatures in the simulation by one if they are not already dead.
 */
void WildLifeSimulation::decreaseHealthOfAllCreaturesByOne() {
    for (int i = 0; i < creaturesInTheSimulation.size(); i++) {
        Creature *currentCreature = &creaturesInTheSimulation[i];
        if (currentCreature->isAlive()) {
            currentCreature->decreaseHealth(1);
        }
        currentCreature = NULL;
    }
}
