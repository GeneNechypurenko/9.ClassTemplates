#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

/*
==========================================================================================================================================
                                                     Шаблони класів. Статичний поліморфізм
==========================================================================================================================================

                                                           Статичний поліморфізм
  Поліморфізм — можливість використовувати одне ім'я (функції, оператора, класу) для роботи з різними типами даних.
  - Статичний поліморфізм — потрібна версія обирається на етапі КОМПІЛЯЦІЇ:
      1. перевантаження функцій і операторів;
      2. шаблони функцій і класів;
  - Динамічний поліморфізм — версія обирається під час ВИКОНАННЯ програми:
      1. покажчик на функцію за принципом роботи;
      2. віртуальні методи і за принципом, і за визначенням ООП;

  Шаблони — окремий випадок статичного поліморфізму: один опис → багато типів, без втрат швидкості під час виконання.

                                                              Шаблон класу
  Шаблон класу — це креслення класу, у якому один або кілька типів замінено параметрами. За цим кресленням компілятор сам створює
  потрібні класи.

  Інстанціювання — створення компілятором конкретного класу з шаблону: Array<int>, Array<double>.
  Шаблон перевіряється повністю лише під час інстанціювання.

  Об'єкт шаблону класу можна передати в шаблон функції: T maxElement(const Array<T>& arr) — T компілятор виводить сам.
*/

template <typename T>
class Array
{
private:
    T* data;
    int size;

public:
    // const T& — бо T може бути великим об'єктом: копіювати його зайвий раз не потрібно
    Array(int size, const T& initialValue) : data(nullptr), size(size)
    {
        data = new T[size];
        for (int i = 0; i < size; i++)
        {
            data[i] = initialValue;
        }
    }

    // Копіювати масив у цьому прикладі не потрібно — забороняємо
    Array(const Array& other) = delete;             // всередині класу Array означає Array<T>
    Array& operator=(const Array& other) = delete;

    ~Array()
    {
        delete[] data;
    }

    int getSize() const
    {
        return size;
    }

    // Повертаємо посилання на T: arr[0] = 90; змінює елемент
    T& operator[](int index)
    {
        return data[index];
    }

    // Константна версія — теж const T&, а не T: не копіюємо елемент, лише даємо прочитати
    const T& operator[](int index) const
    {
        return data[index];
    }

    void print() const;                             // оголошення, тіло — поза класом
};

// ===== Метод шаблону поза класом: знову template і Array<T>:: =====
template <typename T>
void Array<T>::print() const
{
    for (int i = 0; i < size; i++)
    {
        cout << data[i] << " ";                     // вимога до T: має працювати cout << T
    }
    cout << endl;
}

// ===== Шаблон функції, що приймає шаблон класу =====
template <typename T>
T maxElement(const Array<T>& arr)
{
    T result = arr[0];                              // викликається константна версія []
    for (int i = 1; i < arr.getSize(); i++)
    {
        if (arr[i] > result)                        // вимога до T: має працювати оператор >
        {
            result = arr[i];
        }
    }
    return result;
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "1. Array<int> — бали студентів" << endl;
    Array<int> scores(5, 0);                        // компілятор створює клас Array<int>
    scores[0] = 90;
    scores[1] = 75;
    scores[2] = 100;
    scores[3] = 60;
    scores[4] = 85;
    scores.print();
    cout << "Найкращий бал: " << maxElement(scores) << endl;       // T = int виводиться сам

    cout << "2. Array<double> — ціни" << endl;
    Array<double> prices(3, 9.99);                  // окремий клас Array<double>
    prices[1] = 149.5;
    prices.print();
    cout << "Найдорожче: " << maxElement(prices) << endl;

    cout << "3. Array<char> — оцінки за шкалою A–F" << endl;
    Array<char> grades(4, 'B');
    grades[2] = 'A';
    grades[3] = 'D';
    grades.print();
    cout << "Розмір: " << grades.getSize() << endl;

    cout << "4. Array<string> — розклад пар на тиждень: T може бути й класом" << endl;
    Array<string> schedule(3, "вільно");
    schedule[0] = "C++";
    schedule[2] = "Git";
    schedule.print();

    // scores = prices;                             // помилка: Array<int> і Array<double> — різні класи
    // Array<int> copy = scores;                    // помилка: копіювання заборонене через = delete

    return 0;
}

/*
  ІНСАЙТИ / BEST PRACTICES
  - Шаблон — це «креслення», а не код. Поки ви не створили Array<int>, компілятор не створює для int жодного рядка коду.
    Більше того, генеруються лише ті методи, які викликаються.
  - Тому помилки в шаблонах часто з'являються пізно і виглядають страшно: компілятор показує довгий ланцюжок.
    Читайте ПЕРШИЙ рядок помилки і шукайте в ньому свій рядок коду.
  - Шаблон класу пишуть повністю в .h файлі (разом з тілами методів). Розділити його на .h і .cpp, як звичайний клас,
    не вийде: компілятор має бачити весь код шаблону в місці, де створюється Array<int>.
  - Ви вже користувалися шаблоном класу, просто не знали цього: string — це basic_string<char>, тобто шаблон,
    налаштований на тип char. А vector<int>, list<int>, map<...> з модуля 25 — це шаблони класів STL. Наш Array — спрощений vector.
  - Бонус: з C++17 компілятор вміє сам вивести T з аргументів конструктора: Array arr(3, 0); — те саме, що Array<int>.
    У курсі пишемо тип явно: так код зрозуміліший.
  - Бонус: параметром шаблону може бути не лише тип, а й число: template <typename T, int size>. Так влаштовано
    std::array<int, 5> — масив фіксованого розміру, відомого під час компіляції.

  ==========================================================================================================================================

  ПРАКТИКА

  1. Доповніть Array<T> з прикладу двома методами:
     - int indexOf(const T& value) const — індекс першого елемента, рівного value, або -1, якщо такого немає;
     - int count(const T& value) const — скільки елементів дорівнюють value.
     У main: перевірте обидва методи на Array<int> scores і Array<char> grades.

  2. Шаблон класу Bounded<T> — значення, яке не може вийти за межі (здоров'я героя, гучність, рівень заряду):
     - поля (private): T value; T minValue; T maxValue;
     - конструктор Bounded(const T& value, const T& minValue, const T& maxValue) — зберігає межі і викликає set(value);
     - void set(const T& newValue) — якщо newValue < minValue, записує minValue; якщо більше за maxValue — maxValue;
       інакше — newValue;
     - T get() const;
     - void increase(const T& amount) і void decrease(const T& amount) — змінюють значення через set;
     - bool isMin() const і bool isMax() const;
     - метод print() const — оголосіть у класі, а тіло напишіть ПОЗА класом. Виводить, наприклад: 70 [0..100]
     У main:
       а) Bounded<int> health(100, 0, 100); — герой отримує шкоду 30, потім 90; виведіть здоров'я і isMin();
          потім лікування на 500 — здоров'я не має стати більшим за 100;
       б) Bounded<double> volume(0.5, 0.0, 1.0); — збільшіть гучність на 0.3 двічі; виведіть значення і isMax().
*/
