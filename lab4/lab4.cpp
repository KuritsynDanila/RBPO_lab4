#include <iostream>
#include <new>

template <typename T>
class QueueBasedOnUnidirectionalCyclicLinkedList {
private:
    template <typename T>
    class Node {
    public:
        T data;
        Node* next;
        

        Node(T data = T(), Node* next = nullptr) {
            this->data = data;
            this->next = next;
        }
    };

    Node<T>* first; // Указатель на первый элемент (начало очереди)
    Node<T>* last;// Указатель на последний  элемент (конец очереди)
    int size; // Размер очереди

public:

    int GetSize() const {
        return size;
    };

    // Функция очистки очереди
    void clear();

    // Функция добавления элемента в очередь
    bool Push(const T& element);

    // Функция удаления элемента из очереди
    bool Pop(T& element);

    bool Peek(T& element) const;

    // Конструктор по умолчанию
    QueueBasedOnUnidirectionalCyclicLinkedList();

    // Конструктор копирования
    QueueBasedOnUnidirectionalCyclicLinkedList(const QueueBasedOnUnidirectionalCyclicLinkedList<T>& other);

    // Конструктор перемещения
    QueueBasedOnUnidirectionalCyclicLinkedList(QueueBasedOnUnidirectionalCyclicLinkedList<T>&& other);

    // Конструктор, принимающий на вход std::initializer_list
    QueueBasedOnUnidirectionalCyclicLinkedList(std::initializer_list<T> ilist);

    // Деструктор
    ~QueueBasedOnUnidirectionalCyclicLinkedList();

    // Переопределение оператора присваивания (копирования)
    QueueBasedOnUnidirectionalCyclicLinkedList<T>& operator=(const QueueBasedOnUnidirectionalCyclicLinkedList<T>& other);

    // Переопределение оператора присваивания (перемещение)
    QueueBasedOnUnidirectionalCyclicLinkedList<T>& operator=(QueueBasedOnUnidirectionalCyclicLinkedList<T>&& other);

    // Переопределение оператора присваивания (с использованием initializer_list)
    QueueBasedOnUnidirectionalCyclicLinkedList<T>& operator=(std::initializer_list<T> ilist);
};

template<typename T>
QueueBasedOnUnidirectionalCyclicLinkedList<T>::QueueBasedOnUnidirectionalCyclicLinkedList()
{
    size = 0;
    first = nullptr;
    last = nullptr;
}

template<typename T>
QueueBasedOnUnidirectionalCyclicLinkedList<T>::~QueueBasedOnUnidirectionalCyclicLinkedList() {
    clear();  // Очищаем очередь, используя функцию clear
}


template <typename T>
QueueBasedOnUnidirectionalCyclicLinkedList<T>::QueueBasedOnUnidirectionalCyclicLinkedList(const QueueBasedOnUnidirectionalCyclicLinkedList<T>& other) : first(nullptr),last(nullptr), size(0) {
    *this = other;  // Используем оператор присваивания
}

template <typename T>
QueueBasedOnUnidirectionalCyclicLinkedList<T>::QueueBasedOnUnidirectionalCyclicLinkedList(QueueBasedOnUnidirectionalCyclicLinkedList<T>&& other) : first(nullptr),last(nullptr), size(0) {
    *this = std::move(other);  // Используем оператор присваивания перемещения
}

template <typename T>
QueueBasedOnUnidirectionalCyclicLinkedList<T>::QueueBasedOnUnidirectionalCyclicLinkedList(std::initializer_list<T> ilist) : first(nullptr),last(nullptr), size(0) {
    *this = ilist;  // Используем оператор присваивания
}


template <typename T>
QueueBasedOnUnidirectionalCyclicLinkedList<T>& QueueBasedOnUnidirectionalCyclicLinkedList<T>::operator=(const QueueBasedOnUnidirectionalCyclicLinkedList<T>& other) {
    // Защита от самоприсваивания
    if (this == &other)
        return *this;
    // Очищаем текущий список
    clear();

    // Проверяем, не пуст ли исходный список
    if (other.first == nullptr)
        return *this;
    

    // Начинаем с элемента, который находится на "дне" исходной очереди
    Node<T>* currentOther = other.first;

    do {
        // Копируем элементы из исходного списка в новый с помощью Push
        if (!Push(currentOther->data)) {
            // Обработка ошибки выделения памяти, если Push возвращает false
            clear();  // Очищаем уже скопированные элементы
            throw std::bad_alloc();  // Выбрасываем исключение
        }
        currentOther = currentOther->next;
    } while (currentOther != other.first);  // Повторяем, пока не вернемся к начальному узлу
    
    

    return *this;
}

