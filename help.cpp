#include <stdio.h>
#include <string.h>

// Требования системы (что нужно нашей программе)
struct SystemRequirements {
    const char* system_name;      // Название системы
    int search_frequency;         // Как часто ищем (1-10)
    int update_frequency;         // Как часто добавляем/удаляем (1-10)  
    int range_queries_needed;     // Нужны ли поиски по диапазону (1-10)
    int memory_limited;           // Ограничена ли память (1-10)
    const char* storage_type;     // "memory" или "disk"
};

// Кандидат (структура данных, которую мы рассматриваем)
struct StructureCandidate {
    const char* name;             // Название структуры
    int search_speed;             // Скорость поиска (1-10)
    int insert_speed;             // Скорость вставки (1-10)
    int range_query_speed;        // Скорость поиска по диапазону (1-10)
    int memory_efficiency;        // Эффективность использования памяти (1-10)
    const char* best_use_case;    // Для чего лучше всего подходит
    const char* worst_use_case;   // Для чего не подходит
};

// Функция оценки пригодности структуры для системы
void evaluate_structure(struct SystemRequirements system, 
                       struct StructureCandidate candidate) {
    printf("Оцениваем %s для %s:\n", candidate.name, system.system_name);
    
    int total_score = 0;
    
    // Умножаем возможности структуры на потребности системы
    total_score += candidate.search_speed * system.search_frequency;
    total_score += candidate.insert_speed * system.update_frequency;
    total_score += candidate.range_query_speed * system.range_queries_needed;
    total_score += candidate.memory_efficiency * system.memory_limited;
    
    // Дополнительные бонусы/штрафы
    if (strcmp(system.storage_type, "disk") == 0) {
        if (strstr(candidate.name, "B+") != NULL) total_score += 20;
        if (strstr(candidate.name, "B-tree") != NULL) total_score += 15;
    } else {
        if (strstr(candidate.name, "AVL") != NULL) total_score += 10;
        if (strstr(candidate.name, "RBT") != NULL) total_score += 8;
    }
    
    printf("  Итоговый балл: %d\n", total_score);
    printf("  Лучше всего для: %s\n", candidate.best_use_case);
    printf("  Не рекомендуется для: %s\n\n", candidate.worst_use_case);
}

// Сравнение всех структур для одной системы
void compare_for_system(struct SystemRequirements system, 
                       struct StructureCandidate candidates[], 
                       int count) {
    printf("=== СИСТЕМА: %s ===\n", system.system_name);
    printf("Требования: поиск=%d/10, обновления=%d/10, диапазоны=%d/10\n", 
           system.search_frequency, system.update_frequency, system.range_queries_needed);
    printf("Память: %s, Хранилище: %s\n\n", 
           system.memory_limited >= 7 ? "ограничена" : "не ограничена",
           system.storage_type);
    
    int best_score = -1;
    const char* best_candidate = NULL;
    
    for (int i = 0; i < count; i++) {
        // Вычисляем балл для каждого кандидата
        int score = candidates[i].search_speed * system.search_frequency +
                   candidates[i].insert_speed * system.update_frequency +
                   candidates[i].range_query_speed * system.range_queries_needed +
                   candidates[i].memory_efficiency * system.memory_limited;
        
        // Бонусы за специализацию
        if (strcmp(system.storage_type, "disk") == 0) {
            if (strstr(candidates[i].name, "B+") != NULL) score += 20;
            if (strstr(candidates[i].name, "B-tree") != NULL) score += 15;
        }
        
        printf("%s: %d баллов\n", candidates[i].name, score);
        
        if (score > best_score) {
            best_score = score;
            best_candidate = candidates[i].name;
        }
    }
    
    printf("\n🏆 ПОБЕДИТЕЛЬ: %s (%d баллов)\n", best_candidate, best_score);
    printf("=========================================\n\n");
}

