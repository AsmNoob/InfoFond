#ifndef SCHEDSPEC_HPP
#define SCHEDSPEC_HPP
#include <stdio.h>
#include <string.h>
#include <vector>
#include <minisat/core/Solver.h>
#include <minisat/core/SolverTypes.h>


class SchedSpec {

    std::vector<std::vector<int>*>* a;
    std::vector<std::vector<int>*>* b;
    std::vector<int>* c;
    std::vector<int>* d;
    std::vector<std::vector<int>*>* i; //intervales
    std::vector<std::vector<std::vector<Minisat::Var> > > possibilities;
    Minisat::vec<Minisat::Lit> lits;
    std::vector<int> cx; //comptage
    int k; //nombre de changements

    public:
    int T, S, E, P, X;
    SchedSpec(std::vector<std::vector<int>*>* data);

    ~SchedSpec();
    void addRules(Minisat::Solver &solver);
    void addDurationsRules(Minisat::Solver &solver);
    void addIntervalsRules(Minisat::Solver &solver);
    void addRoomChangesRules(Minisat::Solver &solver);
    void addGapRules(Minisat::Solver &solver);
    void solveAndShowSolutions(Minisat::Solver &solver);
    private :
    void generateDistributions(std::vector<std::vector<int>*> &durations, int duration);
    bool isForbiden(int t);
    void addOneChange(int k, int student, std::vector<std::vector<int> > &changes, Minisat::Solver &solver);

};

#endif
