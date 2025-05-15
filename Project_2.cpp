#include <iostream>//provide input and output functinality
#include <queue>//provides queue data structure
#include <vector>//helps us dtore multiple processes in a list
#include <algorithm> //helps with sorting and finging max/min values
#include <climits>//Defines limits for integers

using namespace std;

// Creating a process Blueprint  
struct Process {

    int id;//process number

    int arrivalTime;//when process arrivess

    int burstTime;//how long the process takes to complete

    int remainingTime; // For Round Robin scheduling(time left)

    int priority;//priority of process(sjf)

    int completionTime;//when process is finished

    int waitingTime;//time process had to wait before excetuing

    int turnaroundTime;//time between arrival and completion

    Process(int i, int at, int bt, int pr = 0)

        : id(i), arrivalTime(at), burstTime(bt), remainingTime(bt), priority(pr),

          completionTime(0), waitingTime(0), turnaroundTime(0) {}

};




// Comparator for SJF (Shortest Job First)

struct CompareBurst {

    bool operator()(Process* a, Process* b) {

        return a->burstTime > b->burstTime; // Min-heap

    }

};




// Scheduler Class

class Scheduler {

private://this section holds the information that only the scheduler can access

    vector<Process*> processList;//a lost that stores all processes

    queue<Process*> fcfsQueue;       // Queue for FCFS

    priority_queue<Process*, vector<Process*>, CompareBurst> sjfQueue; // a priority queue that follows SJF

    queue<Process*> rrQueue;         // Queue for Round Robin scheduling whwre each process gets a limited amount of time to run before switching to the next one

    int timeQuantum;


public://anyone can access these function

    Scheduler(int tq = 4) : timeQuantum(tq) {}

    void addProcess(int id, int arrivalTime, int burstTime, int priority = 0) {

        Process* p = new Process(id, arrivalTime, burstTime, priority);

        processList.push_back(p);

        fcfsQueue.push(p);

        sjfQueue.push(p);

        rrQueue.push(p);

    }

    void runFCFS()//the function does not return anything
     {
//initializing time
        int time = 0;//sets tim to zero at start
//displaying the header
        cout << "\n--- First Come First Serve (FCFS) Scheduling ---\n";
//looping hrough the processes
        while (!fcfsQueue.empty())//the loop runs until the queue is empty
         {

            Process* p = fcfsQueue.front();//gets the first processes in the queue

            fcfsQueue.pop();//removes the first process

            time = max(time, p->arrivalTime);//updating time //ensures that the time does not go back
//calculating completion time,turnaround and waiting time
            p->completionTime = time + p->burstTime;

            p->turnaroundTime = p->completionTime - p->arrivalTime;

            p->waitingTime = p->turnaroundTime - p->burstTime;
//printing process details
            cout << "Process " << p->id << " | Completion Time: " << p->completionTime

                 << " | Turnaround Time: " << p->turnaroundTime

                 << " | Waiting Time: " << p->waitingTime << endl;

            time += p->burstTime;
            
        }
    }

    void runSJF()
     {
//intializing timw
        int time = 0;

        cout << "\n--- Shortest Job First (SJF) Scheduling ---\n";

//loop thorough the process
        while (!sjfQueue.empty()) {

           //getting the shortest process
            Process* p = sjfQueue.top();

            sjfQueue.pop();

//update time
            time = max(time, p->arrivalTime);
//calculation of completion,turnaround and waiting time
            p->completionTime = time + p->burstTime;

            p->turnaroundTime = p->completionTime - p->arrivalTime;

            p->waitingTime = p->turnaroundTime - p->burstTime;

            cout << "Process " << p->id << " | Completion Time: " << p->completionTime

                 << " | Turnaround Time: " << p->turnaroundTime

                 << " | Waiting Time: " << p->waitingTime << endl;

            time += p->burstTime;
        }
    }

    void runRoundRobin() {

        int time = 0;

        cout << "\n--- Round Robin Scheduling (Quantum: " << timeQuantum << ") ---\n";
//loop through until all processes have finished exeution
        while (!rrQueue.empty()) {
//gets the first process in the queue 
            Process* p = rrQueue.front();

            rrQueue.pop();
//time update ,this ensure the scheduler does
            time = max(time, p->arrivalTime);
//checksif the process needs more time
            if (p->remainingTime > timeQuantum) {

                time += timeQuantum;

                p->remainingTime -= timeQuantum;

                rrQueue.push(p); // Push back for next round

            } else {

                time += p->remainingTime;

                p->completionTime = time;

                p->turnaroundTime = p->completionTime - p->arrivalTime;

                p->waitingTime = p->turnaroundTime - p->burstTime;

                cout << "Process " << p->id << " | Completion Time: " << p->completionTime

                     << " | Turnaround Time: " << p->turnaroundTime

                     << " | Waiting Time: " << p->waitingTime << endl;
            }
        }
    }

    void runComparativeAnalysis()//this function compares different cpu scheduling methods
     {

        cout << "\n--- Comparative Analysis ---\n";

        runFCFS();

        runSJF();

        runRoundRobin();

    }
};

int main() {

    Scheduler scheduler(3); // Set Time Quantum for Round Robin

    // Adding processes (id, arrivalTime, burstTime)

    scheduler.addProcess(1, 0, 5);

    scheduler.addProcess(2, 2, 3);

    scheduler.addProcess(3, 4, 8);

    scheduler.addProcess(4, 6, 6);


    scheduler.runComparativeAnalysis();
    
    return 0;
}
