
// statemachineFsm.cpp has been generated automatically by StateBuilderCpp 3.0
// Executable:
// file:/C:/Users/Victor%20van%20Wijhe/AppData/Local/StateForge/StateBuilderCpp/bin/StateBuilderCpp.exe
// Command line options
// Date: 28-mei-2018 11:32:16

// Includes
#include "UsainBoat.h"

#include "mbed.h"
#include <fsm/FSM.hpp>

// Disable some MS compiler warnings
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4355) // 'this' : used in base member initialiser list
#pragma warning(disable:4189) // local variable is initialised but not referenced
#pragma warning(disable:4100) // unreferenced formal parameter
#endif

// Disable some GCC compiler warnings
#if ((__GNUC__ * 100) + __GNUC_MINOR__) >= 402
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

// State names and ids
const fsm::StateNameToId kStateNameToIdstatemachineContext[] =
    {
        {"UsainBoat", 0},
        {"S_WAIT_FOR_RADIO_MESSAGE", 1},
        {"S_VOLGPLAN", 2},
        {"End", 3},
        {"S_RELAY", 4},
        {"S_MANUAL_CONTROL", 5},
        {NULL, 0}
    };

// Transition names and ids
const fsm::TransitionNameToId kTransitionNameToIdstatemachineContext[] =
    {
        {"E_START_FOLLOW[follow]", 0},
        {"E_START_MANUAL[manual]", 1},
        {"E_START_RELAY[relay]", 2},
        {"E_STOP_STATEMACHINE", 3},
        {"E_WAIT_NEXT_COMMAND[state_done]", 4},
        {"E_WAIT_NEXT_COMMAND", 6},
        {NULL, 0}
    };

// State Instantiation
static const statemachineUsainBoatState kstatemachineUsainBoatState(kStateNameToIdstatemachineContext[0].pcName, 0);
static const statemachineS_WAIT_FOR_RADIO_MESSAGEState kstatemachineS_WAIT_FOR_RADIO_MESSAGEState(kStateNameToIdstatemachineContext[1].pcName, 1);
static const statemachineS_VOLGPLANState kstatemachineS_VOLGPLANState(kStateNameToIdstatemachineContext[2].pcName, 2);
static const statemachineEndState kstatemachineEndState(kStateNameToIdstatemachineContext[3].pcName, 3);
static const statemachineS_RELAYState kstatemachineS_RELAYState(kStateNameToIdstatemachineContext[4].pcName, 4);
static const statemachineS_MANUAL_CONTROLState kstatemachineS_MANUAL_CONTROLState(kStateNameToIdstatemachineContext[5].pcName, 5);

/**************************************************************
  * Contexts implementations
  *************************************************************/
/**************************************************************
  * Context statemachineContext
  *************************************************************/
statemachineContext::statemachineContext(BusinessObject& myBusinessObject) :
    fsm::Context<statemachineUsainBoatState, statemachineContext>()
    , m_myBusinessObject(myBusinessObject)
{
  SetName("statemachineContext");
  SetInitialState(statemachineS_WAIT_FOR_RADIO_MESSAGEState::GetInstance());
}

statemachineContext::~statemachineContext(){}


const fsm::StateNameToId* statemachineContext::GetStateNameToId()
{
  return kStateNameToIdstatemachineContext;
}

void statemachineContext::EnterInitialState()
{
  fsm::WalkChainEntry<statemachineContext, statemachineUsainBoatState>(*this, &statemachineUsainBoatState::GetInstance() , &GetState());
}

void statemachineContext::LeaveCurrentState()
{
  fsm::WalkChainExit<statemachineContext, statemachineUsainBoatState>(*this, &GetState(), &statemachineUsainBoatState::GetInstance());
}

// MainEvent synchronous events
void statemachineContext::E_START_FOLLOW()
{
  GetState().E_START_FOLLOW(*this);
}

void statemachineContext::E_START_RELAY()
{
  GetState().E_START_RELAY(*this);
}

void statemachineContext::E_START_MANUAL()
{
  GetState().E_START_MANUAL(*this);
}

void statemachineContext::E_WAIT_NEXT_COMMAND()
{
  GetState().E_WAIT_NEXT_COMMAND(*this);
}

void statemachineContext::E_STOP_STATEMACHINE()
{
  GetState().E_STOP_STATEMACHINE(*this);
}

// Timer start implementation

/**************************************************************
  * States implementations
 **************************************************************/
/**************************************************************
 * State implementation for statemachineUsainBoatState
 **************************************************************/

