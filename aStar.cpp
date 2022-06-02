#include <iostream>
#include <sstream>
#include <fstream>
#include <istream>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <queue>
#include <set>
#include <tuple>
#include "Node.h"
#include "Problem.h"
using namespace std;

int main(){

	//Will hold the index of the 3 rides the user wants to ride on
	//This will be used to get the permutations to calculae every path
	int choices[3];

	//Holds users current choice
	int choice;

	//Holds users starting ride index
	int start;

	//Has a list of every ride
	vector<string> rides;

	//Holds a list of every possible action path through the 3 rides from the start ride
	//Each action path cosists of a tuple of (list of strings to hold path) and (float to
	//store total path cost to get cheapest one overall)
	vector<tuple<list<string>, float>> allPaths; 

	//Reads in the rides from an external data file
	rides = readRides();

	//Displays a list of rides to the user
	for(int i=0; i<rides.size(); i++){
		cout << i+1 << " - " << rides[i] << endl;
	}

	//Prompts the user to select where they currently are
	cout << "Please elect which ride you are currently at: ";
	cin >> start;

	//Gets the 3 rides the user wants to go on
	for(int i=0; i<3; i++){
		cout << "Please select " << i + 1 << " ride: ";
		cin >> choice;
		choices[i] = choice-1;
	}

	//Holds total cost of the path for each permutation
	float cost;
	//Holds aciton path for each permutation
	list<string> path;
	//Stores goal node recieved from a* algorithm
	Node node;
	cout << endl << "____________________________________" << endl;

	//prepares array for permutations
	sort(choices, choices +3);

	//This loop will use a* to find the optimal path between all permutations of the 3 rides
	//it will store all the possible ride combinations and their optimal path and cost to be
	//searched later so that we can find the optimal path between them all
	do{
		path.clear();
		cost = 0;
		//Gets Goal node from start ride to 1st ride
		node = aStar(rides[start-1], rides[choices[0]]);
		//Gets the list of actions
		list<string> parent = node.getParent();
		int len = parent.size();
		for(int i=0; i<len; i++){
			//adds all actions to overall actions			
			path.push_back(parent.front());
			parent.pop_front();
		}
		//adds the cost to the overall cost
		cost += node.getPathCost();

		//gets the goal node from 1st ride to 2nd ride
		node = aStar(rides[choices[0]], rides[choices[1]]);
		parent = node.getParent();
		//we remove the starting node because the goal state from the last a* search will be the
		//initial state for this search and we dont want duplication of actions in our actions when 
		//we display it to the user
		parent.pop_front();
		len = parent.size();
		for(int i=0; i<len; i++){
			path.push_back(parent.front());
			parent.pop_front();
		}
		cost += node.getPathCost();

		//Gets the goal node from 2nd ride to 3rd ride
		node = aStar(rides[choices[1]], rides[choices[2]]);
		parent = node.getParent();
		parent.pop_front();
		len = parent.size();
		for(int i=0; i<len; i++){
			path.push_back(parent.front());
			parent.pop_front();
		}
		cost += node.getPathCost();

		//Adds this path of actions and its cost to a tuple to be added to the list of every path
		allPaths.push_back(make_tuple(path, cost));


		cout << cost << endl;

		for(auto a : path){
			cout << a << endl;
		}

		cout << "____________________________________" << endl;

		//Reorders the choice array to the next permutation
	}while(next_permutation(choices, choices + 3));

	//This is used to loop through vector of all possible paths to find the cheapest of them all
	int min = 0;
	int count = 0;
	for(auto p : allPaths){
		if(get<1>(p) < get<1>(allPaths[min])){
			min = count;
		}
		count++;
	}

	//Once we have found the cheapest path, display this to the user
	cout << "OPTIMAL PATH" << endl;
	cout << get<1>(allPaths[min]) << endl;
	for(auto a : get<0>(allPaths[min])){
		cout << a << endl;
	}


	return 0;
}

