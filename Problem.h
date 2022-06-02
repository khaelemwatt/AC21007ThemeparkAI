#ifndef PROBLEM
#define PROBLEM

#include<iostream>
#include<string>
#include<list>
#include <tuple>

using namespace std;

//Problem class definition
class Problem {
	private:
		list<list<tuple<string,float>>> possibleActions;
		string initialState;
		string goalState;
	public:
		Problem(){
			initialState = "";
			goalState = "";
			readDataFromFile();
		}

		Problem(string initial, string goal){
			initialState = initial;
			goalState = goal;
			readDataFromFile();
		}

		string getInitialState(){
			//Returns the initial state of the problem
			return initialState;
		}

		bool goalTest(string state){
			if(state.compare(goalState)==0){
				return true;
			}
			return false;
		}

		list<tuple<string, float>> action(string state){
			//return a list of actions (and their cost) from the state provided
			list<tuple<string, float>> actions;
			for(auto list : possibleActions){
				if(get<0>(list.front()).compare(state)==0){
					list.pop_front();
					actions = list;
				}				
			}
			return actions;
		}

		string result(string state, string action){
			state = "";
			return action;
		}

		//Method to read in states and actions from text file
		void readDataFromFile(){
			ifstream myfile;
			myfile.open("actions.txt");
			string line;
			int first = 0;
			//For each line in the text file we want to read in the state and the following actions
			//Possible in that state
			while(getline(myfile, line)){
				//First line in text file is user info so skip it
				if(first==0){	
					first++;				
					continue;
				}

				istringstream ss(line);
				string substring;
				string cost;
				list<tuple<string, float>> stateActions;
				//We have the line, now split it again on tab to get individual elements
				while(getline(ss, substring, '\t')){
					getline(ss, cost, '\t');
					stateActions.push_back(make_tuple(substring, stof(cost)));
				}
				possibleActions.push_back(stateActions);
			}
			myfile.close();
		}
};
#endif