// State Constructor
statemachineUsainBoatState::statemachineUsainBoatState(const char* pName, int id) : fsm::State<statemachineContext, statemachineUsainBoatState>(pName, id)
{
}

// State Destructor
statemachineUsainBoatState::~statemachineUsainBoatState()
{
}

// State GetInstance
const statemachineUsainBoatState& statemachineUsainBoatState::GetInstance()
{
  return kstatemachineUsainBoatState;
}

// OnEntry
void statemachineUsainBoatState::OnEntry(statemachineContext &context) const
{
  // OnEntry for state statemachineUsainBoatState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), statemachineUsainBoatState::GetInstance().GetName());
  }
}

// OnExit
void statemachineUsainBoatState::OnExit(statemachineContext &context) const
{
  // OnExit for state statemachineUsainBoatState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), statemachineUsainBoatState::GetInstance().GetName());
  }
}

// Events implementation for state  statemachineUsainBoatState
void statemachineUsainBoatState::E_START_FOLLOW(statemachineContext &context) const
{
  // 0 transition(s)
}

void statemachineUsainBoatState::E_START_RELAY(statemachineContext &context) const
{
  // 0 transition(s)
}

void statemachineUsainBoatState::E_START_MANUAL(statemachineContext &context) const
{
  // 0 transition(s)
}

void statemachineUsainBoatState::E_WAIT_NEXT_COMMAND(statemachineContext &context) const
{
  // 0 transition(s)
}

void statemachineUsainBoatState::E_STOP_STATEMACHINE(statemachineContext &context) const
{
  // 0 transition(s)
}


/**************************************************************
 * State implementation for statemachine S_WAIT_FOR_RADIO_MESSAGE State
 **************************************************************/

// State Constructor
statemachineS_WAIT_FOR_RADIO_MESSAGEState::statemachineS_WAIT_FOR_RADIO_MESSAGEState(const char* pName, int id) : statemachineUsainBoatState(pName, id)
{
  m_pStateParent = &statemachineUsainBoatState::GetInstance();
}

// State Destructor
statemachineS_WAIT_FOR_RADIO_MESSAGEState::~statemachineS_WAIT_FOR_RADIO_MESSAGEState()
{
}

// State GetInstance
const statemachineS_WAIT_FOR_RADIO_MESSAGEState& statemachineS_WAIT_FOR_RADIO_MESSAGEState::GetInstance()
{
  return kstatemachineS_WAIT_FOR_RADIO_MESSAGEState;
}

// OnEntry
void statemachineS_WAIT_FOR_RADIO_MESSAGEState::OnEntry(statemachineContext &context) const
{
  // OnEntry for state statemachineS_WAIT_FOR_RADIO_MESSAGEState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), statemachineS_WAIT_FOR_RADIO_MESSAGEState::GetInstance().GetName());
  }




}

// OnExit
void statemachineS_WAIT_FOR_RADIO_MESSAGEState::OnExit(statemachineContext &context) const
{
  // OnExit for state statemachineS_WAIT_FOR_RADIO_MESSAGEState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), statemachineS_WAIT_FOR_RADIO_MESSAGEState::GetInstance().GetName());
  }
}

// Events implementation for state  statemachine S_WAIT_FOR_RADIO_MESSAGE State
void statemachineS_WAIT_FOR_RADIO_MESSAGEState::E_START_FOLLOW(statemachineContext &context) const
{
  // 1 transition(s)
  BusinessObject& myBusinessObject = context.getBusinessObject();
  if((follow) == true){
    context.SetTransitionName("E_START_FOLLOW[follow]");
    fsm::ProcessTransitionPre<statemachineContext, statemachineUsainBoatState>(context, &statemachineS_VOLGPLANState::GetInstance());
    // No action to do
    fsm::ProcessTransitionPost<statemachineContext, statemachineUsainBoatState>(context, &statemachineS_VOLGPLANState::GetInstance());
    return;
  }
  // pass the event to the parent state
  statemachineUsainBoatState::GetInstance().E_START_FOLLOW(context);
}

void statemachineS_WAIT_FOR_RADIO_MESSAGEState::E_START_MANUAL(statemachineContext &context) const
{
  // 1 transition(s)
  BusinessObject& myBusinessObject = context.getBusinessObject();
  if((manual) == true){
    context.SetTransitionName("E_START_MANUAL[manual]");
    fsm::ProcessTransitionPre<statemachineContext, statemachineUsainBoatState>(context, &statemachineS_MANUAL_CONTROLState::GetInstance());
    // No action to do
    fsm::ProcessTransitionPost<statemachineContext, statemachineUsainBoatState>(context, &statemachineS_MANUAL_CONTROLState::GetInstance());
    return;
  }
  // pass the event to the parent state
  statemachineUsainBoatState::GetInstance().E_START_MANUAL(context);
}

