#include<iostream>
#include<bits/stdc++.h>
using namespace std;



vector<pair<char,int>> algorithms;  // Stores algorithm ID and quantum (if applicable)
vector<tuple<string,int,int>> processes;  // Process details: name, arrival time, service time
string operation;  // Type of operation: "trace" or "stats"
int process_count;  // Total number of processes
int last_instant;  // Last time instant for simulation
vector<vector<char>> timeline;  // Timeline representation for scheduling

// Statistics
vector<int> finishTime;  // Finish times of processes
vector<int> turnAroundTime;  // Turnaround times of processes
vector<float> normTurn;  // Normalized turnaround times
vector<int> waitingTime;  // Waiting times of processes


const string TRACE = "trace";
const string SHOW_STATISTICS = "stats";
const string ALGORITHMS[8] = {"","FCFS","RR-","SPN","SRT","HRRN","FB-1","FB-2i"};

void clear_timeline(){
    for(int i =0;i<last_instant;i++){
        for(int j =0;j<process_count;j++){
            timeline[i][j] = ' ';
        }
    }
}

string getProcessName(tuple<string,int,int>&a){
    return get<0>(a);
}

int getArrivalTime(tuple<string,int,int>&a){
    return get<1>(a);
}

int getServiceTime(tuple<string,int,int>&a){
    return get<2>(a);
}

// Comparator to sort processes by arrival time
bool sortbyArrivalTime(tuple<string,int,int>&a,tuple<string,int,int>&b){
    return getArrivalTime(a) < getArrivalTime(b) ;
}


// Fill timeline with wait time representation ('.')
void fillInWaitTime(){
    for(int i=0;i<process_count;i++){
        for(int j =getArrivalTime(processes[i]);j<finishTime[i];j++){
            if(timeline[j][i] != '*'){
                timeline[j][i] = '.';
            }
        }
    }
}

// Calculate waiting times based on turnaround time and service time
void calculateWaitingTime(){
    for(int i = 0; i < process_count; i++) {
        waitingTime[i] = turnAroundTime[i] - getServiceTime(processes[i]);
    }
}


// First Come First Serve (FCFS) Scheduling Algorithm
void firstComeFirstServe(){
    sort(processes.begin(),processes.end(),sortbyArrivalTime);
    int j =0;
    for(int time =0;time<last_instant;time++){
        if(j < process_count && getArrivalTime(processes[j]) <= time){
            finishTime[j] = (time + getServiceTime(processes[j]));
            turnAroundTime[j] = (finishTime[j] - getArrivalTime(processes[j]));
            normTurn[j] = (turnAroundTime[j] * 1.0 / getServiceTime(processes[j]));
            
            for(int k = getArrivalTime(processes[j]); k<time;k++){
                timeline[k][j] = '.';
            }
            for(int k = time;k<(time+getServiceTime(processes[j]));k++){
                timeline[k][j] = '*';
            }
            time += (getServiceTime(processes[j])-1);
            j++;
        }
    }
    calculateWaitingTime();
}

// Round Robin (RR) Scheduling Algorithm
void roundRobin(int quantum){
    queue<pair<int,int>>q;
    int j =0;
    
    while( j< process_count && getArrivalTime(processes[j]) == 0){
        q.push(make_pair(j,getServiceTime(processes[j])));
        j++;
    }
    int current_quantum = quantum;
    for(int time =0;time < last_instant;time++){
        while(j < process_count && getArrivalTime(processes[j]) == time+1){
            q.push(make_pair(j,getServiceTime(processes[j])));
            j++;
        }
        
        if(!q.empty()){
            int process_index = q.front().first;
            q.front().second -= 1;
            int remainingtime = q.front().second;
            current_quantum -= 1;

            int arrivaltime = getArrivalTime(processes[process_index]);
            int servicetime = getServiceTime(processes[process_index]);

            // Mark current process in timeline
            timeline[time][process_index] = '*';

            if(current_quantum == 0 && remainingtime == 0){
                // Process finishes exactly as quantum expires
                finishTime[process_index] = time+1;
                turnAroundTime[process_index] = (finishTime[process_index] - arrivaltime);
                normTurn[process_index] = (turnAroundTime[process_index] * 1.0/servicetime);
                q.pop();
                current_quantum = quantum;
            }
            else if (current_quantum == 0 && remainingtime != 0){
                // Quantum expires but process needs more time
                q.pop();
                q.push(make_pair(process_index,remainingtime));
                current_quantum = quantum;
            }
            else if(current_quantum != 0 && remainingtime == 0){
                // Process finishes before quantum expires
                finishTime[process_index] = time +1;
                turnAroundTime[process_index] = (finishTime[process_index] - arrivaltime);
                normTurn[process_index] = (turnAroundTime[process_index] * 1.0/servicetime);
                q.pop();
                current_quantum = quantum;
            }
        }
    }
    fillInWaitTime();
    calculateWaitingTime();
}

