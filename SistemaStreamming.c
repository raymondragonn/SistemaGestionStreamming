#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  
#define N 100

// Lista enlazada simple - Catálogo / Historial
struct elemento
{
    int id;
    char titulo[100];      // Título de la película
    int calif;             // 1-5 estrellas
    char genero[50];       // Género de la película
    int duracion;          // Duración en minutos
};
typedef struct nodo
{
    struct elemento pelicula;
    struct nodo *sgt;
} Nodo;

void pausa();
void menu(Nodo **Catalogo, Nodo **Historial);

void cargarArchivo(Nodo **Lista);

void merge(int left, int center, int right, Nodo **array);
void mergeSort(int left, int right, Nodo **array);
void ordenaPorGenero(Nodo **Catalogo);
Nodo** listaEnArreglo(Nodo* Lista, int* n);
void reconstruirListaDesdeArreglo(Nodo** Lista, Nodo** arreglo, int n);
void quickSortPorCalif(Nodo** arr, int low, int high);
void topPopulares(Nodo** Catalogo);
void ordenaPorCalif(Nodo **Catalogo);
void verUnaPelicula(Nodo **Catalogo, Nodo **Historial);
void eliminarPrimera(Nodo **Historial);
void imprimirPrimera(Nodo *Historial);
void primerPeliculaVista(Nodo **Catalogo, Nodo **Historial);
void imprimirLista(Nodo *Lista);
void verHistorial(Nodo **Historial);

Nodo *buscarNodo(Nodo **Lista, int id);
Nodo *crearNodo(int count);
void insertarNodo(Nodo **Lista, Nodo *peliculaSeleccionada);
int eliminarNodo(Nodo **Lista, int id);
void liberarLista(Nodo **Lista);

int count = 0;

int main(int argc, char const *argv[])
{
    Nodo *Catalogo = NULL;
    Nodo *Historial = NULL;

    // Cargar el archivo y llenar la lista con nodos
    cargarArchivo(&Catalogo);

    menu(&Catalogo, &Historial);
    // Imprimir la lista de películas
    //imprimirLista(Catalogo);

    // Liberar la memoria utilizada
    liberarLista(&Catalogo);

    return 0;
}

void limpiarBuffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void pausa() {
    getchar();
}

void menu(Nodo **Catalogo, Nodo **Historial)
{
    char option;
    int salir = 0;

    while(!salir)
    {
        system("cls");

        puts("- - - - - - - - - - - - - - -");
        puts("SISTEMA DE GESTION DE STREAMING");
        puts("\n~ Exploracion de Peliculas ~");
        puts("\tA) Por genero");
        puts("\tB) Top 10 populares");
        puts("\tC) Por calificacion");
        puts("\tD) X peliculas en X tiempo");
        puts("\n~ Busqueda de Peliculas ~");
        puts("\tE) Por titulo o palabras clave");
        puts("\n~ Historial de Peliculas ~");
        puts("\tF) Ver una pelicula");
        puts("\tG) Primer pelicula vista");
        puts("\tH) Ver historial");
        puts("");
        puts("S) Salir");
        puts("Selecciona una opcion: ");
        
        option = getchar();
        limpiarBuffer();
        option = toupper(option);

        switch(option)
        {
            case 'A':
                ordenaPorGenero(Catalogo);
                break;
            case 'B':
                topPopulares(Catalogo);
                break;
            case 'C':
                ordenaPorCalif(Catalogo);
                break;
            case 'D':
                break;
            case 'E':
                break;
            case 'F':
                verUnaPelicula(Catalogo,Historial);
                break;
            case 'G':
                primerPeliculaVista(Catalogo, Historial);
                break;
            case 'H':
                verHistorial(Historial);
                break;
            case 'S':
                salir = 1;
                break;
            default:
                break;
        }
    }
}

// Función para crear un nuevo nodo
Nodo *crearNodo(int count)
{
    Nodo *nodo = (Nodo *)malloc(sizeof(Nodo));
    if (!nodo)
        return NULL;

    nodo->pelicula.id = count;
    nodo->sgt = NULL;
    return nodo;
}

