#ifndef Modular_h
#define Modular_h

#include "Arduino.h"

enum ComponentState {
  idle,
  working,
  error,
  interrupt
};

struct Message
{
  public:
    //void switchAdresses();
    bool hasMessage = true;
    int target;
    int sender;
    String message;
};

class Component
{
  public:
    Component();
    //Component(int newAdress, int newTarget);
    //virtual Message componentLoop();
    //virtual bool recieveMessage(Message transmission);
    void setInterrupt(bool enableInterrupt);
    int getAdress();

  protected:
    ComponentState _componentState = idle; //ToDo: Replace _Underline
    ComponentState stateBeforeInterrupt;
    //int adress;
    //int target;
  };

class CommunicationConnection {
  public:
    CommunicationConnection(int *adressList, unsigned int numberOfAdresses);
    virtual String listen();
    virtual bool sendMessage(Message transmission);
    bool hasConnectionAttached(int connection);

  private:
    int *adressList;
    unsigned int numberOfAdresses;

};

class CommunicationNode {
  public:
    //CommunicationNode();
    CommunicationNode(Component *firstComponent, Component *secondComponent, Component *thirdComponent, Component *fourthComponent, Component *fifthComponent, CommunicationConnection *firstCommunicationConnection);
    //CommunicationNode(Component *componentList, unsigned int numberOfComponents, CommunicationConnection *communicationList, unsigned int numberOfConnections);
    void setInterrput(bool enableInterrupt);
    void loopAllAttached();
    bool sendMessage(Message transmission);

  private:
    Component *firstComponent;
    Component *secondComponent;
    Component *thirdComponent;
    Component *fourthComponent;
    Component *fifthComponent;
    CommunicationConnection *firstCommunicationConnection;

    /*
    Component *componentList;
    unsigned int numberOfComponents;
    CommunicationConnection *communicationList;
    unsigned int numberOfConnections;
    */
    /*
    Component attachedComponents[10];
    CommunicationConnection attachedConnections[5]; //id 0 = always top connection
    */

};

#endif
