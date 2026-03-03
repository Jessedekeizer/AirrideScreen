#include "Communication.h"

Communication::Communication(ISerial &serial): nextId(1), serial(serial) {
}

Communication::~Communication() {
    subscribers.clear();
}

void Communication::Setup() {
    serial.Init(9600);
}

unsigned int Communication::Subscribe(Callback callback) {
    Subscription subscriber;
    subscriber.id = nextId++;
    subscriber.callback = callback;
    subscribers.push_back(subscriber);
    return subscriber.id;
}

void Communication::Unsubscribe(int id) {
    if (id == -1) {
        return;
    }
    for (int i = 0; i < subscribers.size(); i++) {
        if (subscribers[i].id == id) {
            subscribers.erase(subscribers.begin() + i);
            return;
        }
    }
}

void Communication::Notify(String message) {
    for (auto subscriber : subscribers) {
        subscriber.callback(message);
    }
}

void Communication::CheckForMessage() {
    if (!serial.ReceiveAvailable()) {
        return;
    }
    while (serial.ReceiveAvailable()) {
        String message;
        message = serial.Receive();
        Notify(message);
    }
}

void Communication::SendMessage(String message) {
    serial.SendMessage(message);
}