// Shortest Process Next (SPN) Scheduling Algorithm
void shortestProcessNext(){
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>pq;
    int j =0;
    for(int time =0;time<last_instant;time++){
        while(j < process_count && getArrivalTime(processes[j]) <= time){
            pq.push(make_pair(getServiceTime(processes[j]),j));
            j++;
        }
        if(!pq.empty()){
            int service_time = pq.top().first;
            int process_index = pq.top().second;
            pq.pop();

            int arrivaltime = getArrivalTime(processes[process_index]);

            finishTime[process_index] = (time + service_time);
            turnAroundTime[process_index] = (finishTime[process_index] - arrivaltime);
            normTurn[process_index] = (turnAroundTime[process_index] * 1.0/service_time);

            for(int i = arrivaltime;i<time;i++){
                timeline[i][process_index] = '.';
            }
            for(int i =time;i<finishTime[process_index];i++){
                timeline[i][process_index] = '*';
            }
            time += (service_time-1);
        }
    }
    calculateWaitingTime();
}

// Shortest Remaining Time (SRT) Scheduling Algorithm
void shortestRemainingTime(){
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>pq;
    int j =0;
    for(int time =0;time <last_instant;time++){
        while(j < process_count && getArrivalTime(processes[j]) == time){
            pq.push(make_pair(getServiceTime(processes[j]),j));
            j++;
        }
        if(!pq.empty()){
            int curr_time = pq.top().first;
            int process_index = pq.top().second;
            int remaining_time = curr_time -1;

            pq.pop();

            int arrivaltime = getArrivalTime(processes[process_index]);
            int servicetime = getServiceTime(processes[process_index]);

            timeline[time][process_index] = '*';

            if(remaining_time == 0){
                finishTime[process_index] = (time +1);
                turnAroundTime[process_index] = (finishTime[process_index] - arrivaltime);
                normTurn[process_index] = (turnAroundTime[process_index] * 1.0/servicetime);
            }
            else{
                pq.push(make_pair(remaining_time,process_index));
            }
        }
    }
    fillInWaitTime();
    calculateWaitingTime();
}

double calculate_response_ratio(int wait_time,int service_time){
    return 1.0 + (wait_time * 1.0/service_time);
}

// Highest Response Rato Next (HRRN) Scheduling Algorithm
void highestResponseRatioNext(){
    priority_queue<pair<double,int>,vector<pair<double,int>>>pq;
    int j =0;
    for(int time = 0 ;time <last_instant;time++){
        while(j < process_count && getArrivalTime(processes[j]) <= time){
            pq.push(make_pair(1.0,j));
            j++;
        }
        
        vector<pair<double,int>>temp;
        while(!pq.empty()){
            int process_index = pq.top().second;
            pq.pop();
            int wait_time = time - getArrivalTime(processes[process_index]);
            int service_time = getServiceTime(processes[process_index]);

            temp.push_back(make_pair(calculate_response_ratio(wait_time,service_time),process_index));
        }

        for(auto i : temp){
            pq.push(i);
        }

        if(!pq.empty()){
            int process_index = pq.top().second;
            int servicetime = getServiceTime(processes[process_index]);
            int arrivaltime = getArrivalTime(processes[process_index]);

            for(int i = time;i<(time + servicetime);i++){
                timeline[i][process_index] ='*';
            }

            for(int i = arrivaltime;i<time;i++){
                timeline[i][process_index] ='.';
            }
            pq.pop();
            finishTime[process_index] = (time + servicetime);
            turnAroundTime[process_index] = (finishTime[process_index] - arrivaltime);
            normTurn[process_index] = (turnAroundTime[process_index] * 1.0/servicetime);

            time += (servicetime -1);
        }
    }
    calculateWaitingTime();
}

