#include "CommunicationDistributor.h"

CommunicationDistributor::~CommunicationDistributor() {
    subscribers.clear();
}

unsigned int CommunicationDistributor::Subscribe(Callback callback) {
    Subscription subscriber;
    subscriber.id = nextId++;
    subscriber.callback = callback;
    subscribers.push_back(subscriber);
    return subscriber.id;
}

void CommunicationDistributor::Unsubscribe(int id) {
    for (int i = 0; i < subscribers.size(); i++) {
        if (subscribers[i].id == id) {
            subscribers.erase(subscribers.begin() + i);
            return;
        }
    }
}

void CommunicationDistributor::Notify(String message) {
    for (auto subscriber : subscribers) {
        subscriber.callback(message);
    }
}