void statemachineS_WAIT_FOR_RADIO_MESSAGEState::E_START_RELAY(statemachineContext &context) const
{
  // 1 transition(s)
  BusinessObject& myBusinessObject = context.getBusinessObject();
  if((relay) == true){
    context.SetTransitionName("E_START_RELAY[relay]");
    fsm::ProcessTransitionPre<statemachineContext, statemachineUsainBoatState>(context, &statemachineS_RELAYState::GetInstance());
    // No action to do
    fsm::ProcessTransitionPost<statemachineContext, statemachineUsainBoatState>(context, &statemachineS_RELAYState::GetInstance());
    return;
  }
  // pass the event to the parent state
  statemachineUsainBoatState::GetInstance().E_START_RELAY(context);
}

void statemachineS_WAIT_FOR_RADIO_MESSAGEState::E_STOP_STATEMACHINE(statemachineContext &context) const
{
  // 1 transition(s)
  BusinessObject& myBusinessObject = context.getBusinessObject();
  context.SetTransitionName("E_STOP_STATEMACHINE");
  fsm::ProcessTransitionPre<statemachineContext, statemachineUsainBoatState>(context, &statemachineEndState::GetInstance());
  // No action to do
  fsm::ProcessTransitionPost<statemachineContext, statemachineUsainBoatState>(context, &statemachineEndState::GetInstance());
  context.OnEnd();
  return;
}



/**************************************************************
 * State implementation for statemachineS_VOLGPLANState
 **************************************************************/

// State Constructor
statemachineS_VOLGPLANState::statemachineS_VOLGPLANState(const char* pName, int id) : statemachineUsainBoatState(pName, id)
{
  m_pStateParent = &statemachineUsainBoatState::GetInstance();
}

// State Destructor
statemachineS_VOLGPLANState::~statemachineS_VOLGPLANState()
{
}

// State GetInstance
const statemachineS_VOLGPLANState& statemachineS_VOLGPLANState::GetInstance()
{
  return kstatemachineS_VOLGPLANState;
}

// OnEntry
void statemachineS_VOLGPLANState::OnEntry(statemachineContext &context) const
{
  // OnEntry for state statemachineS_VOLGPLANState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), statemachineS_VOLGPLANState::GetInstance().GetName());
  }
}

// OnExit
void statemachineS_VOLGPLANState::OnExit(statemachineContext &context) const
{
  // OnExit for state statemachineS_VOLGPLANState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), statemachineS_VOLGPLANState::GetInstance().GetName());
  }
}

// Events implementation for state  statemachineS_VOLGPLANState
void statemachineS_VOLGPLANState::E_WAIT_NEXT_COMMAND(statemachineContext &context) const
{
  // 1 transition(s)
  BusinessObject& myBusinessObject = context.getBusinessObject();
  if((state_done) == true){
    context.SetTransitionName("E_WAIT_NEXT_COMMAND[state_done]");
    fsm::ProcessTransitionPre<statemachineContext, statemachineUsainBoatState>(context, &statemachineS_WAIT_FOR_RADIO_MESSAGEState::GetInstance());
    // No action to do
    fsm::ProcessTransitionPost<statemachineContext, statemachineUsainBoatState>(context, &statemachineS_WAIT_FOR_RADIO_MESSAGEState::GetInstance());
    return;
  }
  // pass the event to the parent state
  statemachineUsainBoatState::GetInstance().E_WAIT_NEXT_COMMAND(context);
}



/**************************************************************
 * State implementation for statemachineEndState
 **************************************************************/

// State Constructor
statemachineEndState::statemachineEndState(const char* pName, int id) : statemachineUsainBoatState(pName, id)
{
  m_eStateKind = fsm::STATE_FINAL;
  m_pStateParent = &statemachineUsainBoatState::GetInstance();
}

// State Destructor
statemachineEndState::~statemachineEndState()
{
}

// State GetInstance
const statemachineEndState& statemachineEndState::GetInstance()
{
  return kstatemachineEndState;
}

// OnEntry
void statemachineEndState::OnEntry(statemachineContext &context) const
{
  // OnEntry for state statemachineEndState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), statemachineEndState::GetInstance().GetName());
  }
}

// OnExit
void statemachineEndState::OnExit(statemachineContext &context) const
{
  // OnExit for state statemachineEndState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), statemachineEndState::GetInstance().GetName());
  }
}

