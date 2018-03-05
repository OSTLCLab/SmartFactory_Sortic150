#include "Modular.h"
/*
void Message::switchAdresses() {
  int a = target;
  target = sender;
  sender = a;
}
*/

//some code if needed

//Component::Component(int newAdress, int newTarget) {
//Component::Component(int adress, int target) {
Component::Component() {
  //this->adress = adress;
  //this->target = target;
}

void Component::setInterrupt(bool enableInterrupt) {
  if(enableInterrupt) {
    stateBeforeInterrupt = _componentState;
    _componentState = interrupt;
  }
  else {
    if(_componentState == interrupt) {
      _componentState = stateBeforeInterrupt;
    }
  }
}


int Component::getAdress() {
  //return adress;
  return 1;
}

//CommunicationConnection::CommunicationConnection(int adress) { //(&newAttachedAdresses)[10]
CommunicationConnection::CommunicationConnection(int *adressList, unsigned int numberOfAdresses) { //(&newAttachedAdresses)[10]
 this->adressList = adressList;
 this->numberOfAdresses = numberOfAdresses;
}

bool CommunicationConnection::hasConnectionAttached(int connection){
  for(unsigned int i = 0; i < numberOfAdresses; i++) {
    if(*(adressList+i)==connection) return true;
  }
  return false;

}

//CommunicationNode::CommunicationNode() {

CommunicationNode::CommunicationNode(Component *firstComponent, Component *secondComponent, Component *thirdComponent, Component *fourthComponent, Component *fifthComponent, CommunicationConnection *firstCommunicationConnection) {
  this->firstComponent = firstComponent;
  this->secondComponent = secondComponent;
  this->thirdComponent = thirdComponent;
  this->fourthComponent = fourthComponent;
  this->fifthComponent = fifthComponent;
  this->firstCommunicationConnection = firstCommunicationConnection;
}

/*
CommunicationNode::CommunicationNode(Component *componentList, unsigned int numberOfComponents, CommunicationConnection *communicationList, unsigned int numberOfConnections) {
  this->componentList = componentList;
  this->numberOfComponents = numberOfComponents;
  this->communicationList = communicationList;
  this->numberOfConnections = numberOfConnections;
}
*/


void CommunicationNode::setInterrput(bool enableInterrupt) {
  /*
  for(unsigned int i = 0; i < sizeof(attachedComponents)/sizeof(attachedComponents[0]); i++) {
    attachedComponents[i].setInterrupt(enableInterrupt);
  }
  */
}

bool CommunicationNode::sendMessage(Message transmission) {
  //Check components
  /*firstCommunicationConnection->sendMessage(transmission);

  if(firstComponent->getAdress() == transmission.target) return firstComponent->recieveMessage(transmission);
  if(secondComponent->getAdress() == transmission.target) return secondComponent->recieveMessage(transmission);
  if(thirdComponent->getAdress() == transmission.target) return thirdComponent->recieveMessage(transmission);
  if(fourthComponent->getAdress() == transmission.target) return fourthComponent->recieveMessage(transmission);
  if(fifthComponent->getAdress() == transmission.target) return fifthComponent->recieveMessage(transmission);
*/
  return true; //firstCommunicationConnection->sendMessage(transmission);
  /*
  for(unsigned int i = 0; i < numberOfComponents; i++) {
    if(transmission.target == (componentList+i*2)->getAdress()) {
      return (componentList+i*2)->recieveMessage(transmission);
    }
  }

  //Check connections
  for(unsigned int i = 0; i<numberOfConnections; i++) {
    if((communicationList+i*2)->hasConnectionAttached(transmission.target)){
      return (communicationList+i*2)->sendMessage(transmission);
    }
  }
  return (communicationList)->sendMessage(transmission);
  */
}

