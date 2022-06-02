#ifndef NODE
#define NODE

#include<iostream>
#include<string>
#include<list>

using namespace std;

//Node class definition
class Node {
	private:
		string state;
		float pathCost;
		list<string> parent;
	public:
		Node(){
			state = "";
			pathCost = 0;
		}

		Node(string initialState, float initialCost){
			state = initialState;
			pathCost = initialCost;		}

		Node(string initialState, float initialCost, list<string> parentNode){
			state = initialState;
			pathCost = initialCost;
			parent = parentNode;
		}

		string getState() const{
			return state;
		}

		float getPathCost() const{
			return pathCost;
		}

		list<string> getParent(){
			return parent;
		}

		void display(){
			cout << state << endl;
		}

		bool operator==(const Node& node1){
			return (state.compare(node1.getState())==0);
		}

		friend bool operator<(const Node node1, const Node node2);
};

bool operator<(const Node node1, const Node node2){
	return !(node1.getState().compare(node2.getState())==0);
}


//Custom struct so priority queue orders nodes by their path cost (lowest)
struct lowestPathCost{
	bool operator()(const Node& node1, const Node& node2){
		return node1.getPathCost() > node2.getPathCost();;
	}
};

//Advance declarations of functions
Node aStar(string initial, string goal);
vector<string> readRides();
void readDataFromFile(list<list<string>> &possibleActions);
void displayList(list<tuple<string, float>> list);
bool frontierContains(Node node, priority_queue<Node, vector<Node>, lowestPathCost> frontier);
bool exploredContains(Node node, set<Node> explored);
priority_queue<Node, vector<Node>, lowestPathCost> frontierReplace(Node node, priority_queue<Node, vector<Node>, lowestPathCost> frontier);
float frontierFindPathCost(Node node, priority_queue<Node, vector<Node>, lowestPathCost> frontier);
void solution(Node* goal);
#endif