template <typename T>
QueueBasedOnUnidirectionalCyclicLinkedList<T>& QueueBasedOnUnidirectionalCyclicLinkedList<T>::operator=(QueueBasedOnUnidirectionalCyclicLinkedList<T>&& other) {
    if (this == &other)
        return *this;
    // Защита от самоприсваивания
    clear();  // Очищаем текущий список

    // Перемещаем данные из other в текущий объект
    first = other.first;
    last = other.last;
    size = other.size;

    // Оставляем other в валидном, но пустом состоянии
    other.first = nullptr;
    other.last = nullptr;
    other.size = 0;
    

    return *this;
}


template <typename T>
QueueBasedOnUnidirectionalCyclicLinkedList<T>& QueueBasedOnUnidirectionalCyclicLinkedList<T>::operator=(std::initializer_list<T> ilist) {
    clear();  // Очистка текущего списка

    // Добавление каждого элемента из списка инициализации
    for (const T& item : ilist) {
        Push(item);
    }

    return *this;  // Возвращение ссылки на текущий объект
}

template<typename T>
void QueueBasedOnUnidirectionalCyclicLinkedList<T>::clear() {
    Node<T>* nextNode;
    while (size > 0) {
        nextNode = first->next; // Сохраняем ссылку на следующий элемент
        delete first; // Удаляем текущий узел
        first = nextNode; // Переходим к следующему узлу
        --size;
    }

    first = nullptr; // Обнуляем указатель на начало очереди
    last = nullptr; // Обнуляем указатель на конец очереди
}

template<typename T>
bool QueueBasedOnUnidirectionalCyclicLinkedList<T>::Push(const T& element) {
    // Создаем новый узел
    Node<T>* newNode = new(std::nothrow) Node<T>(element);

    // Проверяем, был ли узел успешно создан
    if (newNode == nullptr) {
        // Не удалось выделить память
        return false;
    }

    if (size == 0) {
        // Если список пуст, новый узел становится первым и единственным элементом
        newNode->next = newNode; // Ссылки на самого себя
        first = newNode;
        
    }
    else {
        // Вставляем новый узел в конец
        last->next = newNode;
        newNode->next = first;//Соблюдение цикличности.
               
    }

    last = newNode; // Обновляем last для указания на новый узел
    size++;
    return true;
}

template<typename T>
bool QueueBasedOnUnidirectionalCyclicLinkedList<T>::Pop(T& element) {
    // Проверяем, пуста ли очередь
    if (size == 0) {
        // Очередь пуста
        return false;
    }

    // Сохраняем данные первого элемента
    element = first->data;

    // ССылка на старый первый элемент
    Node<T>* toDelete = first;

    // Если в очереди остался только один элемент
    if (first == last) {

        first = last = nullptr;
    }
    else {
        // В очереди более одного элемента
        first = first->next;
        last->next = first;
    }

    delete toDelete;  // Удаляем старый первый элемент
    --size;           // Уменьшаем размер очереди
    return true;
}

template<typename T>
bool QueueBasedOnUnidirectionalCyclicLinkedList<T>::Peek(T& element) const {
    // Проверяем, пуста ли очередь
    if (size==0) {
        return false;
    }

    // Копируем данные первого элемента
    element = first->data;

    return true;
}



template<typename T>
void printQueue(const QueueBasedOnUnidirectionalCyclicLinkedList<T>& other)
{
    QueueBasedOnUnidirectionalCyclicLinkedList<T> new_queue = other;
    T data;
    
    

    while (new_queue.GetSize() > 0)
    {
        bool flag = new_queue.Pop(data);
        std::cout << data <<"  "<<flag << std::endl;


    };

    std::cout << std::endl;
}



int main() {
    QueueBasedOnUnidirectionalCyclicLinkedList<int> *new_queue = new QueueBasedOnUnidirectionalCyclicLinkedList<int>();
    for (int i = 0; i <= 10; i++)
    {
        new_queue->Push(i);

    }
    printQueue(*new_queue);
    QueueBasedOnUnidirectionalCyclicLinkedList<int>* new_queue1 = new QueueBasedOnUnidirectionalCyclicLinkedList<int>();
    for (int i = 11; i <= 15; i++)
    {
        new_queue1->Push(i);

    }
    std::swap(new_queue, new_queue1);
    int data;
    new_queue->Peek(data);
    printQueue(*new_queue);
    std::cout << data;

    return 0;
}