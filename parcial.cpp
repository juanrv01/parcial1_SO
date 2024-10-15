
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Proceso {
public:
    // atributos con entradas del archivo de texto
    string label;
    int burstTime,arrivalTime,queue,priority; 
    // atributos con valores inicializados
	int waitingTime, completionTime,responseTime,turnAroundTime,burstDone,no_rrblock;
	bool finish,rr1finish,rr2finish;

    Proceso(const string& lbl, int bt, int at, int q, int pr)
        : label(lbl), burstTime(bt), arrivalTime(at), queue(q), priority(pr),
        waitingTime(0),completionTime(0),responseTime(0),turnAroundTime(0),burstDone(0)
        ,no_rrblock(0),finish(false),rr1finish(false),rr2finish(false) {}

    void add_burstDone() {
    	burstDone++;
    }

    void add_waitingTime() {
    	waitingTime++;
    }

    void set_responseTime(int a) {
    	responseTime= a;
    }

    void set_completionTime(int a) {
    	completionTime= a;
    }
 
    void set_turnAroundTime() {
    	turnAroundTime = completionTime - arrivalTime;
    }

    void set_finish () {
    	finish = true;
    }

};

void sort_Queue(vector<Proceso>& procesos) {
    std::sort(procesos.begin(), procesos.end(), [](const Proceso& a, const Proceso& b) {
        return a.priority > b.priority; // Ordenar de mayor a menor
    });
}

vector<Proceso> leerProcesosDesdeArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    string linea;
    vector<Proceso> procesos;

    if (!archivo) {
        cerr << "Error al abrir el archivo." << endl;
        return procesos; // Retornar un vector vacío en caso de error
    }

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string label;
        string burstTimeStr, arrivalTimeStr, queueStr, priorityStr;

        // Leer cada parte de la línea separada por ';'
        getline(ss, label, ';');
        getline(ss, burstTimeStr, ';');
        getline(ss, arrivalTimeStr, ';');
        getline(ss, queueStr, ';');
        getline(ss, priorityStr, ';');

        // Convertir las partes numéricas de string a int
        int burstTime = stoi(burstTimeStr);
        int arrivalTime = stoi(arrivalTimeStr);
        int queue = stoi(queueStr);
        int priority = stoi(priorityStr);

        // Crear un objeto Proceso y agregarlo al vector
        procesos.emplace_back(label, burstTime, arrivalTime, queue, priority);
    }

    archivo.close();
    return procesos;
}

void roundrobin_1 (vector<Proceso*> queue, int quantum, int clock) {
    for (auto Proceso:queue) {
        Proceso->add_waitingTime();
        if (Proceso->finish==false && Proceso->rr1finish==false) {
            Proceso->add_burstDone();
            if (Proceso->burstDone==1) {
                Proceso->set_responseTime(clock-1);
            } 

            if (Proceso->burstDone == Procesol->burstTime){
                Proceso->set_completionTime(clock);
            } else if (Proceso->burstDone==quantum) {
                Proceso->rr1finish==true;
            }
            Proceso->waitingTime--;
       } 
    }
}

void roundrobin_2 (vector<Proceso*> queue, int quantum_1, int quantum_2,int clock) {
    for (auto Proceso:queue) {
        if (Proceso->finish==false && Proceso->rr2finish==false && Proceso->rr1finish == true) {
            procesoActual = Proceso;
            procesoActual->add_burstDone();
            if (procesoActual->burstDone==1) {
                procesoActual->set_responseTime(clock-1);
            } 

            if (procesoActual->burstDone == procesoActual->burstTime){
                procesoActual->set_completionTime(clock);
            } else if (procesoActual->burstDone==quantum) {
                procesoActual->rr1finish==true;
            }
            procesoActual->waitingTime--;
        } 
    }
}

void add_waiting_time_other_queues(vector<Proceso*> queue_1,vector<Proceso*> queue_2) {
    for (const auto& Proceso:queue_1)
    {
        if (Proceso->finish==false) {
            Proceso->add_waitingTime();
        }
    }
    for (const auto& Proceso:queue_2)
    {
        if (Proceso->finish==false) {
            Proceso->add_waitingTime();
        }
    }
}

int main(int argc, char* argv[]) { 
    vector<Proceso> procesos = leerProcesosDesdeArchivo(argv[1]);
    vector<Proceso*> queue_1,queue_2,queue_3;
    int finishTime,clock;

    for (const auto& Proceso : procesos) {
		finishTime += Proceso.burstTime; //tiempo finalizacion de los procesos
    }
    
    for (clock=0; clock < finishTime; clock++)
    {
        for (int i = 0; i < procesos.size();i++) {
            switch(procesos[i].queue)
            {
                case 1:
                    queue_1.emplace_back(&procesos[i]);
                    break;
                case 2:
                    queue_2.emplace_back(&procesos[i]);
                    break;
                case 3:
                    queue_3.emplace_back(&procesos[i]);
                    break;
            }   
        }
        
        if (clock>0)
        {
            if (queue_1.size() > 0) {
                roundrobin_1(queue_1,1,clock);
                roundrobin_2(queue_1,1,3,clock);
                add_waiting_time_other_queues(queue_2,queue_3);
            } else if (queue_2.size() > 0) {
                roundrobin_1(queue_2,1,clock);
                roundrobin_2(queue_2,1,3,clock);
                add_waiting_time_other_queues(queue_1,queue_3);
            } else if (queue_3.size() > 0) {
                roundrobin_1(queue_3,1,clock);
                roundrobin_2(queue_3,1,3,clock);
                add_waiting_time_other_queues(queue_1,queue_2);
            }
            
        }
    }

    for (const auto& Proceso : procesos)
    {
    	cout<< Proceso->label << "#" << Proceso->burstTime << "#" << Proceso->arrivalTime << "#" << Proceso->queue << "#" << Proceso->priority << "#" << 
    	Proceso->waitingTime << "#" << Proceso->completionTime << "#" << Proceso-> responseTime << "#" << Proceso->turnAroundTime <<endl;
    }
    return 0;
}