void CommunicationNode::loopAllAttached() {
  Message currentMessage;

  /*
  //Output pointer adresses
  currentMessage.hasMessage = true;
  int pointerAdress = (unsigned int)&firstComponent;
  String output = "adress ";
  output += pointerAdress;
  currentMessage.message = output;// + pointerAdress;
  currentMessage.target = 1;
  this->sendMessage(currentMessage);

  pointerAdress = (unsigned int)&secondComponent;
  output = "adress ";
  output += pointerAdress;
  currentMessage.message = output;
  this->sendMessage(currentMessage);

  pointerAdress = (unsigned int)&thirdComponent;
  output = "adress ";
  output += pointerAdress;
  currentMessage.message = output;
  this->sendMessage(currentMessage);

  pointerAdress = (unsigned int)&fourthComponent;
  output = "adress ";
  output += pointerAdress;
  currentMessage.message = output;
  this->sendMessage(currentMessage);

  delay(1000);
  */

  /*
  //Increment and output pointer adress
  Component *componentPointer = firstComponent;
  componentPointer++;
  pointerAdress = (unsigned int)&componentPointer;
  output = "adress ";
  output += pointerAdress;
  currentMessage.message = output;// + pointerAdress;
  this->sendMessage(currentMessage);

  delay(1000);
  */

  /*
  //Loop trought all components test -> Crashes
  for(int i = 0; i<1; i++) {
    currentMessage = (firstComponent+i*2)->componentLoop();
    if(currentMessage.hasMessage == true) {
      this->sendMessage(currentMessage);
    }
  }
  */

  /*
  //Pointer increment test -> Crashes
  Component *componentPointer = firstComponent;

  currentMessage = (componentPointer)->componentLoop();
  if(currentMessage.hasMessage == true) {
    this->sendMessage(currentMessage);
  }

  componentPointer++;
  currentMessage = (componentPointer)->componentLoop();
  if(currentMessage.hasMessage == true) {
    this->sendMessage(currentMessage);
  }
  */



  // Old and terrible way -> works
  /*currentMessage = firstComponent->componentLoop();
  if(currentMessage.hasMessage == true) {
    this->sendMessage(currentMessage);
  }

  currentMessage = secondComponent->componentLoop();
  if(currentMessage.hasMessage == true) {
    this->sendMessage(currentMessage);
  }

  currentMessage = thirdComponent->componentLoop();
  if(currentMessage.hasMessage == true) {
    this->sendMessage(currentMessage);
  }

  currentMessage = fourthComponent->componentLoop();
  if(currentMessage.hasMessage == true) {
    this->sendMessage(currentMessage);
  }

  currentMessage = fifthComponent->componentLoop();
  if(currentMessage.hasMessage == true) {
    this->sendMessage(currentMessage);
  }*/


  /*
  //-> Same implementation for communication connections -> works with 1, crashes with more
  for(int i = 0; i<1; i++) {
    String recievedMessage = (firstCommunicationConnection+i*2)->listen();
    if(!recievedMessage.equalsIgnoreCase("")) { //adress:sender:message
      //ToDo: Sepparate message

      Message currentMessage;
      int indexA;
      int indexB;
      String substring;

      indexA = recievedMessage.indexOf(';');
      substring = recievedMessage.substring(0,indexA);
      currentMessage.target = substring.toInt();

      indexB = recievedMessage.indexOf(';',indexA+1);
      substring = recievedMessage.substring(indexA+1,indexB);
      currentMessage.sender = substring.toInt();

      currentMessage.message = recievedMessage.substring(indexB+1);

      if(this->sendMessage(currentMessage)) {
        //If recieved correctly;

        currentMessage.message = "Message delivered";
        currentMessage.target = 1;
        currentMessage.sender = 1;
        this->sendMessage(currentMessage);

      }
      else {
        //If not recieved correctly

        //currentMessage.switchAdresses();
        //currentMessage.message = "error:" + currentMessage.message;

        currentMessage.message = "Error: Target = " + currentMessage.target;
        currentMessage.message = currentMessage.message + ", Sender = " + currentMessage.sender; + ", Message = " + currentMessage.message;
        currentMessage.target = 1;
        currentMessage.sender = 1;
        this->sendMessage(currentMessage);
      }
    }
  }
  */


  // Old and terrible way -> crashes
  /*String recievedMessage = firstCommunicationConnection->listen();
  if(!recievedMessage.equalsIgnoreCase("")) { //adress:sender:message
    //ToDo: Sepparate message

    Message currentMessage;
    int indexA;
    int indexB;
    String substring;

    indexA = recievedMessage.indexOf(';');
    substring = recievedMessage.substring(0,indexA);
    currentMessage.target = substring.toInt();

    indexB = recievedMessage.indexOf(';',indexA+1);
    substring = recievedMessage.substring(indexA+1,indexB);
    currentMessage.sender = substring.toInt();

    currentMessage.message = recievedMessage.substring(indexB+1);

    if(this->sendMessage(currentMessage)) {
      //If recieved correctly;

      currentMessage.message = "Message delivered";
      currentMessage.target = 1;
      currentMessage.sender = 1;
      this->sendMessage(currentMessage);

    }
    else {
      //If not recieved correctly

      //currentMessage.switchAdresses();
      //currentMessage.message = "error:" + currentMessage.message;

      currentMessage.message = "Error: Target = " + currentMessage.target;
      currentMessage.message = currentMessage.message + ", Sender = " + currentMessage.sender; + ", Message = " + currentMessage.message;
      currentMessage.target = 1;
      currentMessage.sender = 1;
      this->sendMessage(currentMessage);
    }
  } */


  // VVV Older tests VVV

  /*
  Message currentMessage = (componentList+1)->componentLoop();

  if(currentMessage.hasMessage == true) {
    this->sendMessage(currentMessage);
  }

  Message testMessage;
  testMessage.hasMessage = true;
  testMessage.message = "test";
  testMessage.target = 1;
  testMessage.sender = 1;

  this->sendMessage(testMessage);

  */
  /*
  Component * nextComponent = componentList +1;
  currentMessage = nextComponent->componentLoop();

  if(currentMessage.hasMessage == true) {
    this->sendMessage(currentMessage);
  }
  */
  /*
  for(unsigned int i = 0; i<numberOfComponents; i++) {
    Message currentMessage = (componentList+i)->componentLoop();

    if(currentMessage.hasMessage == true) {
      this->sendMessage(currentMessage);
    }

  }
  */

  /*
  for(unsigned int i = 0; i<numberOfConnections; i++) {
    String recievedMessage = (communicationList+i)->listen();

    if(!recievedMessage.equalsIgnoreCase("")) { //adress:sender:message
      //ToDo: Sepparate message

      Message currentMessage;
      int indexA;
      int indexB;
      String substring;

      indexA = recievedMessage.indexOf(';');
      substring = recievedMessage.substring(0,indexA);
      currentMessage.target = substring.toInt();

      indexB = recievedMessage.indexOf(';',indexA+1);
      substring = recievedMessage.substring(indexA+1,indexB);
      currentMessage.sender = substring.toInt();

      currentMessage.message = recievedMessage.substring(indexB+1);

      if(this->sendMessage(currentMessage)) {
        //If recieved correctly;

        currentMessage.message = "Message delivered";
        currentMessage.target = 1;
        currentMessage.sender = 1;
        this->sendMessage(currentMessage);

      }
      else {
        //If not recieved correctly

        //currentMessage.switchAdresses();
        //currentMessage.message = "error:" + currentMessage.message;

        currentMessage.message = "Error: Target = " + currentMessage.target;
        currentMessage.message = currentMessage.message + ", Sender = " + currentMessage.sender; + ", Message = " + currentMessage.message;
        currentMessage.target = 1;
        currentMessage.sender = 1;
        this->sendMessage(currentMessage);
      }
    }
  }*/

}
