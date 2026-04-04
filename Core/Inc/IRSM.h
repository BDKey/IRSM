/**
 * @brief Struct that holds all the neccessary data for the IRS
 *
 * Данный класс хранит основные данные пользователя (имя, ID)
 * и предоставляет методы для их обновления.
 *
 * Пример использования:
 * @code
 *   User u("admin", 1);
 *   u.setName("SuperAdmin");
 * @endcode
 *
 * @note Этот класс не является потокобезопасным.
 */
#include <cmath>
#include <string>
#include <map>
#include <list>
#include <functional>
#include <inttypes.h>

//long map(long x, long in_min, long in_max, long out_min, long out_max);
float mapf(float x, float in_min, float in_max, float out_min, float out_max);
void Log(bool IsError, std::string Text);

struct IRS_DelayConfig;
struct IRS_RotorConfig;

class State;
class StateMachine;
class State1;
class State2;