int main() {
    printf("=== АРХИТЕКТУРНЫЙ БАТТЛ: Выбор структуры данных ===\n\n");
    
    // Наши кандидаты (структуры данных)
    struct StructureCandidate candidates[] = {
        // AVL Tree
        {
            "AVL Tree",
            10,    // Отличный поиск
            6,     // Средние вставки (много вращений)
            4,     // Плохие диапазонные запросы
            7,     // Нормальная память
            "Системы с частым поиском и редкими изменениями",
            "Системы с частыми вставками/удалениями"
        },
        
        // Red-Black Tree
        {
            "Red-Black Tree", 
            8,     // Хороший поиск
            8,     // Хорошие вставки
            5,     // Средние диапазонные запросы
            7,     // Нормальная память
            "Сбалансированные нагрузки (поиск и обновления)",
            "Системы с частыми поисками по диапазону"
        },
        
        // B-tree
        {
            "B-tree",
            7,     // Средний поиск
            7,     // Средние вставки
            6,     // Нормальные диапазонные запросы
            8,     // Хорошая эффективность памяти
            "Дисковые хранилища, базы данных",
            "In-memory приложения"
        },
        
        // B+ tree
        {
            "B+ tree",
            8,     // Хороший поиск
            7,     // Средние вставки
            10,    // Отличные диапазонные запросы!
            9,     // Отличная эффективность памяти
            "Базы данных, системы с range queries",
            "Простые in-memory кеши"
        },
        
        // 2-3 Tree
        {
            "2-3 Tree",
            7,     // Средний поиск
            7,     // Средние вставки
            4,     // Плохие диапазонные запросы
            6,     // Средняя эффективность памяти
            "Учебные проекты, простые системы",
            "Высоконагруженные production-системы"
        }
    };
    
    // Примеры реальных систем
    struct SystemRequirements systems[] = {
        // 1. DNS кеш-сервер
        {
            "DNS кеш-сервер",
            9,  // Очень частый поиск по доменным именам
            2,  // Редкие обновления (когда TTL истекает)
            1,  // Диапазонные запросы почти не нужны
            8,  // Память ограничена (кеш в RAM)
            "memory"
        },
        
        // 2. Хранилище логов
        {
            "Хранилище логов с временными метками", 
            6,  // Средний поиск
            8,  // Частые добавления новых логов
            9,  // Очень частые запросы "логи за период"
            3,  // Память не сильно ограничена
            "disk"
        },
        
        // 3. In-memory кеш сессий
        {
            "In-memory кеш сессий пользователей",
            8,  // Частый поиск сессий
            7,  // Частые обновления (новые сессии, logout)
            2,  // Диапазонные запросы редко нужны
            9,  // Память сильно ограничена
            "memory"
        },
        
        // 4. Гео-индекс для карт
        {
            "Гео-индекс для картографии",
            7,  // Средний поиск
            5,  // Средние обновления карты
            8,  // Частые запросы "объекты в области"
            5,  // Память средне ограничена
            "memory"
        }
    };
    
    int num_candidates = sizeof(candidates) / sizeof(candidates[0]);
    int num_systems = sizeof(systems) / sizeof(systems[0]);
    
    // Сравниваем для каждой системы
    for (int i = 0; i < num_systems; i++) {
        compare_for_system(systems[i], candidates, num_candidates);
    }
    
    // Интерактивная часть: пользователь создает свою систему
    printf("=== СОЗДАЙТЕ СВОЮ СИСТЕМУ ===\n");
    
    struct SystemRequirements custom_system;
    custom_system.system_name = "Моя кастомная система";
    
    printf("Опишите вашу систему:\n");
    
    printf("Как часто будет поиск? (1-10, где 10=очень часто): ");
    scanf("%d", &custom_system.search_frequency);
    
    printf("Как часто будут вставки/удаления? (1-10): ");
    scanf("%d", &custom_system.update_frequency);
    
    printf("Как часто нужны поиски по диапазону? (1-10): ");
    scanf("%d", &custom_system.range_queries_needed);
    
    printf("Память ограничена? (1-10, где 10=сильно ограничена): ");
    scanf("%d", &custom_system.memory_limited);
    
    printf("Тип хранилища (1=memory, 2=disk): ");
    int storage_choice;
    scanf("%d", &storage_choice);
    custom_system.storage_type = (storage_choice == 2) ? "disk" : "memory";
   printf("\n");
    compare_for_system(custom_system, candidates, num_candidates);
   printf("\n=== ОБЪЯСНЕНИЕ РЕЗУЛЬТАТОВ ===\n");
    printf("• AVL Tree: идеален когда поиск >> вставок\n");
    printf("• Red-Black Tree: универсальный выбор для сбалансированных нагрузок\n");
    printf("• B-tree: для дисковых систем, минимизирует I/O операции\n");
    printf("• B+ tree: король баз данных и range queries\n");
    printf("• 2-3 Tree: учебная структура, редко используется на практике\n");
    return 0;
}
