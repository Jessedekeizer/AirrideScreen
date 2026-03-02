#include "Communication.h"

Communication::~Communication() {
    subscribers.clear();
}

unsigned int Communication::Subscribe(Callback callback) {
    Subscription subscriber;
    subscriber.id = nextId++;
    subscriber.callback = callback;
    subscribers.push_back(subscriber);
    return subscriber.id;
}

void Communication::Unsubscribe(int id) {
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
    String message;
    message = serial.Receive();
    Notify(message);
}
