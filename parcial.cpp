//Librerias incluidas
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


//Definicion de la clase proceso
class Proceso {
public:
    string label;
    int burstTime,arrivalTime,queue,priority;
	int waitingTime=0;
	int completionTime=0;
	int responseTime=0;
	int turnAroundTime=0;
	int burstDone=0;

    Proceso(const string& lbl, int bt, int at, int q, int pr)
        : label(lbl), burstTime(bt), arrivalTime(at), queue(q), priority(pr) {}
};

//Definicion de mis funciones

//Funcion sort que organiza las queue de acuerdo a su prioridad
void sort_Queue(vector<Proceso>& procesos) {
    std::sort(procesos.begin(), procesos.end(), [](const Proceso& a, const Proceso& b) {
        return a.priority > b.priority; // Ordenar de mayor a menor
    });
}

//Round robin
void round_robin (vector<Proceso> queue, int quantum) {
	queue.front().burstDone ++;
	if (queue.front().burstDone==queue.front().burstTime) {
		queue.erase(queue.begin());
	} else if (queue.front().burstDone%quantum ==0 && queue.size()>1) {
		queue.push_back(queue.front());
		queue.erase(queue.begin());
	}
}

void algorithm_order(vector<Proceso> queue, int quantum_1, int quantum_2) {

	int sum_Quantum = quantum_1+quantum_2;
	if (queue.front().burstDone >= 0 && queue.front().burstDone <quantum_1) {
		round_robin(queue,quantum_1);
	} else if (queue.front().burstDone >= quantum_1 && queue.front().burstDone <sum_Quantum) {
		round_robin(queue,quantum_2);
	} else if (queue.front().burstDone >=sum_Quantum) {
		cout << "Se ejecuta el algoritmo 3" <<endl;
	}
}




//Funcion que recibe los datos de entrada
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


int main(int argc, char* argv[]) {
string nombreArchivo = argv[1]; 
    vector<Proceso> procesos = leerProcesosDesdeArchivo(nombreArchivo);
	vector<Proceso> queue_1, queue_2, queue_3;
	int finishTime=0;

    for (const auto& Proceso : procesos) {
		finishTime += Proceso.burstTime; //tiempo finalizacion de los procesos
    }

	for (int clock = 0; clock <= finishTime; clock++)
	{
		for (const auto& Proceso:procesos)
		{
			if (Proceso.arrivalTime == clock)
			{
				switch (Proceso.queue) {//asignacion a cola
					case 1:
						queue_1.push_back(Proceso); 
						break;
					case 2:
						queue_2.push_back(Proceso);
						break;
					case 3:
						queue_3.push_back(Proceso);
						break;
				}
			}
		}
		
		if (clock== 0)
		{
			sort_Queue(queue_1);
			sort_Queue(queue_2);
			sort_Queue(queue_3);
		}
		
		//Quiero ingresar este comentario	

		while (clock >=1)
		{
			if(queue_1.size()>0) {
				algorithm_order(queue_1,1,3);
			} else if (queue_2.size()>0) {
				algorithm_order(queue_2,3,5);
			} else if (queue_3.size()>0){
				algorithm_order(queue_3,2,3);
			}
		}
	}
	cout << finishTime << endl;
    	return 0;
}
	
