#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define EXECUCOES 10

// ---------- Funções estatísticas ----------
double media(double *tempos) {
    double soma = 0;
    for (int i = 0; i < EXECUCOES; i++)
        soma += tempos[i];
    return soma / EXECUCOES;
}

double desvio_padrao(double *tempos, double media) {
    double soma = 0;
    for (int i = 0; i < EXECUCOES; i++)
        soma += pow(tempos[i] - media, 2);
    return sqrt(soma / EXECUCOES);
}

double minimo(double *tempos) {
    double min = tempos[0];
    for (int i = 1; i < EXECUCOES; i++)
        if (tempos[i] < min) min = tempos[i];
    return min;
}

double maximo(double *tempos) {
    double max = tempos[0];
    for (int i = 1; i < EXECUCOES; i++)
        if (tempos[i] > max) max = tempos[i];
    return max;
}

// ---------- Algoritmos de Ordenação ----------

void bubble_sort(double *arr, int n) {
    int i, j;
    int trocou;
    for (i = 0; i < n-1; i++) {
        trocou = 0;
        for (j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                double tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
}

void merge(double *arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    double *L = malloc(n1 * sizeof(double));
    double *R = malloc(n2 * sizeof(double));

    for (i = 0; i < n1; i++) L[i] = arr[l + i];
    for (j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    i = 0; j = 0; k = l;
    while (i < n1 && j < n2) {
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

void merge_sort(double *arr, int l, int r) {
    if (l < r) {
        int m = l + (r-l)/2;
        merge_sort(arr, l, m);
        merge_sort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

void bucket_sort(double *arr, int n) {
    if (n <= 0) return;
    int i, j, k;
    int bucket_count = n;
    double min = arr[0], max = arr[0];

    for (i = 1; i < n; i++) {
        if (arr[i] < min) min = arr[i];
        if (arr[i] > max) max = arr[i];
    }

    double range = (max - min + 1) / bucket_count;
    double **buckets = malloc(bucket_count * sizeof(double *));
    int *sizes = calloc(bucket_count, sizeof(int));

    for (i = 0; i < bucket_count; i++)
        buckets[i] = malloc(n * sizeof(double));

    for (i = 0; i < n; i++) {
        int index = (int)((arr[i] - min) / range);
        if (index >= bucket_count) index = bucket_count - 1;
        buckets[index][sizes[index]++] = arr[i];
    }

    k = 0;
    for (i = 0; i < bucket_count; i++) {
        bubble_sort(buckets[i], sizes[i]);  // Ordenar cada balde
        for (j = 0; j < sizes[i]; j++)
            arr[k++] = buckets[i][j];
        free(buckets[i]);
    }

    free(buckets);
    free(sizes);
}

// ---------- Utilitários ----------
double tempo_execucao(void (*sort)(double *, int), double *arr, int n) {
    clock_t start = clock();
    sort(arr, n);
    clock_t end = clock();
    return ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;
}

void copia_vetor(double *dest, double *src, int n) {
    for (int i = 0; i < n; i++)
        dest[i] = src[i];
}

// ---------- Execução ----------
void testar_algoritmos(int tamanho) {
    printf("\n📌 Testando com vetor de tamanho %d...\n", tamanho);

    double bucket[EXECUCOES], merge[EXECUCOES], bubble[EXECUCOES];

    double *original = malloc(tamanho * sizeof(double));
    double *vetor = malloc(tamanho * sizeof(double));
    srand(time(NULL));

    for (int i = 0; i < EXECUCOES; i++) {
        for (int j = 0; j < tamanho; j++)
            original[j] = ((double)rand() / RAND_MAX) * 1000.0;

        copia_vetor(vetor, original, tamanho);
        bucket[i] = tempo_execucao(bucket_sort, vetor, tamanho);

        copia_vetor(vetor, original, tamanho);
        merge[i] = tempo_execucao((void (*)(double *, int)) (^(double *a, int n){ merge_sort(a, 0, n - 1); }), vetor, tamanho);

        copia_vetor(vetor, original, tamanho);
        bubble[i] = tempo_execucao(bubble_sort, vetor, tamanho);
    }

    printf("\n🔎 Tabela 1 – Tempos individuais de execução (ms):\n");
    printf("%-12s %-15s %-15s %-15s\n", "Execução", "Bucket Sort", "Merge Sort", "Bubble Sort");
    for (int i = 0; i < EXECUCOES; i++) {
        printf("%-12d %-15.2f %-15.2f %-15.2f\n", i + 1, bucket[i], merge[i], bubble[i]);
    }

    printf("\n📊 Tabela 2 – Estatísticas dos tempos de execução (ms):\n");
    printf("%-15s %-10s %-10s %-10s %-10s\n", "Algoritmo", "Média", "Desvio", "Mínimo", "Máximo");

    double m;
    m = media(bucket);
    printf("%-15s %-10.2f %-10.2f %-10.2f %-10.2f\n", "Bucket Sort", m, desvio_padrao(bucket, m), minimo(bucket), maximo(bucket));
    m = media(merge);
    printf("%-15s %-10.2f %-10.2f %-10.2f %-10.2f\n", "Merge Sort", m, desvio_padrao(merge, m), minimo(merge), maximo(merge));
    m = media(bubble);
    printf("%-15s %-10.2f %-10.2f %-10.2f %-10.2f\n", "Bubble Sort", m, desvio_padrao(bubble, m), minimo(bubble), maximo(bubble));

    free(original);
    free(vetor);
}

int main() {
    int tamanhos[] = {100, 1000, 10000, 100000};
    int n_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);

    for (int i = 0; i < n_tamanhos; i++) {
        testar_algoritmos(tamanhos[i]);
    }

    return 0;
}
