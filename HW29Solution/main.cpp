#include <iostream>
#include <cstring>
#include <string>
#include <initializer_list>
#include <windows.h>

using namespace std;
/*
==========================================================================================================================================
                                ДОМАШНЄ ЗАВДАННЯ 29 тема: шаблони класів, std::initializer_list, спеціалізація
                                                            клас Backpack<T>
==========================================================================================================================================
*/
// ===================================== РІВЕНЬ 1 =====================================

template <typename T>
class Backpack
{
    //  3. Навіщо рюкзаку два поля, count і capacity? Чому місткість збільшується вдвічі, а не на 1?
private:
    T* items;
    int count;          // скільки предметів лежить
    int capacity;       // на скільки предметів виділено пам'ять

    // Збільшує місткість удвічі: новий буфер → копіюємо → звільняємо старий
    void grow()
    {
        capacity *= 2;
        T* bigger = new T[capacity];
        for (int i = 0; i < count; i++)
        {
            bigger[i] = items[i];
        }
        delete[] items;
        items = bigger;
    }

public:
    Backpack() : items(nullptr), count(0), capacity(2)
    {
        items = new T[capacity];
    }

    Backpack(initializer_list<T> list) : items(nullptr), count(0), capacity((int)list.size())
    {
        if (capacity < 2)
        {
            capacity = 2;
        }
        items = new T[capacity];
        for (const T& item : list)
        {
            items[count] = item;
            count++;
        }
    }

    Backpack(const Backpack& other) : items(nullptr), count(other.count), capacity(other.capacity)
    {
        items = new T[capacity];
        for (int i = 0; i < count; i++)
        {
            items[i] = other.items[i];
        }
    }

    Backpack& operator=(const Backpack& other)
    {
        if (this == &other)
        {
            return *this;
        }
        T* copy = new T[other.capacity];
        for (int i = 0; i < other.count; i++)
        {
            copy[i] = other.items[i];
        }
        delete[] items;
        items = copy;
        count = other.count;
        capacity = other.capacity;
        return *this;
    }

    ~Backpack()
    {
        delete[] items;
    }

    //  2. Чому add і contains приймають const T&, а не просто T?
    void add(const T& item)
    {
        if (count == capacity)
        {
            grow();
        }
        items[count] = item;
        count++;
    }

    //  2. Чому add і contains приймають const T&, а не просто T?
    bool contains(const T& item) const
    {
        for (int i = 0; i < count; i++)
        {
            if (items[i] == item)
            {
                return true;
            }
        }
        return false;
    }

    int getCount() const
    {
        return count;
    }

    int getCapacity() const
    {
        return capacity;
    }

    T& operator[](int index)
    {
        return items[index];
    }

    const T& operator[](int index) const
    {
        return items[index];
    }

    // Дружня функція всередині шаблону: Backpack тут означає Backpack<T>
    friend ostream& operator<<(ostream& out, const Backpack& backpack)
    {
        out << "[";
        for (int i = 0; i < backpack.count; i++)
        {
            if (i > 0)
            {
                out << ", ";
            }
            out << backpack.items[i];
        }
        out << "]";
        return out;
    }
};

// ===================================== РІВЕНЬ 3 =====================================
//   5. (Рівень 3) Чому без спеціалізації contains не знаходить "мапа", хоча така записка в рюкзаку є?
//   І чому для Backpack<string> спеціалізація не потрібна?
template <>
bool Backpack<const char*>::contains(const char* const& item) const
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(items[i], item) == 0)
        {
            return true;
        }
    }
    return false;
}

// ===================================== РІВЕНЬ 2 =====================================

class Item
{
private:
    string name;
    int weight;

public:
    //  4. (Рівень 2) Закоментуйте в Item конструктор за замовчуванням. Де компілятор покаже помилку і чому?
    //  Поверніть конструктор назад.
    Item() : name(""), weight(0) {}

    Item(const string& name, int weight) : name(name), weight(weight) {}

    int getWeight() const
    {
        return weight;
    }

