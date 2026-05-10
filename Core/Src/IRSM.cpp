/*
 * statemachine.c
 *
 *  Created on: 14 мар. 2026 г.
 *      Author: Арсений
 */

#include <IRSM.h>

/*long map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}*/

struct IRS_DelayConfig {
public:
	//ms
	uint16_t IRSensorDelay;
	//ms
	uint16_t RotorDelay;
	//ms
	uint16_t RammerDelay;
};
struct IRS_RotorConfig {
public:
	//Max RPM
	uint16_t RotorMaxPeriodicity;
	//Min RPM
	uint16_t RotorMinPeriodicity;
	//mm per sec
	uint16_t ConveyorSpeed;
	//mm
	uint16_t SliceLength;
	//Minimal speed for rotor to spin (for L298N driver)
	uint8_t RotorMinSpeed;
	//0-255 for L298N driver
	uint8_t RotorSpeed;
	/* @brief CalculateRotorSpeed
	 * @retval Change success
	 */
	void CalculateRotorSpeed() {
		if (ConveyorSpeed <= 0 || SliceLength <= 0) {
			Log(1, "Wrong values");
			return;
		};
		float periodicity = (float)ConveyorSpeed / (float)SliceLength;
		if (periodicity <= RotorMinPeriodicity || periodicity > RotorMaxPeriodicity) {
			Log(3, "RPM out of range");
			return;
		};
		RotorSpeed = static_cast<uint8_t>(std::lround(mapf(periodicity, RotorMinPeriodicity, RotorMaxPeriodicity, RotorMinSpeed, 255)));
		Log(3, "RPM is set to: " + RotorSpeed);
	}
};


class State;
class StateMachine;

class State {
public:
	std::string Name{};
	State(StateMachine& ParentStateMachine, std::string StateName) {
		this->ParentStateMachine = &ParentStateMachine;
		Name = StateName;
	}
	virtual void OnEnter() {};
	virtual void Tick() {};
	virtual void OnExit() {};
protected:
	StateMachine* ParentStateMachine; //Write *ParentStateMachine in order to access it
};

inline State* FindState(std::list<State*>& States, std::string Name) {
	for (auto var : States)
	{
		if (var->Name == Name) return var;
	}
	return NULL;
}
inline bool CheckState(std::list<State*>& States, std::string Name) {
	for (auto var : States)
	{
		if (var->Name == Name) return true;
	}
	return false;
}

class StateMachine {
public:
	std::list<State*> States{};
	std::string CurrentState{};
	std::function<void(uint8_t, std::string)> Log;
	StateMachine(std::function<void(uint8_t, std::string)> Logger) : Log(Logger) {
		States = {};
		CurrentState = {};
		ToEmit = { false };
	}
	void AddState(State* State) {
		States.push_front(State);
		Log(1, "Added new state: " + State->Name);
		if (CurrentState.empty()) {
			CurrentState = State->Name;
			State->OnEnter();
		}
	}
	void Tick() {
		if (CurrentState.c_str() == NULL) {
			this->Log(3, "NULL state");
			return;
		};
		if (!(CheckState(States, CurrentState))) {
			this->Log(3, "Unknown state: " + CurrentState);
			return;
		}
		FindState(States, CurrentState)->Tick();
		if (ToEmit) {
			if (!(CheckState(States, NewState))) {
				this->Log(3, "Unknown emitted state");
				return;
			};
			FindState(States, CurrentState)->OnExit();
			this->Log(1, "Exited state: " + (std::string)CurrentState);
			CurrentState = NewState;
			FindState(States, CurrentState)->OnEnter();
			this->Log(1, "Entered state: " + (std::string)CurrentState);
			this->Log(1, "New state: " + (std::string)NewState);
			ToEmit = false;
		}
	}
	void Emit(std::string NewState) {
		ToEmit = true;
		this->NewState = NewState;
	}
private:
	bool ToEmit{};
	std::string NewState{};
};
