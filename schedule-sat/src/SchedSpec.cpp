#include "SchedSpec.hpp"
#include <iostream>
#include <algorithm> //find

using namespace Minisat;

SchedSpec::SchedSpec(std::vector<std::vector<int>*>* data){
	std::vector<std::vector<int>*>::iterator it = data->begin();
	// before we read T and S we make sure there are at least 2 elements in
	// the list
	if (data->size() < 2)
		printf("No values for T and S found.");
	// reading T
	if ((*it)->size() > 1)
		printf("T should be a single number.");
	this->T = (*it)->front();
	delete (*it);
	it++;
	// reading S
	if ((*it)->size() > 1)
		printf("S should be a single number.");
	this->S = (*it)->front();
	delete (*it);
	it++;
	// reading c
	if (data->size() - 2 < this->S)
		printf("Capacities and room count don't match.");
	this->c = new std::vector<int>();
	for (int i = 0; i < this->S; i++) {
		if ((*it)->size() > 1)
			printf("Each capacity should be a single number.");
		this->c->push_back((*it)->front());
		delete (*it);
		it++;
	}
	// before we read E,P,X we make sure there are at least 3 elements in
	// the list
	if (data->size() - (2 + this->c->size()) < 3)
		printf("No values for E, P and X found.");
	// reading E
	if ((*it)->size() > 1)
		printf("E should be a single number.");
	this->E = (*it)->front();
	delete (*it);
	it++;
	// reading P
	if ((*it)->size() > 1)
		printf("P should be a single number.");
	this->P = (*it)->front();
	delete (*it);
	it++;
	// reading X
	if ((*it)->size() > 1)
		printf("P should be a single number.");
	this->X = (*it)->front();
	delete (*it);
	it++;
	// reading a
	if (data->size() - (5 + this->c->size()) < this->E)
		printf("Student number and exams-per-student "
				"mapping don't match.");
	this->a = new std::vector<std::vector<int>*>();
	for (int i = 0; i < this->E; i++) {
		this->a->push_back((*it));
		it++;
	}
	// reading b
	if (data->size() - (5 + this->c->size() +
			this->a->size()) < this->P)
		printf("Professor number and exams-per-professor "
				"mapping don't match.");
	this->b = new std::vector<std::vector<int>*>();
	for (int i = 0; i < this->P; i++) {
		this->b->push_back((*it));
		it++;
	}
	// reading d
	if (data->size() - (5 + this->c->size() +
			this->a->size() +
			this->b->size()) > 0) {
		if (data->size() - (5 + this->c->size() +
				this->a->size() +
				this->b->size()) < this->X)
			printf("Exam durations and exam count don't match.");
		this->d = new std::vector<int>();
		for (int i = 0; i < this->X; i++) {
			if ((*it)->size() > 1)
				printf("Each duration should be a single number.");
			this->d->push_back((*it)->front());
			delete (*it);
			it++;
		}
	} else {
		this->d = NULL;
	}
	//reading I
	if((data->end() - it) > 1){
		this->i = new std::vector<std::vector<int>*>();
		while((data->end() - it) > 1){
			if((*it)->size() !=2){
				printf("Each gap should be composed of two numbers.");
			}
			this->i->push_back((*it));
			it++;
		}
	}else{this->i = NULL;}
	//reading k
	if((data->end() - it) == 1){
		if((*it)->size() > 1){
			printf("k value sould be a single number.");
		}
		this->k = (*it)->at(0);
		it++;
	}
	// final check, every index in a b should be valid
	for (int student = 0; student < this->E; student++)
		for (std::vector<int>::iterator it = (*this->a)[student]->begin();
				it != (*this->a)[student]->end();
				++it)
			if (*it > this->X)
				printf("Invalid exam mapped to a student.");
	for (int prof = 0; prof < this->P; prof++)
		for (std::vector<int>::iterator it = (*this->b)[prof]->begin();
				it != (*this->b)[prof]->end();
				++it)
			if (*it > this->X)
				printf("Invalid exam mapped to a professor.");
	// print information received
	/*
	printf("T = %d\n", this->T);
	printf("|S| = %d\n", this->S);
	int i = 0;
	for (std::vector<int>::iterator it = this->c->begin();
			it != this->c->end();
			++it) {
		printf("c(s%d) = %d\n", ++i, (*it));
	}
	printf("|E| = %d\n", this->E);
	printf("|P| = %d\n", this->P);
	printf("|X| = %d\n", this->X);
	i = 0;
	for (std::vector<std::vector<int>*>::iterator it = this->a->begin();
			it != this->a->end();
			++it) {
		printf("a(e%d) = { ", ++i);
		for (int j = 0; j < (*it)->size(); j++)
			printf("%d ", (**it)[j]);
		printf("}\n");
	}
	i = 0;
	for (std::vector<std::vector<int>*>::iterator it = this->b->begin();
			it != this->b->end();
			++it) {
		printf("b(p%d) = { ", ++i);
		for (int j = 0; j < (*it)->size(); j++)
			printf("%d ", (**it)[j]);
		printf("}\n");
	}
	if (this->d != NULL) {
		i = 0;
		for (std::vector<int>::iterator it = this->d->begin();
				it != this->d->end();
				++it) {
			printf("d(x%d) = %d\n", ++i, (*it));
		}
	}
	*/
	//init
	cx.assign(X,0);

	std::vector<std::vector<Minisat::Var> > exam;
	std::vector<Minisat::Var> room;
	Minisat::Var time;
	possibilities.assign(X, exam);
	for (int x = 0; x < X; ++x) {
		possibilities[x].assign(S, room);
		for (int s = 0; s < S; ++s) {
			possibilities[x][s].assign(T,time);
		}
	}

	//comptage du nb d'étudiants par examen :
	for (int e = 0; e < E; ++e){
		for (std::vector<int>::iterator it = a->at(e)->begin() ; it != a->at(e)->end(); ++it){
			cx[(*it) - 1] += 1; //matching index
		}
	}
}