// Events implementation for state  statemachineEndState
void statemachineEndState::E_START_FOLLOW(statemachineContext &context) const
{
  // 0 transition(s)
}

void statemachineEndState::E_START_RELAY(statemachineContext &context) const
{
  // 0 transition(s)
}

void statemachineEndState::E_START_MANUAL(statemachineContext &context) const
{
  // 0 transition(s)
}

void statemachineEndState::E_WAIT_NEXT_COMMAND(statemachineContext &context) const
{
  // 0 transition(s)
}

void statemachineEndState::E_STOP_STATEMACHINE(statemachineContext &context) const
{
  // 0 transition(s)
}



/**************************************************************
 * State implementation for statemachineS_RELAYState
 **************************************************************/

// State Constructor
statemachineS_RELAYState::statemachineS_RELAYState(const char* pName, int id) : statemachineUsainBoatState(pName, id)
{
  m_pStateParent = &statemachineUsainBoatState::GetInstance();
}

// State Destructor
statemachineS_RELAYState::~statemachineS_RELAYState()
{
}

// State GetInstance
const statemachineS_RELAYState& statemachineS_RELAYState::GetInstance()
{
  return kstatemachineS_RELAYState;
}

// OnEntry
void statemachineS_RELAYState::OnEntry(statemachineContext &context) const
{
  // OnEntry for state statemachineS_RELAYState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), statemachineS_RELAYState::GetInstance().GetName());
  }
}

// OnExit
void statemachineS_RELAYState::OnExit(statemachineContext &context) const
{
  // OnExit for state statemachineS_RELAYState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), statemachineS_RELAYState::GetInstance().GetName());
  }
}

// Events implementation for state  statemachineS_RELAYState
void statemachineS_RELAYState::E_WAIT_NEXT_COMMAND(statemachineContext &context) const
{
  // 1 transition(s)
  BusinessObject& myBusinessObject = context.getBusinessObject();
  context.SetTransitionName("E_WAIT_NEXT_COMMAND");
  fsm::ProcessTransitionPre<statemachineContext, statemachineUsainBoatState>(context, &statemachineS_WAIT_FOR_RADIO_MESSAGEState::GetInstance());
  // No action to do
  fsm::ProcessTransitionPost<statemachineContext, statemachineUsainBoatState>(context, &statemachineS_WAIT_FOR_RADIO_MESSAGEState::GetInstance());
  return;
}



/**************************************************************
 * State implementation for statemachineS_MANUAL_CONTROLState
 **************************************************************/

// State Constructor
statemachineS_MANUAL_CONTROLState::statemachineS_MANUAL_CONTROLState(const char* pName, int id) : statemachineUsainBoatState(pName, id)
{
  m_pStateParent = &statemachineUsainBoatState::GetInstance();
}

// State Destructor
statemachineS_MANUAL_CONTROLState::~statemachineS_MANUAL_CONTROLState()
{
}

// State GetInstance
const statemachineS_MANUAL_CONTROLState& statemachineS_MANUAL_CONTROLState::GetInstance()
{
  return kstatemachineS_MANUAL_CONTROLState;
}

// OnEntry
void statemachineS_MANUAL_CONTROLState::OnEntry(statemachineContext &context) const
{
  // OnEntry for state statemachineS_MANUAL_CONTROLState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), statemachineS_MANUAL_CONTROLState::GetInstance().GetName());
  }
}

// OnExit
void statemachineS_MANUAL_CONTROLState::OnExit(statemachineContext &context) const
{
  // OnExit for state statemachineS_MANUAL_CONTROLState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), statemachineS_MANUAL_CONTROLState::GetInstance().GetName());
  }
}

// Events implementation for state  statemachineS_MANUAL_CONTROLState
void statemachineS_MANUAL_CONTROLState::E_WAIT_NEXT_COMMAND(statemachineContext &context) const
{
  // 1 transition(s)
  BusinessObject& myBusinessObject = context.getBusinessObject();
  context.SetTransitionName("E_WAIT_NEXT_COMMAND");
  fsm::ProcessTransitionPre<statemachineContext, statemachineUsainBoatState>(context, &statemachineS_WAIT_FOR_RADIO_MESSAGEState::GetInstance());
  // No action to do
  fsm::ProcessTransitionPost<statemachineContext, statemachineUsainBoatState>(context, &statemachineS_WAIT_FOR_RADIO_MESSAGEState::GetInstance());
  return;
}








// Reenable some compiler warnings
#ifdef _MSC_VER
#pragma warning(pop)
#endif