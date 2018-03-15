# TPGame

## Описание
Есть несколько игроков. У каждого из них есть армия, состоящая из юнитов, деньги, население, ресурсы и база. Цель – разрушить базы других игроков, но при этом не дать другим разрушить свою.
### Ресурсы
В игре есть несколько типов ресурсов. Каждый из них можно купить за деньги.
### Юниты
Для покупки доступны несколько типов юнитов.  Каждый юнит имеет свои характеристики. Юниты могут нападать на других юнитов. Когда у игрока нет живых юнитов, юнитами можно нападать на его базу. Каждый юнит ежедневно требует немного денег, если же их нет, то ему наносится урон, пропорциональный недостающему количеству денег. Юнитов можно лечить - стоимость пропорциональна нанесенному им урону. 
### Население
Население каждый день платит налоги. Их уровень можно регулировать. Большие налоги - уменьшение населения.

## Настройка
### Ресурсы
Настройте типы ресурсов в `resourceList.cpp`, добавив новый ресурс:
```c++
ResourceAdder newResource("newResourceName", 1000);
```
и начальные значения для игрока в `playerCalc.cpp`
### Действия
Добавьте новое действие в `availableActions.cpp`:
```c++
class NewAction : public Action {
    using Action::Action;
public:
    // конструктор запроса
    NewAction(...);

    // парсер, возвращающий new NewAction(...), если запрос удалось распарсить, иначе nullptr
    Action *parseQuery(Player *, const std::vector<std::string> &);

    // обработка действия
    void process();

    ~NewAction() = default;
};

StoreAction newActionStored(new NewAction()); // добавление действия в список доступных действий
```
### Юниты
Добавьте новый класс юнитов в `availableUnits.cpp`:
```c++
class NewUnit : public UnitCRTP<NewUnit> {
    using UnitCRTP<NewUnit>::UnitCRTP;
};

template<> const int UnitCRTP<NewUnit>::_coins = 100;                                                 // количество монет для создания юнита
template<> const int UnitCRTP<NewUnit>::_people = 1;                                                  // количество людей для создания юнита
template<> const Resources* UnitCRTP<NewUnit>::_resources = new Resources(std::vector<int>({0, 0}));  // необходимые ресурсы для создания юнита
template<> const int UnitCRTP<NewUnit>::_dailyNeeds = 10;                                             // количество монет, которые ежедневно нужно платить юниту
template<> const int UnitCRTP<NewUnit>::_initialHealth = 100;                                         // изначальное здоровье юнита
template<> const int UnitCRTP<NewUnit>::_outDamage = 70;                                              // наносимый урон другому юниту
template<> const int UnitCRTP<NewUnit>::_inDamage = 25;                                               // ответный урон
template<> const float UnitCRTP<NewUnit>::_moneylessDamage = 0.20;                                    // урон за каджую недополученную монету ежедневного финансирования
template<> const int UnitCRTP<NewUnit>::_towerAttack = 40;                                            // наносимый урон базе
template<> const std::string UnitCRTP<NewUnit>::_unitClass = "archer";                                // имя класса юнитов
template<> const std::string UnitCRTP<NewUnit>::_unitDesc = "Man with a bow";                         // описание класса юнитов
StoreUnit archerUnitStored(new NewUnit());                                                            // добавление юнита в список доступных для покупки
```
