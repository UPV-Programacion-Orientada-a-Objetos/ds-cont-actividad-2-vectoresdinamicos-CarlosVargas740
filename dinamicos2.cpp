#include <iostream>

void inicializar(int *&, float *&, char **&, int *&, char *&, int &);
void recibir(int *&, float *&, char **&, int *&, char *&, int &, int &, int &);
void procesar(char *, int &, int &, int &);
void despachar(int *, float *, char **, int *, char *, int &, int &, int);
void reporte(char *, int);
void liberar(int *&, float *&, char **&, int *&, char *&, int);
int longitudCadena(const char *);
void copiarCadena(char *, const char *);

int main()
{
    int *ids = nullptr;
    float *pesos = nullptr;
    char **destinos = nullptr;
    int *prioridades = nullptr;
    char *estados = nullptr;

    int capacidad = 50;
    int total = 0;
    int top_pila = -1;
    int frente_cola = -1;
    int final_cola = -1;

    inicializar(ids, pesos, destinos, prioridades, estados, capacidad);

    int op;
    do
    {
        std::cout << "\nMENU\n";
        std::cout << "1. Recibir paquete (va a Pila)\n";
        std::cout << "2. Procesar paquete (Pila -> Cola)\n";
        std::cout << "3. Despachar paquete (sale de Cola)\n";
        std::cout << "4. Ver reporte\n";
        std::cout << "5. Salir\n";
        std::cout << "Opcion: ";
        std::cin >> op;
        std::cin.ignore();

        switch (op)
        {
        case 1:
            recibir(ids, pesos, destinos, prioridades, estados,
                    capacidad, total, top_pila);
            break;
        case 2:
            procesar(estados, top_pila, frente_cola, final_cola);
            break;
        case 3:
            despachar(ids, pesos, destinos, prioridades, estados,
                      frente_cola, final_cola, total);
            break;
        case 4:
            reporte(estados, total);
            break;
        case 5:
            liberar(ids, pesos, destinos, prioridades, estados, capacidad);
            std::cout << "Chao!\n";
            break;
        }
    } while (op != 5);

    return 0;
}

int longitudCadena(const char *cadena)
{
    int lon = 0;
    while (cadena[lon] != '\0')
    {
        lon++;
    }
    return lon;
}

void copiarCadena(char *destino, const char *origen)
{
    int i = 0;
    while (origen[i] != '\0')
    {
        destino[i] = origen[i];
        i++;
    }
    destino[i] = '\0';
}

void inicializar(int *&ids, float *&pesos, char **&destinos,
                 int *&prioridades, char *&estados, int &capacidad)
{
    std::cout << "Iniciando con espacio para " << capacidad << " paquetes\n";

    ids = new int[capacidad];
    pesos = new float[capacidad];
    destinos = new char *[capacidad];
    prioridades = new int[capacidad];
    estados = new char[capacidad];

    for (int i = 0; i < capacidad; i++)
    {
        destinos[i] = nullptr;
    }
}

void recibir(int *&ids, float *&pesos, char **&destinos,
             int *&prioridades, char *&estados,
             int &capacidad, int &total, int &top_pila)
{
    if (total >= capacidad)
    {
        std::cout << "Duplicando espacio\n";
        int nuevo_cap = capacidad * 2;

        int *n_ids = new int[nuevo_cap];
        float *n_pesos = new float[nuevo_cap];
        char **n_destinos = new char *[nuevo_cap];
        int *n_prioridades = new int[nuevo_cap];
        char *n_estados = new char[nuevo_cap];

        for (int i = 0; i < total; i++)
        {
            n_ids[i] = ids[i];
            n_pesos[i] = pesos[i];
            n_destinos[i] = destinos[i];
            n_prioridades[i] = prioridades[i];
            n_estados[i] = estados[i];
        }
        for (int i = total; i < nuevo_cap; i++)
        {
            n_destinos[i] = nullptr;
        }

        delete[] ids;
        delete[] pesos;
        delete[] destinos;
        delete[] prioridades;
        delete[] estados;

        ids = n_ids;
        pesos = n_pesos;
        destinos = n_destinos;
        prioridades = n_prioridades;
        estados = n_estados;
        capacidad = nuevo_cap;
    }

    std::cout << "\nRecibir Paquete\n";
    std::cout << "ID: ";
    std::cin >> ids[total];
    std::cout << "Peso: ";
    std::cin >> pesos[total];
    std::cin.ignore();
    std::cout << "Destino: ";
    char buffer[100];
    std::cin.getline(buffer, 100);
    destinos[total] = new char[longitudCadena(buffer) + 1];
    copiarCadena(destinos[total], buffer);
    std::cout << "Prioridad (1/2/3): ";
    std::cin >> prioridades[total];

    estados[total] = 'R';
    top_pila = total;
    total++;

    std::cout << "Paquete " << ids[top_pila] << " recibido. Estado: 'R'. Pila.top_pila = " << top_pila << ".\n";
}