Node aStar(string initial, string goal){
	Problem problem(initial, goal);
	Node startNode(problem.getInitialState(), 0);
	priority_queue<Node, vector<Node>, lowestPathCost> frontier;
	frontier.push(startNode);
	set<Node> explored;

	//displayList(problem.action(startNode.getState()));

	int count = 0;

	while(true){

		priority_queue<Node, vector<Node>, lowestPathCost> copy;

		copy = frontier;

		//cout << count << endl;

		while(copy.size()!=0){
			//cout << copy.top().getState() << " - " << copy.top().getPathCost() << endl;
			copy.pop();
		}

		//if frontier is empty then we have explored all nodes and found no solutions
		if(frontier.empty()){
			Node blank("", 0);
			return blank;
		}

		Node node = frontier.top();
		frontier.pop();

		//if current node is a goal state, get solution and return the node
		if(problem.goalTest(node.getState())){
			list<string> newList = node.getParent();
			newList.push_back(node.getState());
			Node node(node.getState(), node.getPathCost(), newList);
			solution(&node);
			return node;
		}

		//add node to explored set
		explored.insert(node);

		for(auto a : problem.action(node.getState())){
			list<string> newList = node.getParent();
			newList.push_back(node.getState());
			//Remove node.getPathCost() to change to unifor cost search
			Node child(get<0>(a), get<1>(a) + node.getPathCost(), newList);
			if(!frontierContains(child, frontier)&&(!exploredContains(child, explored))){
				frontier.push(child);
			}
			else if(frontierContains(child, frontier)){
				if(child.getPathCost() < frontierFindPathCost(child, frontier)){
					frontier = frontierReplace(child, frontier);
				}
			}
		}		
		count++;	
	}
}

void solution(Node* goal){
	//cout << endl << "GOAL PATH" << endl;
	list<string> path = goal->getParent();
	for(auto n : path){
		//cout << n << endl;
	}
	//cout << goal->getState();
}

//Priortity queue doesnt allow me to iterate through it so I will make a custom "contains" function for my lists
bool frontierContains(Node node, priority_queue<Node, vector<Node>, lowestPathCost> frontier){
	Node next;
	while(frontier.size()!=0){
		next = frontier.top();
		frontier.pop();
		if(next == node){
			return true;
		}
	}
	return false;
}

float frontierFindPathCost(Node node, priority_queue<Node, vector<Node>, lowestPathCost> frontier){
	Node next;
	while(frontier.size()!=0){
		next = frontier.top();
		frontier.pop();
		if(next == node){
			return next.getPathCost();
		}
	}
	return 0;
}

priority_queue<Node, vector<Node>, lowestPathCost> frontierReplace(Node node, priority_queue<Node, vector<Node>, lowestPathCost> frontier){
	priority_queue<Node, vector<Node>, lowestPathCost> newFrontier;
	Node next;
	while(frontier.size()!=0){
		next = frontier.top();
		frontier.pop();

		if(next==node){
			newFrontier.push(node);
		}else{
			newFrontier.push(next);
		}
	}
	return newFrontier;
}

//Again, set contains no real useful, concise "contains" method so i will write it myself for readability
bool exploredContains(Node node, set<Node> explored){
	Node next;
	for(auto n : explored){
		if(n == node){
			return true;
		}
	}
	return false;
}

//Method to display list contents to the user
void displayList(list<tuple<string, float>> list){
	cout << endl;
	cout << "-----------------------------" << endl;

	int first = 0;			
	for (auto s : list){
		if(first==0){
			cout << "LOCATION:" << get<0>(s) << endl;
			first++;
			continue;
		}
		cout << get<0>(s) << " - " << get<1>(s) << endl;
	}
}

vector<string> readRides(){
	vector<string> rides;
	ifstream myfile;
	myfile.open("rides.txt");
	string line;

	while(getline(myfile, line)){
		rides.push_back(line);
	}

	return rides;
}