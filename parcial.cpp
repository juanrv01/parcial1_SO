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
	int burstLeft=burstTime;

    Proceso(const string& lbl, int bt, int at, int q, int pr)
        : label(lbl), burstTime(bt), arrivalTime(at), queue(q), priority(pr) {}
};

//Definicion de mis funciones

//Funcion sort que organiza las queue de acuerdo a su prioridad
void sort_Queue(std::vector<Proceso>& procesos) {
    std::sort(procesos.begin(), procesos.end(), [](const Proceso& a, const Proceso& b) {
        return a.priority > b.priority; // Ordenar de mayor a menor
    });
}

//Round robin
void round_robin (vector<Proceso> queue, int quantum) {
	queue.front().completionTime ++;
	if (queue.front().completionTime==queue.front().burstTime) {
		queue.erase(queue.begin());
	} else if (queue.front().completionTime%quantum ==0 && queue.size()>1) {
		queue.push_back(queue.front());
		queue.erase(queue.begin());
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
		


		while (clock >=1)
		{
			if(queue_1.size()>0) {
				/*
				if (/* condition )
				{
					round_robin(queue_1,1);

				} else if (/* condition )
				{
					round_robin(queue_1,3);
				} else if (/* condition )
				{
					/* code 
				}
				*/
				
				
				
				

			} else if (queue_2.size()>0) {
				round_robin(queue_2,3);
				round_robin(queue_2,5);

			} else if (queue_3.size()>0){
				round_robin(queue_3,2);
				round_robin(queue_3,3);
			}
		}
		
		

	
		
	}
	
	//cout << finishTime << endl;
    return 0;
}
	
