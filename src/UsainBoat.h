// statemachineFsm.h has been generated automatically by StateBuilderCpp 3.0
// Executable:
// file:/C:/Users/Victor%20van%20Wijhe/AppData/Local/StateForge/StateBuilderCpp/bin/StateBuilderCpp.exe
// Command line options:
//  D:/statemachine.fsmcpp
// Date: 28-mei-2018 11:32:16

#ifndef STATEMACHINEFSM_H
#define STATEMACHINEFSM_H


// Includes
#include <cstring>
#include <fsm/Fsm.hpp>

// Object classes forward declaration

// Context classes forward declaration
class statemachineContext;
// Parallel classes forward declaration
// State classes forward declaration
class statemachineS_RELAYState;
class statemachineS_WAIT_FOR_RADIO_MESSAGEState;
class statemachineEndState;
class statemachineS_MANUAL_CONTROLState;
class statemachineS_VOLGPLANState;
class statemachineUsainBoatState;

// Object classes forward declaration within the custom namepace
class BusinessObject;


// Context declaration for state UsainBoat
class statemachineContext : public fsm::Context<statemachineUsainBoatState, statemachineContext> {
 public:
  // Constructor
  statemachineContext(BusinessObject& myBusinessObject);

  // Destructor
  virtual ~statemachineContext();


  static const fsm::StateNameToId* GetStateNameToId();
  // MainEvent's events
  void E_START_FOLLOW();
  void E_START_RELAY();
  void E_START_MANUAL();
  void E_WAIT_NEXT_COMMAND();
  void E_STOP_STATEMACHINE();


  // Enter the initial state: walk the onEntry chain from the top state to the initial state.
  void EnterInitialState();

  // Leave the current state: walk the onExit chain from the current state to the top state.
  void LeaveCurrentState();

  BusinessObject& getBusinessObject(){return m_myBusinessObject;}

 private:
  BusinessObject& m_myBusinessObject;


};


// State statemachineUsainBoatState
class statemachineUsainBoatState : public fsm::State<statemachineContext, statemachineUsainBoatState> {
 public:
  // Constructor
  statemachineUsainBoatState(const char* pName, int id);

  // Singleton pattern
  static const statemachineUsainBoatState& GetInstance();

  // Destructor
  virtual ~statemachineUsainBoatState();

  // OnEntry and OnExit
  virtual void OnEntry(statemachineContext& context) const;
  virtual void OnExit(statemachineContext& context) const;

  // Events
  virtual void E_START_FOLLOW(statemachineContext &context) const ;
  virtual void E_START_RELAY(statemachineContext &context) const ;
  virtual void E_START_MANUAL(statemachineContext &context) const ;
  virtual void E_WAIT_NEXT_COMMAND(statemachineContext &context) const ;
  virtual void E_STOP_STATEMACHINE(statemachineContext &context) const ;

};


// State statemachineS_WAIT_FOR_RADIO_MESSAGEState
class statemachineS_WAIT_FOR_RADIO_MESSAGEState : public statemachineUsainBoatState {
 public:
  // Constructor
  statemachineS_WAIT_FOR_RADIO_MESSAGEState(const char* pName, int id);

  // Singleton pattern
  static const statemachineS_WAIT_FOR_RADIO_MESSAGEState& GetInstance();

  // Destructor
  virtual ~statemachineS_WAIT_FOR_RADIO_MESSAGEState();

  // OnEntry and OnExit
  virtual void OnEntry(statemachineContext& context) const;
  virtual void OnExit(statemachineContext& context) const;

  // Events
  virtual void E_START_FOLLOW(statemachineContext &context) const ;
  virtual void E_START_MANUAL(statemachineContext &context) const ;
  virtual void E_START_RELAY(statemachineContext &context) const ;
  virtual void E_STOP_STATEMACHINE(statemachineContext &context) const ;

};


// State statemachineS_VOLGPLANState
class statemachineS_VOLGPLANState : public statemachineUsainBoatState {
 public:
  // Constructor
  statemachineS_VOLGPLANState(const char* pName, int id);

  // Singleton pattern
  static const statemachineS_VOLGPLANState& GetInstance();

  // Destructor
  virtual ~statemachineS_VOLGPLANState();

  // OnEntry and OnExit
  virtual void OnEntry(statemachineContext& context) const;
  virtual void OnExit(statemachineContext& context) const;

  // Events
  virtual void E_WAIT_NEXT_COMMAND(statemachineContext &context) const ;

};


// State statemachineEndState
class statemachineEndState : public statemachineUsainBoatState {
 public:
  // Constructor
  statemachineEndState(const char* pName, int id);

  // Singleton pattern
  static const statemachineEndState& GetInstance();

  // Destructor
  virtual ~statemachineEndState();

  // OnEntry and OnExit
  virtual void OnEntry(statemachineContext& context) const;
  virtual void OnExit(statemachineContext& context) const;

  // Events
  virtual void E_START_FOLLOW(statemachineContext &context) const ;
  virtual void E_START_RELAY(statemachineContext &context) const ;
  virtual void E_START_MANUAL(statemachineContext &context) const ;
  virtual void E_WAIT_NEXT_COMMAND(statemachineContext &context) const ;
  virtual void E_STOP_STATEMACHINE(statemachineContext &context) const ;

};


// State statemachineS_RELAYState
class statemachineS_RELAYState : public statemachineUsainBoatState {
 public:
  // Constructor
  statemachineS_RELAYState(const char* pName, int id);

  // Singleton pattern
  static const statemachineS_RELAYState& GetInstance();

  // Destructor
  virtual ~statemachineS_RELAYState();

  // OnEntry and OnExit
  virtual void OnEntry(statemachineContext& context) const;
  virtual void OnExit(statemachineContext& context) const;

  // Events
  virtual void E_WAIT_NEXT_COMMAND(statemachineContext &context) const ;

};


// State statemachineS_MANUAL_CONTROLState
class statemachineS_MANUAL_CONTROLState : public statemachineUsainBoatState {
 public:
  // Constructor
  statemachineS_MANUAL_CONTROLState(const char* pName, int id);

  // Singleton pattern
  static const statemachineS_MANUAL_CONTROLState& GetInstance();

  // Destructor
  virtual ~statemachineS_MANUAL_CONTROLState();

  // OnEntry and OnExit
  virtual void OnEntry(statemachineContext& context) const;
  virtual void OnExit(statemachineContext& context) const;

  // Events
  virtual void E_WAIT_NEXT_COMMAND(statemachineContext &context) const ;

};

#endif //STATEMACHINEFSM_H

