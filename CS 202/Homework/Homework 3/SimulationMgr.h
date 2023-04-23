/**
 * Author: Deniz Tuna ONGUNER
 * ID: 22001788
 * Section: 2
 * Homework: 3
 * Description: Definitions of WildLifeSimulation, Food, and Creature objects.
 */

#ifndef SIMULATION_MGR_H
#define SIMULATION_MGR_H


#include <vector> // To store creature objects.
#include <iostream> // For strings and null pointers.
#include <algorithm> // To sort vectors: the vector of creatures.
#include "PriorityQueue.h"

class WildLifeSimulation {
public:
    WildLifeSimulation();

    void fetchDataFromFile(const std::string &fileName);

    void start();

protected:
    bool areAllCreaturesDead() const;

    void printOutAllCreatures() const;

    void addNewFoodResourcesIfAny();

    void runFightsInBetweenCreatures();

    void makeCreaturesConsumeNearbyFood();

    void moveCreaturesTowardsTheBestFood();

    void decreaseHealthOfAllCreaturesByOne();

private:
    int timeAsTicks;

    class Food {
    private:
        int id;
        double x;
        double y;
        int quality;
        int spawnTime;

    public:
        Food();

        Food(int id, double x, double y, int quality, int spawnTime);

        bool operator<(const Food &otherFood) const;

        double getX() const;

        double getY() const;

        int getQuality() const;

        int getSpawnTime() const;

        void resetSpawnTime();
    };

    Food *bestFood;

    class Creature {
    private:
        int id;
        double x;
        double y;
        int health;

    public:
        Creature(int id, double x, double y, int health);

        bool operator<(const Creature &otherCreature) const;

        int getId() const;

        double getX() const;

        double getY() const;

        bool isAlive() const;

        double getSpeed() const;

        void eatFood(int qualityOfTheFood);

        void decreaseHealth(int amountOfReduction);

        void fightAgainst(Creature &otherCreature);

        void moveTowards(double destinationX, double destinationY);
    };

    PriorityQueue<Food, 30> allFetchedFoodResources;
    PriorityQueue<Food, 30> currentlyAvailableFoods;
    std::vector<Creature> creaturesInTheSimulation;
};


#endif // SIMULATION_MGR_H
