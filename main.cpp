#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define BPLUS_ORDER 4
#define B_ORDER 4  // Добавлено определение для B-дерева

// Симулятор дисковых операций
struct DiskSimulator {
    int read_count;
    int write_count;
    int page_size;
};

void disk_init(struct DiskSimulator* disk) {
    disk->read_count = 0;
    disk->write_count = 0;
    disk->page_size = 4096;
}

void disk_read(struct DiskSimulator* disk, long long offset) {
    disk->read_count++;
}

void disk_write(struct DiskSimulator* disk, long long offset) {
    disk->write_count++;
}

// Структура для B-дерева (добавлено)
struct BTreeNode {
    int keys[B_ORDER - 1];
    long long children[B_ORDER];
    int key_count;
    bool is_leaf;
    long long offset;
};

struct BTreeNode* create_btree_node(bool is_leaf, long long offset) {
    struct BTreeNode* node = (struct BTreeNode*)malloc(sizeof(struct BTreeNode));
    node->key_count = 0;
    node->is_leaf = is_leaf;
    node->offset = offset;
    for (int i = 0; i < B_ORDER; i++) node->children[i] = -1;
    return node;
}

// Структура для B+-дерева
struct BPlusNode {
    int keys[BPLUS_ORDER - 1];
    union {
        long long children[BPLUS_ORDER]; // для внутренних узлов
        struct BPlusNode* next_leaf; // для листьев
        int data[BPLUS_ORDER - 1]; // данные в листьях
    };
    int key_count;
    bool is_leaf;
    long long offset;
};

struct BPlusNode* create_bplus_node(bool is_leaf, long long offset) {
    struct BPlusNode* node = (struct BPlusNode*)malloc(sizeof(struct BPlusNode));
    node->key_count = 0;
    node->is_leaf = is_leaf;
    node->offset = offset;
    if (is_leaf) node->next_leaf = NULL;
    return node;
}

// ==================== ТЕСТЫ ДЛЯ B+-ДЕРЕВА ====================

// ТЕСТ 1: Сравнение структуры B-дерева и B+-дерева
void test_structure_comparison() {
    printf("\n=== ТЕСТ 1: Сравнение структур B-дерева и B+-дерева ===\n\n");
    
    printf("B-дерево (ORDER=%d):\n", B_ORDER);
    printf("• Ключи хранятся во всех узлах\n");
    printf("• Каждый узел содержит и ключи, и указатели\n");
    printf("• Поиск может завершиться в любом узле\n");
    printf("• Нет связей между листьями\n\n");
    
    printf("B+-дерево (ORDER=%d):\n", BPLUS_ORDER);
    printf("• Ключи хранятся только в листьях\n");
    printf("• Внутренние узлы - только индекс\n");
    printf("• Все данные в последовательных листьях\n");
    printf("• Листья связаны через связный список\n");
    printf("• Поиск всегда идет до листа\n\n");
    
    printf("Преимущества B+-дерева:\n");
    printf("Более эффективные range queries\n");
    printf("Последовательный доступ к данным\n");
    printf("Меньше высота (только ключи в индексе)\n");
    printf("Оптимально для баз данных\n\n");
}

// ТЕСТ 2: Анализ эффективности использования памяти
void test_memory_efficiency() {
    printf("=== ТЕСТ 2: Эффективность использования памяти ===\n\n");
    
    int typical_keys = (BPLUS_ORDER - 1) * 2 / 3; // типичная заполненность 66%
    
    printf("B+-дерево ORDER=%d:\n", BPLUS_ORDER);
    printf("• Максимум ключей в узле: %d\n", BPLUS_ORDER - 1);
    printf("• Типичных ключей в листе: %d\n", typical_keys);
    printf("• Высота для 1M элементов: ~%.1f\n", log(1000000) / log(BPLUS_ORDER));
    printf("• Эффективность хранения: ~%.1f%%\n\n", (typical_keys * 100.0) / (BPLUS_ORDER - 1));
    
    printf("Сравнение с B-деревом:\n");
    printf("• B+-дерево: все данные в листьях + индекс отдельно\n");
    printf("• B-дерево: данные распределены по всем узлам\n");
    printf("• B+-дерево лучше для последовательного доступа\n");
    printf("• B-дерево может быть лучше для точечных запросов\n\n");
}