SchedSpec::~SchedSpec(){
	if (this->a != NULL)
		delete a;
	if (this->b != NULL)
		delete b;
	if (this->c != NULL)
		delete c;
	if (this->d != NULL)
		delete d;
	if (this->i != NULL)
		delete i;
}

void SchedSpec::addRules(Minisat::Solver &solver){


	//declaration des variables
	for (int x = 0; x < X; ++x) {
		for (int s = 0; s < S; ++s) {
			for (int t = 0; t < T; ++t) {
				possibilities[x][s][t] = solver.newVar();
			}
		}
	}
	//chaque examen a bien lieu au moins une fois
	for (int x = 0; x < X; ++x) {
		lits.clear(); //une clause pour chaque examen
		for (int s = 0; s < S; ++s) {
			for (int t = 0; t < T; ++t) {
				lits.push(Minisat::mkLit(possibilities[x][s][t]));
			}
		}
		solver.addClause(lits);
	}
	//chaque examen a lieu une seule fois (pour une seule salle, une seule période)
	for (int x = 0; x < X; ++x) {
		for (int s = 0; s < S; ++s) {
			for (int t = 0; t < T; ++t) {
				for (int ss = 0; ss < S; ++ss) {
					for (int tt = 0; tt < T; ++tt) {
						if((t != tt) || (s != ss))
							//ne pas avoir aussi lieu dans une autre pièce et/ou une autre période
							solver.addClause(~Minisat::mkLit(possibilities[x][s][t]),~Minisat::mkLit(possibilities[x][ss][tt])) ;
					}
				}
			}
		}
	}

	//les examens ne peuvent pas se dérouler dans des pièces de capacité insuffisante
	//equivalent à : ne peut se dérouler que dans une des pièces de capacité suffisante
	for (int x = 0; x < X; ++x) {
		lits.clear(); //une clause pour chaque examen
		for (int s = 0; s < S; ++s) {
			if(cx[x] <= c->at(s)){
				for (int t = 0; t < T; ++t) {
					lits.push(Minisat::mkLit(possibilities[x][s][t]));
				}
			}
		}
		solver.addClause(lits);
	}

	//les examens d'un même étudiant ne peuvent pas avoir lieu en même temps
	int xx;
	for (int x = 0; x < X; ++x) {
		for (int t = 0; t < T; ++t) {
			for (int e = 0; e < E; ++e){
				if(std::find(a->at(e)->begin(), a->at(e)->end(), (x+1)) != a->at(e)->end()) { //matching index
					//x is one of the exams of e
					for (std::vector<int>::iterator it = a->at(e)->begin() ; it != a->at(e)->end(); ++it){
						xx = (*it) - 1; //matching index
						if(x != xx){//deux examens différents d'un même étudiant
							for (int s = 0; s < S; ++s) {
								for (int ss = 0; ss < S; ++ss) { //les pièces peuvent être différentes ou égales
									solver.addClause(~Minisat::mkLit(possibilities[x][s][t]),~Minisat::mkLit(possibilities[xx][ss][t])) ;
								}
							}

						}
					}
				}
			}

			//idem avec les profs
			for (int p = 0; p < P; ++p){
				if(std::find(b->at(p)->begin(), b->at(p)->end(), (x+1)) != b->at(p)->end()) { //matching index
					//x is one of the exams of p
					for (std::vector<int>::iterator it = b->at(p)->begin() ; it != b->at(p)->end(); ++it){
						xx = (*it) - 1; //matching index
						if(x != xx){//deux examens différents d'un même prof
							for (int s = 0; s < S; ++s) {
								for (int ss = 0; ss < S; ++ss) {
									//les pièces peuvent être égales, mais pas la période
									solver.addClause(~Minisat::mkLit(possibilities[x][s][t]),~Minisat::mkLit(possibilities[xx][ss][t])) ;
								}
							}

						}
					}
				}
			}

		}
	}
	//deux examens différents ne peuvent pas avoir la même salle en même temps
	for (int x = 0; x < X; ++x) {
		for (int s = 0; s < S; ++s) {
			for (int t = 0; t < T; ++t) {
				for(int xx = 0; xx < X; ++xx){
					if(x != xx)
						solver.addClause(~Minisat::mkLit(possibilities[x][s][t]),~Minisat::mkLit(possibilities[xx][s][t])) ;
				}
			}

		}
	}

}
void SchedSpec::addDurationsRules(Minisat::Solver &solver){

	//declaration des variables
	for (int x = 0; x < X; ++x) {
		for (int s = 0; s < S; ++s) {
			for (int t = 0; t < T; ++t) {
				possibilities[x][s][t] = solver.newVar();
			}
		}
	}

	int duration;
	//chaque examen a bien lieu au moins une fois
	for (int x = 0; x < X; ++x) {
		//std::cout << "Au moins une fois exam " << x << std::endl;
		lits.clear(); //une clause pour chaque examen
		duration =  d->at(x);
		for (int s = 0; s < S; ++s) {
			for (int t = 0; t < T; ++t) {
				lits.push(Minisat::mkLit(possibilities[x][s][t]));
			}
		}
		solver.addClause(lits);
	}

	//chaque examen dure un certain nombre de périodes consécutives
	std::vector<std::vector<int>*> distributions;

	for (int x = 0; x < X; ++x) {
		duration =  d->at(x);
		if(duration > 1){
			generateDistributions(distributions, duration);

			for (int t = 0; t < T; ++t) {

				for (int s = 0; s < S; ++s) { //on fixe la salle mais pas obligatoire ici
					//std::cout << "on change de case" << std::endl;
					for (std::vector<std::vector<int>*>::iterator it = distributions.begin() ; it != distributions.end(); ++it){
						lits.clear();
						lits.push(~Minisat::mkLit(possibilities[x][s][t]));
						for (std::vector<int>::iterator it2 = (*it)->begin() ; it2 != (*it)->end(); ++it2){
							if(((t + (*it2)) >= 0) && ((t + (*it2)) < T)){
								//optimisation possible : gérer les répétitions de clauses
								lits.push(Minisat::mkLit(possibilities[x][s][t + (*it2)]));
							}
						}
						solver.addClause(lits);
					}


				}

			}
		}

	}

	//chaque examen a lieu une seule fois (pour une seule salle, pas dans une autre période trop éloignée)
	for (int x = 0; x < X; ++x) {
		duration =  d->at(x);
		for (int s = 0; s < S; ++s) {
			for (int t = 0; t < T; ++t) {
				for (int ss = 0; ss < S; ++ss) {
					for (int tt = 0; tt < T; ++tt) {
						if((abs(t - tt) > (duration-1)) || (s != ss))
							//ne pas avoir aussi lieu dans une autre pièce et/ou dans une période trop éloignée
							solver.addClause(~Minisat::mkLit(possibilities[x][s][t]),~Minisat::mkLit(possibilities[x][ss][tt])) ;
					}
				}
			}
		}
	}

	//les examens ne peuvent pas se dérouler dans des pièces de capacité insuffisante
	//equivalent à : ne peut se dérouler que dans une des pièces de capacité suffisante
	//optimisation possible : fusion avec "se déroule au moins une fois"
	for (int x = 0; x < X; ++x) {
		lits.clear(); //une clause pour chaque examen
		for (int s = 0; s < S; ++s) {
			if(cx[x] <= c->at(s)){
				for (int t = 0; t < T; ++t) {
					lits.push(Minisat::mkLit(possibilities[x][s][t]));
				}
			}
		}
		solver.addClause(lits);
	}

	//les examens d'un même étudiant ne peuvent pas avoir lieu en même temps
	int xx;
	for (int x = 0; x < X; ++x) {
		//for (int s = 0; s < S; ++s) {
		for (int t = 0; t < T; ++t) {
			for (int e = 0; e < E; ++e){
				if(std::find(a->at(e)->begin(), a->at(e)->end(), (x+1)) != a->at(e)->end()) {//matching index
					//x is one of the exams of e
					for (std::vector<int>::iterator it = a->at(e)->begin() ; it != a->at(e)->end(); ++it){
						xx = (*it) - 1; //matching index
						if(x != xx){//deux examens différents d'un même étudiant
							for (int s = 0; s < S; ++s) {
								for (int ss = 0; ss < S; ++ss) { //les pièces peuvent être différentes ou égales
									solver.addClause(~Minisat::mkLit(possibilities[x][s][t]),~Minisat::mkLit(possibilities[xx][ss][t])) ;
								}
							}

						}
					}
				}
			}

			//idem avec les profs
			for (int p = 0; p < P; ++p){
				if(std::find(b->at(p)->begin(), b->at(p)->end(), (x+1)) != b->at(p)->end()) { //matching index
					//x is one of the exams of p
					for (std::vector<int>::iterator it = b->at(p)->begin() ; it != b->at(p)->end(); ++it){
						xx = (*it) - 1; //matching index
						if(x != xx){//deux examens différents d'un même prof
							for (int s = 0; s < S; ++s) {
								for (int ss = 0; ss < S; ++ss) {
									//les pièces peuvent être égales, mais pas la période
									solver.addClause(~Minisat::mkLit(possibilities[x][s][t]),~Minisat::mkLit(possibilities[xx][ss][t])) ;
								}
							}

						}
					}
				}
			}

		}
	}

	//deux examens différents ne peuvent pas avoir la même salle en même temps
	for (int x = 0; x < X; ++x) {
		for (int s = 0; s < S; ++s) {
			for (int t = 0; t < T; ++t) {
				for(int xx = 0; xx < X; ++xx){
					if(x != xx)
						solver.addClause(~Minisat::mkLit(possibilities[x][s][t]),~Minisat::mkLit(possibilities[xx][s][t])) ;
				}
			}

		}
	}
}
void SchedSpec::addIntervalsRules(Minisat::Solver &solver){

	addDurationsRules(solver);

	for (int t = 0; t < T; ++t) {
		if(isForbiden(t)){
			for (int x = 0; x < X; ++x) {
				for (int s = 0; s < S; ++s) {
					solver.addClause(~Minisat::mkLit(possibilities[x][s][t]));
				}
			}
		}
	}
}