// Feedback Queue with  Time Quantum-1 (FQ-1) Scheduling Algorithm
void feedbackQ1(){
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>pq;
    // pair of priority level and process index
    unordered_map<int,int>remainingServiceTime;
    // map from process index to the remaining service time
    int j =0;
    while(j < process_count && getArrivalTime(processes[j]) == 0){
        pq.push(make_pair(0,j));
        remainingServiceTime[j] = getServiceTime(processes[j]);
        j++;
    }
    for(int time =0;time<last_instant;time++){
        if(!pq.empty()){
            int priority_level = pq.top().first;
            int process_index = pq.top().second;
            int arrivalTime = getArrivalTime(processes[process_index]);
            int serviceTime = getServiceTime(processes[process_index]);
            pq.pop();

            while(j < process_count && getArrivalTime(processes[j]) == time+1){
                pq.push(make_pair(0,j));
                remainingServiceTime[j] = getServiceTime(processes[j]);
                j++;
            }

            remainingServiceTime[process_index]--;
            timeline[time][process_index] = '*';

            if(remainingServiceTime[process_index] == 0){
                finishTime[process_index] = (time +1);
                turnAroundTime[process_index] = (finishTime[process_index] - arrivalTime);
                normTurn[process_index] = (turnAroundTime[process_index] * 1.0/serviceTime);
            }
            else{
                if(pq.size() >= 1){
                    pq.push(make_pair(priority_level+1,process_index));
                }
                else{
                    pq.push(make_pair(priority_level,process_index));
                }
            }
        }
        while(j < process_count && getArrivalTime(processes[j]) == time+1){
            pq.push(make_pair(0,j));
            j++;
            remainingServiceTime[j] = getServiceTime(processes[j]);
        }
    }
    fillInWaitTime();
    calculateWaitingTime();
}

// Feedback Queue with varying Time Quantum (FQ-2i) Scheduling Algorithm
void feedbackQ2i(){
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>pq;
    unordered_map<int,int>remainingServiceTime;
    int j =0;
    while(j < process_count && getArrivalTime(processes[j]) == 0){
        pq.push(make_pair(0,j));
        remainingServiceTime[j] = getServiceTime(processes[j]);
        j++;
    }
    for(int time =0;time <last_instant;time++){
        if(!pq.empty()){
            int priority_level = pq.top().first;
            int process_index = pq.top().second;
            int arrivalTime = getArrivalTime(processes[process_index]);
            int serviceTime = getServiceTime(processes[process_index]);
            pq.pop();

            while(j < process_count && getArrivalTime(processes[j]) <= time+1){
                pq.push(make_pair(0,j));
                remainingServiceTime[j] = getServiceTime(processes[j]);
                j++;
            }
            int currentQuantum = pow(2,priority_level);
            
            while(currentQuantum > 0 && remainingServiceTime[process_index] > 0){
                timeline[time++][process_index] = '*';
                currentQuantum --;
                remainingServiceTime[process_index]--;
            }

            if(remainingServiceTime[process_index] == 0){
                finishTime[process_index] = time;
                turnAroundTime[process_index] = (finishTime[process_index] - arrivalTime);
                normTurn[process_index] = (turnAroundTime[process_index] * 1.0/serviceTime);
            }
            else{
                if(pq.size() >= 1){
                    pq.push(make_pair(priority_level+1,process_index));
                }
                else{
                    pq.push(make_pair(priority_level,process_index));
                }
            }
            time --;
        }
        while(j < process_count && getArrivalTime(processes[j]) <= time+1){
            pq.push(make_pair(0,j));
            remainingServiceTime[j] = getServiceTime(processes[j]);
            j++;
        }
    }
    fillInWaitTime();
    calculateWaitingTime();
}

// Execute scheduling algorithm based on ID
void execute_algorithm(char algorithm_id,int quantum,string operation){
    switch(algorithm_id){
        case '1':
            if(operation == TRACE){
                cout<<"FCFS    ";
            }
            firstComeFirstServe();
            break;
        case '2':
            if(operation == TRACE){
                cout<<"RR-"<<quantum<<"    ";
            }
            roundRobin(quantum);
            break;
        case '3':
            if(operation == TRACE){
                cout<<"SPN     ";
            }
            shortestProcessNext();
            break;
        case '4':
            if(operation == TRACE){
                cout<<"SRT     ";
            }
            shortestRemainingTime();
            break;
        case '5':
            if(operation == TRACE){
                cout<<"HRRN    ";
            }
            highestResponseRatioNext();
            break;
        case '6':
            if(operation == TRACE){
                cout<<"FB-1    ";
            }
            feedbackQ1();
            break;
        case '7':
            if(operation == TRACE){
                cout<<"FB-2i   ";
            }
            feedbackQ2i();
            break;
        default:
            break;
    }
}

void printTimeline(int algorithm_index){
    for(int i =0;i<=last_instant;i++){
        cout<< i %10 <<" ";
    }
    cout<<"\n";
    cout<<"----------------------------------------------------\n";
    for(int i =0;i<process_count;i++){
        cout<<setw(2)<<getProcessName(processes[i])<<"      |";
        for(int j =0;j<last_instant;j++){
            cout<<timeline[j][i]<<"|";
        }
        cout<<"\n";
    }
    cout<<"----------------------------------------------------\n";
}