// ТЕСТ 3: Производительность при разных операциях
void test_operation_performance() {
    printf("=== ТЕСТ 3: Сравнение производительности операций ===\n\n");
    
    printf("Операция           | B-дерево | B+-дерево | Преимущество\n");
    printf("-------------------|----------|-----------|-------------\n");
    printf("Точечный поиск     |   O(h)   |   O(h)    |   равны\n");
    printf("Range query        |  O(h+k)  | O(h+k/c)  |   B+ лучше\n");
    printf("Вставка           |  O(h)    |   O(h)    |   равны\n");
    printf("Удаление          |  O(h)    |   O(h)    |   равны\n");
    printf("Последоват. доступ |  O(n)    |  O(n/c)   |   B+ лучше\n\n");
    
    printf("Объяснение:\n");
    printf("• h - высота дерева\n");
    printf("• k - количество элементов в диапазоне\n");
    printf("• c - коэффициент (B+-дерево читает листы блоками)\n");
    printf("• B+-дерево: range query = найти начало + пройти по связному списку\n");
    printf("• B-дерево: range query = обход всех узлов в диапазоне\n\n");
}

// ТЕСТ 4: Практические рекомендации для B+-дерева
void test_bplus_practical_recommendations() {
    printf("=== ТЕСТ 4: Практические рекомендации для B+-дерева ===\n\n");
    
    printf("Оптимальные сценарии использования B+-дерева:\n");
    printf("Реляционные базы данных (индексы)\n");
    printf("Файловые системы с большими файлами\n");
    printf("Системы аналитики (OLAP)\n");
    printf("Range queries и аналитические запросы\n");
    printf("Данные с последовательным доступом\n\n");
    
    printf("Когда выбрать B+-дерево вместо B-дерева:\n");
    printf("Частые range queries\n");
    printf("Много последовательных чтений\n");
    printf("Данные не помещаются в память\n");
    printf("Только точечные запросы\n");
    printf("анные полностью в памяти\n\n");
}