void SchedSpec::addRoomChangesRules(Minisat::Solver &solver){
	addIntervalsRules(solver);
	//max k changements = si on a déjà k changements, on interdit tout autre changement
	//==>besoin de générer toutes les possibilités avec k changements
	//on encode récursivement toutes les possibilités de k+1 changements, soit k+2 éléments
	//dans une clause OR comprenant k+2 litéraux niés, on oblige le k+1 ème à être faux si
	//les k changements sont vrais

	//itérer sur les étudiants
	for (int e = 0; e < E; ++e){
		std::vector<std::vector<int> > changes;
		addOneChange(k+1, e, changes, solver);
	}

}
bool examAlreadyDone(int xx, std::vector<std::vector<int> > &changes){
	for (std::vector<std::vector<int> >::iterator it = changes.begin(); it != changes.end(); ++it){
		std::vector<int> change = *it;
		if(change[0] == xx){
			return true;
		}
	}
	return false;
}
void SchedSpec::addOneChange(int k, int student, std::vector<std::vector<int> > &changes, Minisat::Solver &solver){
	int lastExam = -1;
	int lastRoom = -1;
	int lastPeriod = -1;
	if(changes.size() != 0){
		std::vector<int> lastChange = changes[changes.size() - 1];
		lastExam = lastChange[0];
		lastRoom = lastChange[1];
		lastPeriod = lastChange[2];
	}


	for (int newPeriod = lastPeriod+1; newPeriod < T - k; ++newPeriod) {
		//toutes les périodes tant qu'il en reste au moins k :
		//encore k-1 changements à encoder après (soit k possibilités)
		//plus le k+1ème à interdire (le k+1ème est compté dans k lors du premier appel)
		for (int newRoom = 0; newRoom < S; ++newRoom){
			if(newRoom != lastRoom){
				//il y a changement si on est plus loin dans le temps et dans une salle différente de la précédente
				for (std::vector<int>::iterator it = a->at(student)->begin() ; it != a->at(student)->end(); ++it){
					int newExamen = (*it) - 1; //matching index
					if(not examAlreadyDone(newExamen, changes)){
						std::vector<int> newChange;
						newChange.push_back(newExamen);
						newChange.push_back(newRoom);
						newChange.push_back(newPeriod);
						changes.push_back(newChange); //add to stack
						if(k == 0){
							lits.clear();
							for (std::vector<std::vector<int> >::iterator it = changes.begin(); it != changes.end(); ++it){
								std::vector<int> change = *it;
								lits.push(~Minisat::mkLit(possibilities[change[0]][change[1]][change[2]]));
							}
							solver.addClause(lits);
						}else{
							addOneChange(k-1, student, changes, solver);

						}
						changes.pop_back();//pop from stack
					}
				}

			}
		}

	}


}
void SchedSpec::addGapRules(Minisat::Solver &solver){
	addRoomChangesRules(solver);
	//pas deux examens consécutifs si pas la même salle :
	//pour tout examen, on ne peut pas avoir un autre examen d'un même étudiant
	//dans une autre salle à 1 seule période d'écart
	//similaire à la régle : deux examens d'un même étudiant ne peuvent avoir lieu en même temps
	int xx;
	for (int x = 0; x < X; ++x) {
		//for (int s = 0; s < S; ++s) {
		for (int t = 0; t < T; ++t) {
			for (int e = 0; e < E; ++e){
				if(std::find(a->at(e)->begin(), a->at(e)->end(), (x+1)) != a->at(e)->end()) {//matching index
					//x is one of the exams of e
					for (std::vector<int>::iterator it = a->at(e)->begin() ; it != a->at(e)->end(); ++it){
						xx = (*it) - 1; //matching index
						if(x != xx){//deux examens différents d'un même étudiant
							for (int s = 0; s < S; ++s) {
								for (int ss = 0; ss < S; ++ss) {
									if(ss != s) { //seulement si les pièces sont différentes
										if (t < (T - 1)) solver.addClause(~Minisat::mkLit(possibilities[x][s][t]),~Minisat::mkLit(possibilities[xx][ss][t+1]));
										//if (t > 1) solver.addClause(~Minisat::mkLit(possibilities[x][s][t]),~Minisat::mkLit(possibilities[xx][ss][t-1]));
										//redondant
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void SchedSpec::generateDistributions(std::vector<std::vector<int>*> &durations, int duration){
	durations.clear();
	std::vector<int> increments;
	for (int increment = - (duration - 1); increment <= (duration - 1); increment++) {
		if(increment != 0){
			increments.push_back(increment);
		}
	}
	//ensembles de duration - 1 éléments, au nombre de (duration-1)^(duration)
	std::vector<int> elementsToAddInPosition;
	int elementToAddInSeq;
	int size = pow(duration-1, duration);
	for(int num = 0; num < size; num++){
		durations.push_back(new std::vector<int>);
	}
	for(int position = 0; position < duration; position++){
		elementsToAddInPosition.clear();
		for (std::vector<int>::iterator it = (increments.begin() + position) ; (it - (increments.begin() + position)) != (duration); ++it){
			elementsToAddInPosition.push_back(*it);
		}
		for(int num = 0; num < size; num++){
			elementToAddInSeq = elementsToAddInPosition[(num / ((int) (pow(duration - 1, duration - 1 - position)))) % (duration - 1)];
			durations[num]->push_back(elementToAddInSeq);
		}

	}

}

bool SchedSpec::isForbiden(int t){
	if(this->i != NULL){
		for (std::vector<std::vector<int>*>::iterator it = this->i->begin() ; it != this->i->end(); ++it){
			std::vector<int>* interval = *it;
			if((t >= (interval->at(0) - 1)) && (t <= (interval->at(1) - 1))) {
				//between lower and upperbound with index matching
				//on prend les bornes
				return true;
			}
		}
	}
	return false;
}

void SchedSpec::solveAndShowSolutions(Minisat::Solver &solver){
	// call the SAT solver
	solver.solve();

	if(solver.okay()){
		for (int x = 0; x < X; ++x) {
			int done = false;
			for (int s = 0; s < S; ++s) {
				for (int t = 0; t < T; ++t) {
					if (solver.modelValue(possibilities[x][s][t]) == l_True and not done) {
						//output requested :
						std::cout << s+1 << "," << t+1 << ";";
						done = true; //on ne sort que la période de début de l'examen
					}

				}
			}

		}
	}
	else std::cout << 0;

	std::cout << std::endl;
}