void procesar(char *estados, int &top_pila, int &frente_cola, int &final_cola)
{
    std::cout << "\nProcesar (Pila -> Cola)\n";

    if (top_pila == -1)
    {
        std::cout << "Pila vacia\n";
        return;
    }

    if (estados[top_pila] != 'R')
    {
        std::cout << "Error: top_pila no apunta a un paquete en estado 'R'\n";
        return;
    }

    int pos = top_pila;
    
    estados[pos] = 'E';
    
    if (frente_cola == -1)
    {
        frente_cola = pos;
        final_cola = pos;
    }
    else
    {
        final_cola = pos;
    }

    std::cout << "POP de Pila: Pkg transferido a Cola. Estado: 'E'. frente_cola=" 
              << frente_cola << ", final_cola=" << final_cola << ".\n";

    // Actualizar top_pila: buscar el siguiente 'R' desde el anterior
    top_pila = -1;
    for (int i = pos - 1; i >= 0; i--)
    {
        if (estados[i] == 'R')
        {
            top_pila = i;
            break;
        }
    }
}

void despachar(int *ids, float *pesos, char **destinos,
               int *prioridades, char *estados,
               int &frente_cola, int &final_cola, int total)
{
    std::cout << "\nDespachar\n";

    if (frente_cola == -1)
    {
        std::cout << "Cola vacia\n";
        return;
    }

    std::cout << "Analizando Cola...\n";
    
    int pos_prioritaria = -1;
    for (int i = 0; i < total; i++)
    {
        if (estados[i] == 'E' && prioridades[i] == 1)
        {
            pos_prioritaria = i;
            std::cout << "Detectada ALTA PRIORIDAD.\n";
            break;
        }
    }

    int pos_despachar;
    if (pos_prioritaria != -1)
    {
        pos_despachar = pos_prioritaria;
    }
    else
    {
        pos_despachar = frente_cola;
    }

    std::cout << "DESPACHADO: Paquete " << ids[pos_despachar]
              << " (Prioridad " << prioridades[pos_despachar] << "). Estado: 'D'.\n";
    estados[pos_despachar] = 'D';

    if (pos_despachar == frente_cola)
    {
        frente_cola = -1;
        for (int i = pos_despachar + 1; i < total; i++)
        {
            if (estados[i] == 'E')
            {
                frente_cola = i;
                break;
            }
        }
    }
    else
    {
        std::cout << "frente_cola se mantiene en " << frente_cola << " (esperando turno FIFO original).\n";
    }

    if (frente_cola == -1)
    {
        final_cola = -1;
    }
}

void reporte(char *estados, int total)
{
    std::cout << "\n Reporte \n";
    int r = 0, e = 0, d = 0;

    for (int i = 0; i < total; i++)
    {
        if (estados[i] == 'R')
            r++;
        else if (estados[i] == 'E')
            e++;
        else if (estados[i] == 'D')
            d++;
    }

    std::cout << "En Pila (R): " << r << "\n";
    std::cout << "En Cola (E): " << e << "\n";
    std::cout << "Despachados (D): " << d << "\n";
    std::cout << "Total: " << total << "\n";
}

void liberar(int *&ids, float *&pesos, char **&destinos,
             int *&prioridades, char *&estados, int capacidad)
{
    for (int i = 0; i < capacidad; i++)
    {
        if (destinos[i] != nullptr)
        {
            delete[] destinos[i];
        }
    }

    delete[] ids;
    delete[] pesos;
    delete[] destinos;
    delete[] prioridades;
    delete[] estados;

    std::cout << "Memoria liberada.\n";
}
