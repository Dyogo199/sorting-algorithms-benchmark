import java.util.*;

public class SortBenchmark {

    public static void main(String[] args) {
        int[] tamanhos = {100, 1000, 10000, 100000};
        int numExecucoes = 10;

        for (int n : tamanhos) {
            System.out.println("\n📌 Testando com vetor de tamanho " + n + "...\n");

            List<Double> temposBucket = new ArrayList<>();
            List<Double> temposMerge = new ArrayList<>();
            List<Double> temposBubble = new ArrayList<>();

            for (int i = 0; i < numExecucoes; i++) {
                double[] vetor = gerarVetorAleatorio(n);

                temposBucket.add(medirTempo(SortBenchmark::bucketSort, vetor.clone()));
                temposMerge.add(medirTempo(SortBenchmark::mergeSort, vetor.clone()));
                temposBubble.add(medirTempo(SortBenchmark::bubbleSort, vetor.clone()));
            }

            imprimirTabelaDetalhada(temposBucket, temposMerge, temposBubble);
            imprimirResumoEstatistico("Bucket Sort", temposBucket);
            imprimirResumoEstatistico("Merge Sort", temposMerge);
            imprimirResumoEstatistico("Bubble Sort", temposBubble);
            System.out.println("-------------------------------------------------------------------------------");
        }
    }

    public static double[] gerarVetorAleatorio(int n) {
        Random rand = new Random();
        double[] vetor = new double[n];
        for (int i = 0; i < n; i++) {
            vetor[i] = rand.nextDouble() * 1000;
        }
        return vetor;
    }

    public static double medirTempo(ConsumerOrdenacao ordenacao, double[] vetor) {
        long inicio = System.nanoTime();
        ordenacao.sort(vetor);
        long fim = System.nanoTime();
        return (fim - inicio) / 1_000_000.0;
    }

    public static void imprimirTabelaDetalhada(List<Double> bucket, List<Double> merge, List<Double> bubble) {
        System.out.println("🔎 Tabela 1 – Tempos individuais de execução (ms):");
        System.out.printf("%-15s %-20s %-20s %-20s%n", "Execução", "Bucket Sort", "Merge Sort", "Bubble Sort");
        for (int i = 0; i < bucket.size(); i++) {
            System.out.printf("%-15s %-20.2f %-20.2f %-20.2f%n", (i + 1) + "ª Execução", bucket.get(i), merge.get(i), bubble.get(i));
        }
    }

    public static void imprimirResumoEstatistico(String nome, List<Double> tempos) {
        double media = tempos.stream().mapToDouble(d -> d).average().orElse(0);
        double desvio = Math.sqrt(tempos.stream().mapToDouble(t -> Math.pow(t - media, 2)).sum() / tempos.size());
        double minimo = Collections.min(tempos);
        double maximo = Collections.max(tempos);

        System.out.printf("\n📊 %s – Estatísticas (ms):\n", nome);
        System.out.printf("Média: %.2f | Desvio Padrão: %.2f | Mínimo: %.2f | Máximo: %.2f%n", media, desvio, minimo, maximo);
    }

    interface ConsumerOrdenacao {
        void sort(double[] vetor);
    }

    public static void bubbleSort(double[] arr) {
        int n = arr.length;
        for (int i = 0; i < n - 1; i++) {
            boolean swapped = false;
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    double temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }

    public static void mergeSort(double[] arr) {
        if (arr.length < 2) return;
        int mid = arr.length / 2;
        double[] left = Arrays.copyOfRange(arr, 0, mid);
        double[] right = Arrays.copyOfRange(arr, mid, arr.length);
        mergeSort(left);
        mergeSort(right);
        merge(arr, left, right);
    }

    private static void merge(double[] arr, double[] left, double[] right) {
        int i = 0, j = 0, k = 0;
        while (i < left.length && j < right.length) {
            arr[k++] = (left[i] < right[j]) ? left[i++] : right[j++];
        }
        while (i < left.length) arr[k++] = left[i++];
        while (j < right.length) arr[k++] = right[j++];
    }

    public static void bucketSort(double[] arr) {
        if (arr.length == 0) return;
        int bucketCount = arr.length;
        List<List<Double>> buckets = new ArrayList<>(bucketCount);
        for (int i = 0; i < bucketCount; i++) buckets.add(new ArrayList<>());

        double min = Arrays.stream(arr).min().orElse(0);
        double max = Arrays.stream(arr).max().orElse(0);

        for (double num : arr) {
            int index = (int) ((num - min) / (max - min + 1) * bucketCount);
            buckets.get(index).add(num);
        }

        int idx = 0;
        for (List<Double> bucket : buckets) {
            Collections.sort(bucket);
            for (double num : bucket) {
                arr[idx++] = num;
            }
        }
    }
}