// Insertar un nodo al final de la lista - Registro con complejidad O(1)
void insertarNodo(Nodo **Lista, Nodo *peliculaSeleccionada)
{

    Nodo *newNodo = (Nodo *)malloc(sizeof(Nodo));
    if (!newNodo)
        return;

    newNodo->pelicula = peliculaSeleccionada->pelicula;
    newNodo->sgt = NULL; 

    if (*Lista == NULL){
        *Lista = newNodo;
    }
    else{
        // recorremos la lista hasta el último nodo
        Nodo *temp = *Lista;
        while (temp->sgt != NULL){
            temp = temp->sgt;
        }
        temp->sgt = newNodo;
    }
}

// Buscar un nodo en la lista por ID
Nodo *buscarNodo(Nodo **Lista, int id)
{
    while (*Lista)
    {
        if ((*Lista)->pelicula.id == id)
            return *Lista;
        Lista = &((*Lista)->sgt);
    }
    return NULL;
}

// Eliminar un nodo de la lista por ID
int eliminarNodo(Nodo **Lista, int id)
{
    Nodo *elimino = buscarNodo(Lista, id);
    if (!elimino)
        return 0;

    // Necesitas encontrar el nodo anterior
    Nodo **prev = Lista;
    while (*prev != elimino) {
        prev = &((*prev)->sgt);
    }

    *prev = elimino->sgt;
    free(elimino);
    return 1;
}

// Liberar toda la memoria utilizada por la lista
void liberarLista(Nodo **Lista)
{
    if (!*Lista)
        return;
    while (*Lista)
    {
        Nodo *temp = *Lista;
        *Lista = (*Lista)->sgt;
        free(temp);
    }
}

// Función para cargar las películas desde el archivo y llenar la lista
void cargarArchivo(Nodo **Catalogo)
{
    const char *nombreArchivo = "Catalogo.txt";
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo %s.\n", nombreArchivo);
        return;
    }
    count++;
    char linea[200];
    // Saltar la primera línea con los encabezados
    fgets(linea, sizeof(linea), archivo);

    // Leer cada línea del archivo
    while (fgets(linea, sizeof(linea), archivo))
    {
        // Eliminar el salto de línea al final de la línea
        linea[strcspn(linea, "\n")] = '\0';

        Nodo *newNodo = crearNodo(count++);
        if (!newNodo)
            continue;
        
        // Leer cada campo (separado por '|' y luego analizar)
        char *token = strtok(linea, "|");
        strcpy(newNodo->pelicula.titulo, token);  // Título

        token = strtok(NULL, "|");
        newNodo->pelicula.calif = atoi(token);  // Calificación

        token = strtok(NULL, "|");
        strcpy(newNodo->pelicula.genero, token);  // Género

        token = strtok(NULL, "|");
        newNodo->pelicula.duracion = atoi(token);  // Duración

        // Insertar el nuevo nodo al final de la lista
        newNodo->sgt = *Catalogo;
        *Catalogo = newNodo;
    }

    fclose(archivo);
}

