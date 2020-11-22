#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <bits/stdc++.h>

using namespace std;

void detectDeadlock(vector<std::vector<int>> &graph, vector<int> &num, vector<int> &wait, int init, int dest);

void displayGraph(vector<vector<int>> mat);

void replyToProcess(vector<std::vector<int>> & graph, vector<int> &num, vector<int> &wait, int init, int dest);

void printVector(vector<int> g1) {
    for (auto i=g1.begin(); i!=g1.end(); ++i) {
        cout << *i << " ";
    }
}

int no_processes;
bool deadlock_flag = 0;


int main() {
    int pid_probe;

    cout << "Enter the number of processes (minimum value greater than 1)" << endl;
    cin >> no_processes;

    if (no_processes > 1) {
        cout << "Enter the wait graph " << endl;

        vector<vector<int>> wait_graph(no_processes);

        for (int from=0; from<no_processes; from++) {
            for (int to=0; to<no_processes; to++) {
                int temp;
                cin >> temp;
                wait_graph.at(from).push_back(temp);
            }
        }

        cout << endl;
        cout << "The wait for graph is : " << endl << endl;

        displayGraph(wait_graph);
        cout << endl;

        // Initialising num and wait matrix of same size with zero as default values
        vector<int> num(no_processes);
        vector<int> wait(no_processes);

        cout << endl;
        
        cout << "Enter the processes initiating the probe (Between 1 and no. of processes)" << endl;
        cin >> pid_probe;
        cout << endl;
        
        pid_probe = pid_probe - 1;

        cout << "Initiating Probe....... " << endl << endl;
        cout << "DIRECTION" << "\t" << "PROBE" << endl;
        
        for (int col=0; col<no_processes; col++) {
            if(wait_graph.at(pid_probe).at(col) == 1) {
                cout << " S" << pid_probe+1 << " --> S" << (col+1) << " (" << (pid_probe+1) << "," << (pid_probe+1) << "," << (col+1) << ")" << endl;
                wait[pid_probe] = 1;
                num[pid_probe] = accumulate(wait_graph[pid_probe].begin(), wait_graph[pid_probe].end(), 0);            

                detectDeadlock(wait_graph, num, wait, pid_probe, col);
                       
            }
        }   
    
    } else {
        cout << "Deadlock detection is not possible. No process running in the system" << endl;
    }

    return 0;

}


void displayGraph(vector<vector<int>> wait_graph) {
    int n = wait_graph.at(0).size();
    int m = wait_graph.size();

    cout << "\t";
    for (int j=0; j<m; j++) {
        cout << "S" << (j+1) << "\t";
    }
    cout << endl;

    for (int i=0; i<m; i++) {
        cout << "S" << (i+1) << "\t";
        for (int j=0; j < n; j++) {
            cout << wait_graph.at(i).at(j) << "\t";
        }
        cout << "\n";
    }
}

void detectDeadlock(vector<std::vector<int>> &graph, vector<int> &num, vector<int> &wait, int init, int dest) {
    int end = no_processes;

    // Initiate a diffusion computation for blocked process Pi:
    if (wait[dest] == 0) {
        for (int col=0; col<end; col++) {
            if (graph[dest][col] == 1) {
                wait[dest] = 1;
                num[dest] = accumulate(graph[dest].begin(), graph[dest].end(), 0);            
                cout << "S" << (dest+1) << "--> S" << (col+1) << " (" << (init+1) << "," << (dest+1) << "," << (col+1) << ")" << endl;

                detectDeadlock(graph, num, wait, init, col);
            }
        }
    }
    
    // initiate reply
    if ((wait[dest] == 1) && (num[dest] > 0)) {
        replyToProcess(graph, num, wait, init, dest);
    } else {
        cout << "Deadlock detection is not possible. Process " << dest << " is not returning, closing the program." << endl;
        exit(0);
    }
}

void replyToProcess(vector<std::vector<int>> &graph, vector<int> &num, vector<int> &wait, int init, int dest) {
    int start = no_processes-1;
    for (int col=start; col>=0; col--) {
        if ((wait[dest] == 1) && (graph[col][dest] == 1)) {
            num[dest] = num[dest] - 1;
            if (num[dest] == 0) {
                if(init == col) {
                    cout << " R" << (dest+1) << "--> R" << (col+1) << " (" << (init+1) << "," << (dest+1) << "," << (col+1) << ")" << "---------------------> DEADLOCK DETECTED HERE" << endl;
                }
                else {
                    cout << "R" << (dest+1) << "-->R" << (col+1) << "(" << (init+1) << "," << (dest+1) << "," << (col+1) << ")" << endl;
                }
            }
        }
    }
}