    bool operator==(const Item& other) const
    {
        return name == other.name;
    }

    friend ostream& operator<<(ostream& out, const Item& item)
    {
        out << item.name << " (" << item.weight << " кг)";
        return out;
    }
};

template <typename T>
T total(const Backpack<T>& backpack)
{
    T result = backpack[0];
    for (int i = 1; i < backpack.getCount(); i++)
    {
        result = result + backpack[i];
    }
    return result;
}

int totalWeight(const Backpack<Item>& backpack)
{
    int result = 0;
    for (int i = 0; i < backpack.getCount(); i++)
    {
        result += backpack[i].getWeight();
    }
    return result;
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "===== РІВЕНЬ 1 =====" << endl;

    cout << "1. Монети" << endl;
    //    1. Backpack<int> і Backpack<double> — це один клас чи два? Чому не компілюється supplies = coins; ?
    Backpack<int> coins = { 10, 25, 5 };
    cout << coins << " кількість: " << coins.getCount() << ", місткість: " << coins.getCapacity() << endl;
    coins.add(50);
    coins.add(100);
    cout << coins << " кількість: " << coins.getCount() << ", місткість: " << coins.getCapacity() << endl;

    cout << "2. Пошук" << endl;
    cout << "25: " << coins.contains(25) << ", 7: " << coins.contains(7) << endl;

    cout << "3. Вага припасів" << endl;
    //    1. Backpack<int> і Backpack<double> — це один клас чи два? Чому не компілюється supplies = coins; ?
    Backpack<double> supplies;
    supplies.add(1.5);
    supplies.add(2.25);
    supplies.add(0.5);
    cout << supplies << " місткість: " << supplies.getCapacity() << endl;

    cout << "4. Копія незалежна" << endl;
    Backpack<int> saved = coins;
    coins[0] = 15;
    coins.add(1);
    cout << "coins: " << coins << endl;
    cout << "saved: " << saved << endl;

    cout << "5. Присвоєння" << endl;
    Backpack<int> a;
    Backpack<int> b;
    a = b = saved;
    saved = saved;
    cout << "a: " << a << endl;

    cout << "===== РІВЕНЬ 2 =====" << endl;
    Backpack<Item> hero = { Item("Меч", 5), Item("Щит", 8) };
    hero.add(Item("Зілля", 1));
    cout << hero << endl;
    cout << "Є щит: " << hero.contains(Item("Щит", 0)) << endl;
    cout << "Є лук: " << hero.contains(Item("Лук", 0)) << endl;
    cout << "Загальна вага: " << totalWeight(hero) << " кг" << endl;
    cout << "Усього монет: " << total(coins) << endl;
    cout << "Вага припасів: " << total(supplies) << endl;

    cout << "===== РІВЕНЬ 3 =====" << endl;
    Backpack<const char*> notes = { "ключ від підвалу", "мапа" };
    char query[32];
    strcpy_s(query, "мапа");
    cout << notes << endl;
    cout << "Є мапа: " << notes.contains(query) << endl;   // без спеціалізації — 0

    Backpack<string> stringNotes = { "ключ від підвалу", "мапа" };
    cout << "Є мапа (string): " << stringNotes.contains(query) << endl;   // 1 і без спеціалізації

    return 0;
}

/*
  1. Backpack<int> і Backpack<double> — це один клас чи два? Чому не компілюється supplies = coins; ?
  2. Чому add і contains приймають const T&, а не просто T?
  3. Навіщо рюкзаку два поля, count і capacity? Чому місткість збільшується вдвічі, а не на 1?
  4. (Рівень 2) Закоментуйте в Item конструктор за замовчуванням. Де компілятор покаже помилку і чому?
     Поверніть конструктор назад.
  5. (Рівень 3) Чому без спеціалізації contains не знаходить "мапа", хоча така записка в рюкзаку є?
     І чому для Backpack<string> спеціалізація не потрібна?

  ВІДПОВІДІ НА ЧАСТИНУ «ПОЯСНИ»

  1. 

  2. 

  3. 

  4. 

  5. 

*/