void printAlgorithm(int algorithm_index){
    int algorithm_id = algorithms[algorithm_index].first -'0';
    if(algorithm_id == 2){
        cout<<ALGORITHMS[algorithm_id] << algorithms[algorithm_index].second <<endl;
    }
    else{
        cout<<ALGORITHMS[algorithm_id]<<endl;
    }
}

void printProcesses(){
    cout<<"Process    |";
    for(int i =0;i<process_count;i++){
        cout<<" "<<setw(2)<<getProcessName(processes[i])<<"  |";
    }
    cout<<"\n";
}

void printArrivalTime(){
    cout<<"Arrival    |";
    for(int i =0;i<process_count;i++){
        cout<<setw(3)<<getArrivalTime(processes[i])<<"  |";
    }
    cout<<"\n";
}
void printServiceTime(){
    cout<<"Service    |";
    for(int i =0;i<process_count;i++){
        cout<<setw(3)<<getServiceTime(processes[i])<<"  |";
    }
    cout<<"\n";
}

void printFinishTime(){
    cout<<"Finish     |";
    for(int i =0;i<process_count;i++){
        cout<<setw(3)<<finishTime[i]<<"  |";
    }
    cout<<" Mean|";
    cout<<"\n";
}

void printTurnAroundTime()
{
    cout << "Turnaround |";
    int sum = 0;
    for (int i = 0; i < process_count; i++)
    {
        printf("%3d  |",turnAroundTime[i]);
        sum += turnAroundTime[i];
    }
    if((1.0 * sum / turnAroundTime.size())>=10)
		printf("%2.2f|\n",(1.0 * sum / turnAroundTime.size()));
    else
	 	printf(" %2.2f|\n",(1.0 * sum / turnAroundTime.size()));
}

void printNormTurn()
{
    cout << "NormTurn   |";
    float sum = 0;
    for (int i = 0; i < process_count; i++)
    {
        if( normTurn[i]>=10 )
            printf("%2.2f|",normTurn[i]);
        else
            printf(" %2.2f|",normTurn[i]);
        sum += normTurn[i];
    }

    if( (1.0 * sum / normTurn.size()) >=10 )
        printf("%2.2f|\n",(1.0 * sum / normTurn.size()));
	else
        printf(" %2.2f|\n",(1.0 * sum / normTurn.size()));
}
void printWaitingTime()
{
    cout << "Waiting    |";
    float sum = 0;
    for(int i =0;i<process_count;i++){
        printf("%3d  |",waitingTime[i]);
        sum += waitingTime[i];
    }
    if( (1.0 * sum / waitingTime.size()) >=10 )
        printf("%2.2f|\n",(1.0 * sum / waitingTime.size()));
	else
        printf(" %2.2f|\n",(1.0 * sum / waitingTime.size()));
}

void printStats(int algorithm_index){
    printAlgorithm(algorithm_index);
    printProcesses();
    printArrivalTime();
    printServiceTime();
    printFinishTime();
    printTurnAroundTime();
    printNormTurn();
    printWaitingTime();
}


void parse_processes(){
    cin>>last_instant;
    cin>>process_count;
    string temp;
    for(int i =0;i<process_count;i++){
        string process_chunk;
        cin>>process_chunk;
        stringstream ss(process_chunk);
        getline(ss,temp,',');
        string process_name = temp;
        getline(ss,temp,',');
        int arrival_time = stoi(temp);
        getline(ss,temp,',');
        int service_time = stoi(temp);
        processes.push_back(make_tuple(process_name,arrival_time,service_time));
    }
}
void parse(){
    cin>>operation;
    string algorithm_chunk;
    cin>>algorithm_chunk;
    string temp;
    stringstream ss(algorithm_chunk);
    while(getline(ss,temp,',')){
        stringstream ss1(temp);
        string algo_part;

        getline(ss1,algo_part,'-');
        char algo_id = algo_part[0];

        int quantum =-1;
        if(getline(ss1,algo_part,'-')){
            if(!algo_part.empty()){
                quantum = stoi(algo_part);
            }
        }
        algorithms.push_back(make_pair(algo_id,quantum));
    }
    parse_processes();
}

int main(){
    parse();
    finishTime.resize(process_count);
    turnAroundTime.resize(process_count);
    normTurn.resize(process_count);
    waitingTime.resize(process_count);
    timeline.resize(last_instant);
    for(int i =0;i<last_instant;i++){
        for(int j =0;j<process_count;j++){
            timeline[i].push_back(' ');
        }
    }
    for(int i=0;i <(int)algorithms.size();i++){
        clear_timeline();
        execute_algorithm(algorithms[i].first,algorithms[i].second,operation);
        if(operation == TRACE){
            printTimeline(i);
        }
        else if (operation == SHOW_STATISTICS){
            printStats(i);
        }
        cout<< "\n";
    }
    return 0;
}