// Funcionalidades Inciso A - H
// Funciones para Ordenar por género - MergeSort (nlogn) (Inciso A)
void merge(int left, int center, int right, Nodo **array) {
    int n1 = center - left + 1;
    int n2 = right - center;

    Nodo *L[n1], *R[n2];

    // Llenar los arreglos temporales L y R
    for (int i = 0; i < n1; i++) 
        L[i] = array[left + i];
    for (int i = 0; i < n2; i++) 
        R[i] = array[center + 1 + i];

    int i = 0, j = 0, k = left;

    // Fusionar los arreglos L y R
    while (i < n1 && j < n2) {
        if (strcmp(L[i]->pelicula.genero, R[j]->pelicula.genero) <= 0) {
            array[k] = L[i];
            i++;
        } else {
            array[k] = R[j];
            j++;
        }
        k++;
    }

    // Copiar los elementos restantes de L[] si los hay
    while (i < n1) {
        array[k] = L[i];
        i++;
        k++;
    }

    // Copiar los elementos restantes de R[] si los hay
    while (j < n2) {
        array[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int left, int right, Nodo **array) {
    if (left < right) {
        int center = (left + right) / 2;

        mergeSort(left, center, array);  // Ordenar la primera mitad
        mergeSort(center + 1, right, array);  // Ordenar la segunda mitad

        merge(left, center, right, array);  // Fusionar las dos mitades
    }
}

void ordenaPorGenero(Nodo **Catalogo) {
    system("cls");
    if (!Catalogo || !(*Catalogo)) {
        puts("El catalogo esta vacio.\n");
        puts("\nIngrese cualquier tecla para MENU PRINCIPAL");
        pausa();
        limpiarBuffer();
        return;
    }

    int n = 0;
    Nodo** arreglo = listaEnArreglo(*Catalogo, &n);

    mergeSort(0, n - 1, arreglo);

    puts("- - - - - - - - - - - - - - -");
    puts("ORDEN POR GENERO");
    puts("Peliculas agrupadas por genero:");

    reconstruirListaDesdeArreglo(Catalogo, arreglo, n);

    char *generoActual = (*Catalogo)->pelicula.genero;
    printf("\n~%s~\n", generoActual);

    Nodo* temp = *Catalogo;
    while (temp != NULL) {
        if (strcmp(temp->pelicula.genero, generoActual) != 0) {
            generoActual = temp->pelicula.genero;
            printf("\n~%s~\n", generoActual);
        }
        printf("\nID: %i\n", temp->pelicula.id);
        printf("Titulo: %s\n", temp->pelicula.titulo);
        printf("Calificacion: %d\n", temp->pelicula.calif);
        printf("Genero: %s\n", temp->pelicula.genero);
        printf("Duracion: %d minutos\n", temp->pelicula.duracion);
        printf("----------------------------------------\n");
        temp = temp->sgt;
    }

    free(arreglo);

    puts("\nIngrese cualquier tecla para MENU PRINCIPAL");
    pausa();
    limpiarBuffer();
}

// Funciones para Top 10 populares - QuickSort Hoare O(nlogn) (Inciso B)
// Función para convertir una lista enlazada a un arreglo
Nodo** listaEnArreglo(Nodo* Lista, int* n) {
    // Contar nodos
    int tam = 0;
    Nodo* temp = Lista;
    while (temp) {
        tam++;
        temp = temp->sgt;
    }

    // Crear arreglo
    Nodo** arreglo = (Nodo**)malloc(tam * sizeof(Nodo*));
    temp = Lista;
    for (int i = 0; i < tam; i++) {
        arreglo[i] = temp;
        temp = temp->sgt;
    }

    *n = tam;
    return arreglo;
}

// Función para reconstruir la lista enlazada desde un arreglo
void reconstruirListaDesdeArreglo(Nodo** Lista, Nodo** arreglo, int n) {
    *Lista = arreglo[0];
    Nodo* temp = *Lista;
    for (int i = 1; i < n; i++) {
        temp->sgt = arreglo[i];
        temp = temp->sgt;
    }
    temp->sgt = NULL;
}

// Función corregida de QuickSort
void quickSortPorCalif(Nodo** arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high]->pelicula.calif;
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (arr[j]->pelicula.calif > pivot) { // Orden descendente
                i++;
                Nodo* temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        Nodo* temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        int pi = i + 1;

        quickSortPorCalif(arr, low, pi - 1);
        quickSortPorCalif(arr, pi + 1, high);
    }
}

// Función para mostrar el Top 10
void topPopulares(Nodo** Catalogo) {
    system("cls");
    if (!*Catalogo) {
        printf("El catalogo esta vacio.\n");
        return;
    }

    int n = 0;
    Nodo** arreglo = listaEnArreglo(*Catalogo, &n);

    quickSortPorCalif(arreglo, 0, n - 1);

    puts("- - - - - - - - - - - - - - -");
    puts("TOP 10 POPULARES");
    printf("Mejores 10 peliculas segun su calificacion:\n");
    for (int i = 0; i < n && i < 10; i++) {
        printf("\n\t\tPOSICION #%d\n", i+1);
        printf("ID: %d\n", arreglo[i]->pelicula.id);
        printf("Titulo: %s\n", arreglo[i]->pelicula.titulo);
        printf("Calificacion: %d\n", arreglo[i]->pelicula.calif);
        printf("Genero: %s\n", arreglo[i]->pelicula.genero);
        printf("Duracion: %d\n", arreglo[i]->pelicula.duracion);
        printf("----------------------------------------\n");
    }

    reconstruirListaDesdeArreglo(Catalogo, arreglo, n);
    free(arreglo);
    puts("\nIngrese cualquier tecla para MENU PRINCIPAL");
    pausa();
    limpiarBuffer();
}

// Funcion para ordenar películas por calificación - CountingSort O(n+k) (Inciso C)
void ordenaPorCalif(Nodo **Catalogo) {
    system("cls");
    puts("- - - - - - - - - - - - - - -");
    puts("ORDEN POR CALIFICACION");

    if (!Catalogo || !*Catalogo) {
        printf("No hay peliculas en el catalogo.\n");
        puts("\nIngrese cualquier tecla para MENU PRINCIPAL");
        pausa();
        limpiarBuffer();
        return;
    }

    int max_calificacion = 5; 
    int count[max_calificacion + 1]; 
    memset(count, 0, sizeof(count));

    // Primer recorrido: Contar las calificaciones
    Nodo *temp = *Catalogo;
    while (temp != NULL) {
        count[temp->pelicula.calif]++;  // Contamos las calificaciones
        temp = temp->sgt;
    }

    // Acumular los conteos
    for (int i = 1; i <= max_calificacion; i++) {
        count[i] += count[i - 1];
    }

    // Crear un arreglo auxiliar (ya que sabemos cuántos nodos vamos a tener)
    int total_nodos = count[max_calificacion];
    Nodo **output = (Nodo **)malloc(total_nodos * sizeof(Nodo *));
    if (!output) {
        printf("Error al asignar memoria.\n");
        puts("\nIngrese cualquier tecla para MENU PRINCIPAL");
        pausa();
        limpiarBuffer();
        return;
    }

    // Segundo recorrido: Llenar el arreglo auxiliar
    temp = *Catalogo;
    while (temp != NULL) {
        int calif = temp->pelicula.calif;
        output[count[calif] - 1] = temp;  // Colocamos el nodo en la posición correcta
        count[calif]--;
        temp = temp->sgt;
    }

    // Reconstruir la lista enlazada desde el arreglo
    *Catalogo = output[0];
    Nodo *current = *Catalogo;
    for (int i = 1; i < total_nodos; i++) {
        current->sgt = output[i];
        current = current->sgt;
    }
    current->sgt = NULL;
    free(output);

    printf("Peliculas ordenadas por calificacion:\n");
    temp = *Catalogo;
    while (temp != NULL) {
        printf("\nID: %i\n", temp->pelicula.id);
        printf("Titulo: %s\n", temp->pelicula.titulo);
        printf("Calificacion: %d\n", temp->pelicula.calif);
        printf("----------------------------------------\n");
        temp = temp->sgt;
    }
    puts("\nIngrese cualquier tecla para MENU PRINCIPAL");
    pausa();
    limpiarBuffer();
}

// Funcion para ver una pelicula del catálogo (Inciso F)
void verUnaPelicula(Nodo **Catalogo, Nodo **Historial){
    system("cls");
    int id;
    Nodo *nodoEncontrado = NULL;
    puts("- - - - - - - - - - - - - - -");
    puts("CATALOGO DE PELICULAS");
    imprimirLista(*Catalogo);
    while (nodoEncontrado == NULL) {
        puts("\nIngresa el ID de la pelicula a ver: ");
        scanf(" %i", &id);
        limpiarBuffer();
        nodoEncontrado = buscarNodo(Catalogo, id);  // Buscar nodo en el catálo
        if (nodoEncontrado) {
            printf("\nPelicula con ID #%i seleccionada.", id);
        } else {
            printf("\nPelicula con ID #%i no encontrada. Intenta de nuevo.", id);
        }
    }
    puts("\nIngrese cualquier tecla para VER PELICULA");
    pausa();
    limpiarBuffer();
    system("cls");
    insertarNodo(Historial, nodoEncontrado);
    puts("- - - - - - - - - - - - - - -");
    puts("Viendo pelicula...");
    printf("\nID: %i\n", nodoEncontrado->pelicula.id);
    printf("Titulo: %s\n", nodoEncontrado->pelicula.titulo);
    printf("Calificacion: %d\n", nodoEncontrado->pelicula.calif);
    printf("Genero: %s\n", nodoEncontrado->pelicula.genero);
    printf("Duracion: %d minutos\n", nodoEncontrado->pelicula.duracion);
    printf("----------------------------------------\n");
    puts("\nIngrese cualquier tecla para MENU PRINCIPAL");
    pausa();
    limpiarBuffer();
}

// Funciones para imprimir y eliminar primer pelicula vista (Inciso G)
void eliminarPrimera(Nodo **Historial)
{
    Nodo *temp = *Historial;
    *Historial = temp->sgt;

    free(temp);

    printf("Primer pelicula vista fue eliminada exitosamente.\n");
}

void imprimirPrimera(Nodo *Historial)
{
    printf("Primer pelicula:\n");
    printf("\nID: %i\n", Historial->pelicula.id);
    printf("Titulo: %s\n", Historial->pelicula.titulo);
    printf("Calificacion: %d\n", Historial->pelicula.calif);
    printf("Genero: %s\n", Historial->pelicula.genero);
    printf("Duracion: %d minutos\n", Historial->pelicula.duracion);
    printf("----------------------------------------\n");
}

void primerPeliculaVista(Nodo **Catalogo, Nodo **Historial){
    system("cls");
    if (!*Historial){
        puts("- - - - - - - - - - - - - - -");
        puts("PRIMERA PELICULA VISTA"); 
        printf("No has mirado ninguna pelicula aun!.\n");
        puts("\nIngrese cualquier tecla para MENU PRINCIPAL");
        pausa();
        limpiarBuffer();
        return;
    }
    puts("- - - - - - - - - - - - - - -");
    puts("PRIMERA PELICULA VISTA"); 
    imprimirPrimera(*Historial);
    puts("\nDesesas eliminar la primer pelicula vista?");
    puts("Presiona 'E' para eliminar, o cualquier otra tecla para volver al menu principal:");

    char opcion;
    opcion = getchar();
    limpiarBuffer();
    opcion = toupper(opcion);

    if (opcion == 'E'){
        eliminarPrimera(Historial);
        puts("\nPresiona cualquier tecla para volver al menu principal...");
    }
    else{
        return;
    }
}

// Función para imprimir el historial (Inciso H)
void imprimirLista(Nodo *Lista)
{
    if (!Lista)
        return;

    // Imprimir los detalles de cada película
    while (Lista)
    {
        printf("\nID: %i\n", Lista->pelicula.id);
        printf("Titulo: %s\n", Lista->pelicula.titulo);
        printf("Calificacion: %d\n", Lista->pelicula.calif);
        printf("Genero: %s\n", Lista->pelicula.genero);
        printf("Duracion: %d minutos\n", Lista->pelicula.duracion);
        printf("----------------------------------------\n");
        Lista = Lista->sgt;
    }
}

void verHistorial(Nodo **Historial){
    system("cls");
    if(!*Historial){
        puts("- - - - - - - - - - - - - - -");
        puts("HISTORIAL DE PELICULAS");
        puts("No has visto ninguna pelicula todavia!");
        puts("\nIngrese cualquier tecla para MENU PRINCIPAL");
        pausa();
        limpiarBuffer();
        return;
    }
    puts("- - - - - - - - - - - - - - -");
    puts("HISTORIAL DE PELICULAS");
    imprimirLista(*Historial);
    puts("\nIngrese cualquier tecla para MENU PRINCIPAL");
    pausa();
    limpiarBuffer();
}