// ТЕСТ 5: Масштабируемость B+-дерева
void test_bplus_performance_scaling() {
    printf("=== ТЕСТ 5: Масштабируемость B+-дерева ===\n\n");
    
    int sizes[] = {1000, 10000, 100000, 1000000, 10000000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    
    printf("Сравнение высоты деревьев:\n");
    printf("%-10s | %-15s | %-15s | %-15s\n", 
           "Элементов", "B-дерево", "B+-дерево", "Выигрыш");
    printf("-----------|-----------------|-----------------|-----------------\n");
    
    for (int i = 0; i < num_sizes; i++) {
        int size = sizes[i];
        float btree_h = log(size) / log(B_ORDER);
        float bplus_h = log(size) / log(BPLUS_ORDER);
        float advantage = btree_h / bplus_h;
        
        printf("%-10d | %-15.1f | %-15.1f | %-15.1f\n", 
               size, btree_h, bplus_h, advantage);
    }
    printf("\n");
}

// Range query для B+-дерева
void bplus_range_query(struct BPlusNode* root, int start_key, int end_key) {
    printf("=== RANGE QUERY [%d - %d] ===\n", start_key, end_key);
    
    // Находим стартовый лист
    struct BPlusNode* current = root;
    while (!current->is_leaf) {
        int i = 0;
        while (i < current->key_count && start_key >= current->keys[i]) i++;
        current = (struct BPlusNode*)current->children[i];
    }
    
    // Проходим по связному списку листьев
    int results[100];
    int result_count = 0;
    while (current != NULL) {
        printf("Проверяем лист: [");
        for (int i = 0; i < current->key_count; i++) {
            printf("%d", current->keys[i]);
            if (i < current->key_count - 1) printf(",");
        }
        printf("]\n");
        
        for (int i = 0; i < current->key_count; i++) {
            if (current->keys[i] >= start_key && current->keys[i] <= end_key) {
                results[result_count++] = current->keys[i];
                printf(" Найден: %d\n", current->keys[i]);
            }
        }
        
        if (current->key_count > 0 && current->keys[current->key_count - 1] > end_key) {
            break;
        }
        current = current->next_leaf;
    }
    
    printf("Результаты range query (%d значений): ", result_count);
    for (int i = 0; i < result_count; i++) printf("%d ", results[i]);
    printf("\n\n");
}

// Range query для обычного B-дерева (для сравнения)
void btree_range_query(struct BTreeNode* node, int start_key, int end_key,
                      int* results, int* result_count) {
    if (node == NULL) return;
    
    int i = 0;
    while (i < node->key_count) {
        if (!node->is_leaf) {
            btree_range_query((struct BTreeNode*)node->children[i], start_key, end_key,
                             results, result_count);
        }
        if (node->keys[i] >= start_key && node->keys[i] <= end_key) {
            results[(*result_count)++] = node->keys[i];
        }
        i++;
    }
    
    if (!node->is_leaf) {
        btree_range_query((struct BTreeNode*)node->children[i], start_key, end_key,
                         results, result_count);
    }
}

void benchmark_range_queries() {
    printf("=== КЕЙС 5: B+-дерево — Range Queries ===\n\n");
    
    // Создаем тестовые данные
    const int DATA_SIZE = 20;
    int test_data[DATA_SIZE];
    for (int i = 0; i < DATA_SIZE; i++) {
        test_data[i] = rand() % 100;
    }
    
    printf("Тестовые данные: ");
    for (int i = 0; i < DATA_SIZE; i++) printf("%d ", test_data[i]);
    printf("\n\n");
    
    // Создаем простое B+-дерево для демонстрации
    struct BPlusNode* bplus_root = create_bplus_node(true, 1000);
    
    // Заполняем данными (упрощенная версия)
    for (int i = 0; i < DATA_SIZE; i++) {
        if (bplus_root->key_count < BPLUS_ORDER - 1) {
            bplus_root->keys[bplus_root->key_count++] = test_data[i];
        }
    }
    
    // Сортируем ключи для наглядности
    for (int i = 0; i < bplus_root->key_count - 1; i++) {
        for (int j = i + 1; j < bplus_root->key_count; j++) {
            if (bplus_root->keys[j] < bplus_root->keys[i]) {
                int temp = bplus_root->keys[i];
                bplus_root->keys[i] = bplus_root->keys[j];
                bplus_root->keys[j] = temp;
            }
        }
    }
    
    printf("B+-дерево (только листья для демонстрации):\n");
    printf("Лист 1: [");
    for (int i = 0; i < bplus_root->key_count; i++) {
        printf("%d", bplus_root->keys[i]);
        if (i < bplus_root->key_count - 1) printf(",");
    }
    printf("]\n");
    
    // Тестируем range queries
    int ranges[][2] = {{10, 30}, {40, 60}, {0, 100}};
    for (int r = 0; r < 3; r++) {
        clock_t start = clock();
        bplus_range_query(bplus_root, ranges[r][0], ranges[r][1]);
        clock_t end = clock();
        printf("B+-tree время: %.3f ms\n", 
               (double)(end - start) * 1000 / CLOCKS_PER_SEC);
        
        // Для сравнения с обычным B-tree
        struct BTreeNode* btree_root = create_btree_node(true, 2000);
        for (int i = 0; i < DATA_SIZE; i++) {
            if (btree_root->key_count < B_ORDER - 1) {
                btree_root->keys[btree_root->key_count++] = test_data[i];
            }
        }
        
        int btree_results[100];
        int btree_count = 0;
        start = clock();
        btree_range_query(btree_root, ranges[r][0], ranges[r][1], btree_results, &btree_count);
        end = clock();
        
        printf("B-tree время: %.3f ms\n", 
               (double)(end - start) * 1000 / CLOCKS_PER_SEC);
        printf("B-tree результаты (%d значений)\n\n", btree_count);
        
        free(btree_root);
    }
    
    printf("Вывод: B+-дерево эффективнее для range queries благодаря:\n");
    printf(" – Связный список  между листьями\n");
    printf(" - Все данные хранятся в листьях\n");
    printf(" - Линейный обход вместо рекурсивного\n");
    
    free(bplus_root);
}

int main() {
    srand(time(NULL));
    
    printf(" КЕЙС 5: B+-ДЕРЕВО - ПОЛНЫЙ АНАЛИЗ \n\n");
    
    benchmark_range_queries();           // Основной benchmark
    test_structure_comparison();         // Тест 1
    test_memory_efficiency();            // Тест 2
    test_operation_performance();        // Тест 3
    test_bplus_practical_recommendations(); // Тест 4
    test_bplus_performance_scaling();    // Тест 5
    
    printf("\n=== ИТОГОВЫЕ ВЫВОДЫ ДЛЯ B+-ДЕРЕВА ===\n");
    printf("Специализировано для range queries и последовательного доступа\n");
    printf("Связный список  между листьями ускоряет последовательные операции\n");
    printf("Разделение данных (листья) и индекса (внутренние узлы)\n");
    printf("Более эффективное использование кэша процессора\n");
    printf("Меньше дисковых операций при больших запросах\n");
    printf("Стандарт для реляционных баз данных и файловых систем\n");
    printf("Лучшая масштабируемость для аналитических workload'ов\n");
    
    return 0;
}
